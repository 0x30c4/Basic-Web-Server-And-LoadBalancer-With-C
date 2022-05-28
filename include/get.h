#ifndef _GET_H_
#define _GET_H_

#include <common.h>
#include <error.h>


typedef enum Method {UNSUPPORTED, GET, HEAD} Method;

typedef struct Request {
    enum Method method;
    char *url;
    char *version;
	char *type;
	int http_code;
} Request;

typedef struct MimeType {
    char *header;
    char *extension;
} MimeType;


Request *parse_request(const char *raw, MimeType mimes[], size_t mime_len);
void free_request(Request *req);
bool is_valid_url(const char *url);
int check_resource(const char * path);
void mime_type(char *path, MimeType mimes[], size_t *mime_len);
void free_mime(MimeType mimes[], size_t mime_len);

#endif
