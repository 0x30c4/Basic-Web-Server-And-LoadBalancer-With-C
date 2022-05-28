#include <file.h>
#include <get.h>


/*
 * Function: is_valid_url
 * ----------------------------
 *   Checks if the url is valid if so then returns
 *   True else False.
 *
 *   url: http url which will be processed.
 *
 *   returns: boolean
 */

bool is_valid_url(const char * url) {
	if (strstr(url, "/..") || strstr(url, "./") ||
			strstr(url, "../") || strstr(url, "//") || 
			strstr(url, "/./") || strstr(url, "/../") ||
			strstr(url, "/."))
		return false;
	return true;
}

/*
 * Function: check_resource
 * ----------------------------
 *   Checks if the path is valid and the requested
 *   resource is available.
 *
 *   path: if check if the path exist.
 *
 *   returns: boolean
 */

int check_resource(const char * path) {
	// Check if file exists.
	if (!check_if_file_exists(path)) 
		return NOT_FOUND;
	return OK;
}


/*
 * Function: mime_type
 * ----------------------------
 *   Checks if the path is valid and the requested
 *   resource is available.
 *
 *   path: mime.types file path.
 *   mimes[]: struct array that contains mime data.
 *   mime_len: mime_len struct array lenght.
 *
 *   returns: void
 */

void mime_type(char *path, MimeType mimes[], size_t *mime_len) {

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(path, "r");
    if (fp == NULL){
        fprintf(stderr, "%s can't be opened.", path);
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        /* printf("Retrieved line of length %zu:\n", read); */
        if (read != 1) {
            // get the content type.
            size_t con_type = strcspn(line, " ");
            mimes->header = malloc(con_type + 1);
            memcpy(mimes->header, line, con_type);
            mimes->header[con_type] = '\0';


            size_t con_ext = strcspn(line, ";");
            mimes->extension = malloc(con_ext + 1);
            memcpy(mimes->extension, line + con_type + 1, con_ext);
            mimes->extension[con_type] = '\0';

            // Increment the value of mime_len.
            (*mime_len)++;
            mimes++;
        }
    }

    fclose(fp);
    if (line)
        free(line);
}

/*
 * Function: parse_request
 * ----------------------------
 *   Parses the http header and returns a Request struct.
 *   raw: http request which will be parsed.
 *   mimes: struct array that has all the mimes data.
 *   mimes_len: the lenght of mimes struct array.
 *
 *   returns: Request
 */

Request *parse_request(const char *raw, MimeType mimes[], size_t mime_len) {
    Request *req = NULL;
    req = malloc(sizeof(Request));
    if (!req) {
		req->http_code = INTERNAL_SERVER_ERROR;
        return req;
    }
    memset(req, 0, sizeof(Request));
    // Identify Method type.
	// Only process GET and HEAD request.
    size_t meth_len = strcspn(raw, " ");
    if (memcmp(raw, "GET", strlen("GET")) == 0) {
        req->method = GET;
    } else if (memcmp(raw, "HEAD", strlen("HEAD")) == 0) {
        req->method = HEAD;
    } else {
        req->method = UNSUPPORTED;
    }

    raw += meth_len + 1; // move past <SP>/" "

	// Check if the url starts with '/'.
	if (raw[0] == '/')
		raw++;

    // Parse Request-URI
    size_t url_len = strcspn(raw, " ");
    size_t url_till_param = strcspn(raw, "?");

	// Check for query parameters. And replace them
    if (url_len > url_till_param)
        req->url = malloc(url_till_param+ 1);
    else
        req->url = malloc(url_len + 1);

    if (!req->url) {
        /* free_request(req); */
		req->http_code = BAD_REQUEST;
        return req;
    }
    
    if (url_len < url_till_param) {
        memcpy(req->url, raw, url_len);
        req->url[url_len] = '\0';
    }else {
        memcpy(req->url, raw, url_till_param);
        req->url[url_till_param] = '\0';
    }

	if (is_valid_url(req->url))
		req->http_code = OK;
	else{
		req->http_code = BAD_REQUEST;
		return req;
	}

	// Check if the resource is available.

    if (strlen(req->url) == 0)
        memcpy(req->url, "index.html\0", 11);

	req->http_code = check_resource(req->url);

    // If resource is available then check for the type.
    // By default the content type is text/plain
    req->type = malloc(128);
    memset(req->type, 0, 128);
    memcpy(req->type, "text/plain\0", 10);
    char *token;
    if (req->http_code == OK && (token = strrchr(req->url, '.')) != NULL) {
        token++;
        for (int i = 0; i < mime_len; i++){
            if (strstr(mimes->extension, token)) {
                /* printf("%s | %s | %s ", token, mimes->header, mimes->extension); */
                memset(req->type, 0, 10);
                memcpy(req->type, mimes->header, strlen(mimes->header));
                break;
            }
            mimes++;
        }
    }

	if (req->http_code != OK)
		return req;	

    raw += url_len + 1; // move past <SP>/" "

    // Parse HTTP-Version
    size_t ver_len = strcspn(raw, "\r\n");
    req->version = malloc(ver_len + 1);
    if (!req->version) {
        /* free_request(req); */
		req->http_code = BAD_REQUEST;
        return req;
    }
    memcpy(req->version, raw, ver_len);
    req->version[ver_len] = '\0';
    raw += ver_len + 2; // move past <CR><LF>
    return req;
}


/*
 * Function: free_mime
 * ----------------------------
 *   Frees the allocated memory for the mime data.
 *   mimes: struct array that has all the mimes data.
 *   mimes_len: the lenght of mimes struct array.
 *
 *   returns: void
 */

void free_mime(MimeType mimes[], size_t mime_len) {
    for (int i = 0; i < mime_len; i++){
        free(mimes->header);
        free(mimes->extension);
        mimes++; 
    }
}

/*
 * Function: free_request
 * ----------------------------
 *   
 *   Request: struct containing the parsed request.
 *
 *   returns: void
 */

void free_request(Request *req) {
	if (!req)
		return;
    free(req->url);
    free(req->version);
    free(req->type);
    free(req);
}
