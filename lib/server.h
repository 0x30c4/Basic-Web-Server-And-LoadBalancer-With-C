#ifndef _SERVER_H_
#define _SERVER_H_

#include <common.h>

int create_socket(int port);
void epoll_driver();
// void identifyClient(int);
#endif
