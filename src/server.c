#include <server.h>
#include <file.h>
#include <serve.h>
#include <get.h>
#include <common.h>
#include <proxy.h>

#define MAX_CLIENTS 1024 * 4
#define MAX_EVENTS 1024 * 4

/*
 * Function: create_socket
 * ----------------------------
 *   Creates a server socket. And returns server socket.
 *
 *   port: server listen port
 *
 *   returns: int 
 */

int create_socket(int port) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int bindResult = bind(server_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (bindResult == -1) {
        perror("bindResult");
        /* exit(-1); */
    }

    int listenResult = listen(server_socket, 5);
    if (listenResult == -1) {
        perror("listenResult");
        exit(-1);
    }
    printf("Server Started\n");
    return server_socket;
}

/*
 * Function: epoll_driver
 * ----------------------------
 *   Main driver function for the epoll and it handels
 *   all the network traffic.
 *
 *  port: server listen port.
 *  backend: the array of backends.
 *  backend_len: the lenght of backend.
 *  mime_file: the mime file.
 */

void epoll_driver(int port, Backend backend[], int backend_len, int loadbalancer_algo, char *mime_file) {
    int listen_sock = create_socket(port);

    struct epoll_event ev, events[MAX_EVENTS];
    int conn_sock, nfds, epollfd;
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    int client_sockets[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }

    // Setting the mimes.
    MimeType mimes[128];
    size_t mime_len = 0;
    int lb = 0;
    mime_type(mime_file, mimes, &mime_len);

	Global data;

    data.mimes = mimes;
    data.mime_len = mime_len;
    while (1) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, 1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listen_sock) {
                struct sockaddr_in cliaddr;
                socklen_t addrlen = sizeof(cliaddr);

                conn_sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
                if (conn_sock == -1) {
                    perror("accept");
                }

                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (client_sockets[i] == 0) {
                        client_sockets[i] = conn_sock;
                        ev.events = EPOLLIN | EPOLLET;
                        ev.data.fd = client_sockets[i];
                        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, client_sockets[i], &ev) == -1) {
                            perror("epoll_ctl: conn_sock");
                        }
                        break;
                    }
                }
            }
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] > 0 && events[n].data.fd == client_sockets[i]) {

                    /* serve_fd(client_sockets[i], client_sockets[i], &data); */

                    if (loadbalancer_algo == ROUND_ROBIN) {
                        printf("\nConnected to %s:%d\n", backend[lb].address, backend[lb].port);
                        proxy_fd(backend[lb].address, backend[lb].port, client_sockets[i], client_sockets[i]);
                        if (lb == backend_len - 1)
                            lb = 0;
                        else
                            lb++;
                    } else if (loadbalancer_algo == STATIC_SERVER) {
                        serve_fd(client_sockets[i], client_sockets[i], &data);
                    }

                    epoll_ctl(epollfd, EPOLL_CTL_DEL, client_sockets[i], NULL);
                }
            }
        }
    }
    close(listen_sock);
}
