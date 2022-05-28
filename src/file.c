#include <file.h>

#define BUF_SIZE 65536 //2^16

/*
 * Function: check_if_file_exists 
 * ----------------------------
 *   Check if a file exists or not. And also check
 *   if the file is readable. If exists than return
 *   true else false.
 *
 *   file_name: the file name which user will pass 
 *
 *   returns: bool
 */

bool check_if_file_exists(const char *file_name){
    if (access(file_name, F_OK | R_OK) == 0)
        return true;
    else
        return false; 
}


/*
 * Function: check_if_file_exists 
 * ----------------------------
 *   Check if a file exists or not. And also check
 *   if the file is readable. If exists than return
 *   true else false.
 *
 *   fd_read: the fd that will be read.
 *   fd_write: the fd that will get write.
 *   buff_size: at a singel read how many 
 *              char will be writen.
 *
 *   returns: void
 */

void read_and_write_fd(int fd_read, int fd_write, size_t buff_size) {

    char buff[1];
    
    while ((read(fd_read, buff, buff_size)))
        write(fd_write, buff, buff_size);

}

/*
 * Function: read_header_fd 
 * ----------------------------
 *   Read the header and store it into a string.
 *
 *   fd_read: the fd that will be read.
 *
 *   returns: char *
 */

char *read_header_fd(int fd_read) {

    char *req = malloc(4096);
    char buff[1];
    int counter = 0;
    bool cr = false;
    memset(req, 0, 4096);


    while ((read(fd_read, buff, 1)) && counter <= 4095) {

        req[counter] = buff[0];

        // look for \r\n to stop reading.
        // because we are implementing a basic web
        // server we dont need any other header.
        if (buff[0] == '\n' && cr)
            break;
        if (buff[0] == '\r')
            cr = true;

        counter++;
    }

    return req;
}
