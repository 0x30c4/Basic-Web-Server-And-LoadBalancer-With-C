#ifndef _GET_H_
#define _GET_H_

#include <common.h>
#include <error.h>

Request *parse_request(const char *raw, MimeType mimes[], size_t mime_len);
void free_request(Request *req);
bool is_valid_url(const char *url);
int check_resource(const char * path);
void mime_type(char *path, MimeType mimes[], size_t *mime_len);
void free_mime(MimeType mimes[], size_t mime_len);

#endif
