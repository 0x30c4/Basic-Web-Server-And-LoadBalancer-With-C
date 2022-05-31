#include <common.h>
#include <proxy.h>
#include <server.h>


int main(int argc, char const *argv[]) {

    /* proxy_fd("127.0.0.1", 1111, 0, 1); */
    /* proxy_fd("192.168.0.1", 80, 0, 1); */
    Backend backend[2];
    int backend_len = 2;

    backend[0].address = "127.0.0.1";
    backend[1].address = "127.0.0.1";

    backend[0].port = 1111;
    backend[1].port = 2222;

    epoll_driver(2121, backend, backend_len, ROUND_ROBIN);

    return 0;
}
