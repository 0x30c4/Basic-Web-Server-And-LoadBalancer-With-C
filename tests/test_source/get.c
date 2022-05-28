#include <common.h>
#include <get.h>
#include <file.h>

int main(int argc, char *argv[]){

    // Load mime type from file.
    
    MimeType mimes[128];
    size_t mime_len = 0;
    mime_type("../mime.types", mimes, &mime_len);

    /* for (int i = 0; i < mime_len; i++){ */
    /*     printf("%s | %s", mimes[i].header, mimes[i].extension); */
    /* } */

	// Test the parse_request.
    char *raw_request = "GET /?fbclick=11 HTTP/1.1\r\n";

    struct Request *req = parse_request(raw_request, mimes, mime_len);
    if (req) {
        printf("Method: %d\n", req->method);
        printf("Request-URI: %s\n", req->url);
        printf("HTTP-Version: %s\n", req->version);
        printf("Content-Type: %s\n", req->type);

		if (req->http_code == OK)
        	printf("HTTP-Status-code: %d | OK\n", req->http_code);

		if (req->http_code == BAD_REQUEST)
        	printf("HTTP-Status-code: %d | BAD_REQUEST\n", req->http_code);

		if (req->http_code == FORBIDDEN)
        	printf("HTTP-Status-code: %d | FORBIDDEN\n", req->http_code);

		if (req->http_code == NOT_FOUND)
        	printf("HTTP-Status-code: %d | NOT_FOUND\n", req->http_code);

		if (req->http_code == INTERNAL_SERVER_ERROR)
        	printf("HTTP-Status-code: %d | INTERNAL_SERVER_ERROR\n", req->http_code);

		printf("\n\n");
    }
    free_request(req);
    free_mime(mimes, mime_len);

	// Test the is_valid_url
	if (is_valid_url("/asd"))
		printf("/asd | is valid.\n");

	if (!is_valid_url(".../aad/.."))
		printf(".../aad/.. | is not valid.\n");

	if (!is_valid_url("aad/."))
		printf("aad/. | is not valid.\n");

	if (!is_valid_url("aad../"))
		printf("aad../ | is not valid.\n");

	if (!is_valid_url("aad./."))
		printf("aad./. | is not valid.\n");

    printf("\n\n\n\n");
    // Test read file.

    int fd_read = open("HTTP_HEADER.txt", O_RDONLY);

    read_and_write_fd(fd_read, 1, 1);

    close(fd_read);

	return 0;
}
