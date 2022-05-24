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

bool check_if_file_exists(char *file_name){
    if (access(file_name, F_OK | R_OK) == 0)
        return true;
    else
        return false; 
}
