#ifndef PAGEDIR_H
#define PAGEDIR_H

#include<stdbool.h>

/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Crawler
 * Page Directory Checking (pagedir.h)
 * 
 * This is a header file which contains the headers for the functions to be 
 * implemented in the pagedir.c file in the same directory. The functions in
 * this directory determine if the given path name is a directory, and if the
 * user is allowed to write to this directory. 
 */

/* Checks if the given path points to a directory, if it does and the directory
 * is writable, then a file will be written to the directory `.crawler` and the
 * function will return true. If an invalid argument is given or the directory
 * is not writable or not a directory, then false is returned.
 */
bool is_dir_writable(char *path);


#endif
