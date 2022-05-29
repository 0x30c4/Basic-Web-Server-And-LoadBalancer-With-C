#ifndef _FILE_H_
#define _FILE_H_

#include "common.h"

bool check_if_file_exists(const char *);
void read_and_write_fd(int fd_read, int fd_write, size_t buff_size);
char *read_header_fd(int fd_read);
size_t content_len(const char * path);

#endif
