#ifndef _SERVER_H_
#define _SERVER_H_

#include <common.h>

int create_socket(int port);
void epoll_driver(int port, Backend backend[], int backend_len, int loadbalancer_algo, char *mime_file);

#endif
