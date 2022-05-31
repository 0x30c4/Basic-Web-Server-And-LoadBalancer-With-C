#include <proxy.h>
#include <file.h>
#include <serve.h>


/*
 * Function: proxy_fd
 * ----------------------------
 *   Creates a reverse proxy for the backend. 
 *   
 *   address: the backend ip.
 *   port: the backend port.
 *   fd_read: the writing fd.
 *   fd_out: the writing fd.
 *
 *   returns: void
 */

void proxy_fd(const char *address, int port, int fd_read, int fd_out) {
    int server_fd;
    struct sockaddr_in server;
    int len;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Cannot create socket");
        return;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(address);
    server.sin_port = htons(port);
    len = sizeof(server);
    if (connect(server_fd, (struct sockaddr *)&server, len) < 0) {
        fprintf(stderr, "Cannot connect to backend %s:%d", address, port);
        return;
    }

    pid_t p_pid = getpid();
    fork();
    if (p_pid == getpid()){
        read_and_write_fd(fd_read, server_fd, 1);
    }else{
        read_and_write_fd(server_fd, fd_out, 1);
    }
    close(server_fd);
}

