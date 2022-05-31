#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <argp.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <limits.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>


//Bash colors and other formatting.
//Colors
#define RED            "\e[31m"
#define GREEN          "\e[32m"
#define LIGHTGREEN     "\e[32m"

//Formats.
#define RESETALL       "\e[0m"
#define BOLD           "\e[1m"
#define UNDERLINE      "\e[4m"
#define BLINK          "\e[5m"

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

typedef struct Global {
    MimeType *mimes;
    int mime_len;
} Global;

typedef struct Backend {
    char *address;
    int port;
} Backend;

typedef struct Conf {
    int backends_len;
    char *mime_file;
    int listen_port; 
    char *www_path;
} Conf;


#define OK_H "200 OK\0"
#define BAD_REQ_H "400 Bad Request\0"
#define FORBIDDEN_H "403 Forbidden\0"
#define NOT_FOUND_H "404 Not Found\0"
#define INTERNAL_SERVER_ERROR_H "500 Internal Server Error\0"

#define OK_LEN 6
#define BAD_REQ_LEN 15
#define FORBIDDEN_LEN 13
#define NOT_FOUND_LEN 13
#define INTERNAL_SERVER_ERROR_LEN 25

#define BAD_REQ_PAGE "./error/400.html\0"
#define FORBIDDEN_PAGE "./error/403.html\0"
#define NOT_FOUND_PAGE "./error/404.html\0"
#define INTERNAL_SERVER_ERROR_PAGE "./error/500.html\0"

#define BAD_REQ_PAGE_LEN 17
#define FORBIDDEN_PAGE_LEN 17
#define NOT_FOUND_PAGE_LEN 17
#define INTERNAL_SERVER_ERROR_PAGE_LEN 17


#define CRLF "\r\n"

#define SERVER_NAME "BigPP/beta-0.2"

#define ROUND_ROBIN 0
#define LEAST_CONN 1
#define STATIC_SERVER 2
#define MAX_BACKENDS 10


#define BUFSIZE 1024 


#endif
