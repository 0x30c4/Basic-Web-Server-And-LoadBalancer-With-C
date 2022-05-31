#ifndef _PROXY_H_
#define _PROXY_H_

#include <common.h>

void proxy_fd(const char *address, int port, int fd_read, int fd_out);

#endif
