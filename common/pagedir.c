#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "pagedir.h"

/* Alan Sun
 * CS50, Fall 2021, Tiny Search Enginer: Crawler
 * Check Directory Writable (pagedir.c)
 *
 * The function in this file checks if a given directory is indeed a directory
 * and if this directory is writable. The details of this function is given
 * in its function header as well as the header file.
 */

/* The `is_dir_writable` function checks if a directory is writable. If it is
 * then true is return. Otherwise, false is returned. The function tests for
 * this property by creating a ".crawler" file in the directory; if this is
 * successful, then true is returned. The function takes in one argument:
 * the path to the directory being checked. 
 */
bool is_dir_writable(char *path)
{
  char *new_name = malloc(strlen(path) + strlen("/.crawler") + 1);
  FILE *file;

  // Check valid path argument.
  if (path == NULL)
    return false;

  // Create .crawler file.
  strcpy(new_name, path);
  strcat(new_name, "/.crawler");

  // Check if creation is successful.
  if ((file = fopen(new_name, "w")) == NULL) {
    free(new_name);
    return false;
  } else {
    fclose(file);
    free(new_name);
    return true;
  }
}
