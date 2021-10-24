#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "pagedir.h"
#include "memory.h"

/* Alan Sun
 * CS50, Fall 2021, Tiny Search Enginer: Crawler
 * Check Directory Writable (pagedir.c)
 *
 * The function in this file checks if a given directory is indeed a directory
 * and if this directory is writable. The details of this function is given
 * in its function header as well as the header file. The other function in
 * this directory checks to see if a given directory is created by the crawler.
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

/* (description): The `is_crawler_dir` function checks if a given directory is
 * created by the crawler. If the directory has been created by the crawler 
 * then `true` is returned. Otherwise, `false` is returned. Since the crawler
 * will add a `.crawler` hidden file in each directory that it 
 * creates/traverses, this function simply checks for this existence of this
 * file in the given directory. 
 *
 * (inputs): The function takes in a string that represents the path of the 
 * directory that is being checked.
 *
 * (outputs): The function will return true if the directory has been created
 * by the crawler, and false if the directory has not been created by the
 * crawler. 
 *
 * (error handling): If the given path is NULL or if the path is invalid or
 * non-existent, then false is also returned.
 */
bool is_crawler_dir(char *path) 
{
  FILE *fp; 
  char *crawler_path = malloc(strlen(path) + strlen("/.crawler") + 1);

  assertp(crawler_path, "Memory could not be allocated for the path" \
                  " of the crawler.");
  // Checking to see if we can read the <path>/.crawler file. If we can
  // it likely means that the path is made the crawler. 
  strcpy(crawler_path, path);
  strcat(crawler_path, "/.crawler");

  // Check null-pointer argument.
  if (path == NULL) {
    return false;
  // Check if the /.crawler file is existent.
  } else if ((fp = fopen(crawler_path, "r")) == NULL) {
    free(crawler_path);
    return false;
  // The crawler file exists and is readable.
  } else {
    free(crawler_path);
    fclose(fp);
    return true;
  }
}

