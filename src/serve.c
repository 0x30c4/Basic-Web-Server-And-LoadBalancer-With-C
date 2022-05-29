#include <serve.h>
#include <get.h>
#include <file.h>

/*
 * Function: serve_fd
 * ----------------------------
 *   Read and process a request from client and then 
 *   serve the requested resource.
 *
 *   fd_in: the client fd for reading the request.
 *   fd_out: the client fd for writing to back to the client.
 *
 *   returns: int
 */

int serve_fd(int fd_in, int fd_out, Global *data) {

    char *raw_request = read_header_fd(fd_in);

    Request *req = parse_request(raw_request, data->mimes, data->mime_len);

    // Start building the response header.
    char *resp_header = malloc(1024);
    char *header = NULL;
    memset(resp_header, '\0', 1024);

    // Check the status code and send response accordingly. 
    // And set the error file also.
    switch (req->http_code) {

        case OK:
            header = malloc(OK_LEN);
            memcpy(header, OK_H, OK_LEN);
            break;
        case BAD_REQUEST:
            header = malloc(BAD_REQ_LEN);
            memcpy(header, BAD_REQ_H, BAD_REQ_LEN);
            memcpy(req->url, BAD_REQ_PAGE, BAD_REQ_PAGE_LEN);
            break;
        case FORBIDDEN:
            header = malloc(FORBIDDEN_LEN);
            memcpy(header, FORBIDDEN_H, FORBIDDEN_LEN);
            memcpy(req->url, FORBIDDEN_PAGE, FORBIDDEN_PAGE_LEN);
            break;
        case NOT_FOUND:
            header = malloc(NOT_FOUND_LEN);
            memcpy(header, NOT_FOUND_H, NOT_FOUND_LEN);
            memcpy(req->url, NOT_FOUND_PAGE, NOT_FOUND_PAGE_LEN);
            break;
            break;
        case INTERNAL_SERVER_ERROR:
            header = malloc(INTERNAL_SERVER_ERROR_LEN);
            memcpy(header, INTERNAL_SERVER_ERROR_H, INTERNAL_SERVER_ERROR_LEN);
            memcpy(req->url, INTERNAL_SERVER_ERROR_PAGE, INTERNAL_SERVER_ERROR_PAGE_LEN);
            break;
    }

    sprintf(resp_header, 
            "HTTP/1.1 %s%sServer: %s%sContent-Type: %s%sContent-Length: %ld%sConnection: close%s%s",
            header, CRLF, SERVER_NAME, CRLF, req->type, CRLF, content_len(req->url), CRLF, CRLF, CRLF);

    printf("-----------------\n");

    // Send the header to the client.
    write(fd_out, resp_header, strlen(resp_header));

    // Send the content to the client.
    
    int file_fd = open(req->url, O_RDONLY);

    read_and_write_fd(file_fd, fd_out, 1);

    // Printing header to the stdout.
    printf("%s", resp_header);


    close(file_fd);

    printf("\n-----------------\n");

    /* free_request(req); */
    free(header);
    free(resp_header);
    free(raw_request);
    free(req);

    return 0;
}
