#ifndef CRAWLER_H
#define CRAWLER_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Crawler
 * Crawler Header (crawler.h)
 *
 * Utility functions for implementing the crawler program. Please see
 * the function headers for a more detailed description of the functions.
 */


/* The `export_webpage_to_dir` function exports the contents of an html
 * page to a file in the directory `path`; The exported file is named with
 * the given argument `id`. The first line of the exported file shows the URL
 * where this page was taken from; the second line shows the depth of this page
 * relative to the root. This is also given to the function as `depth`. Lastly,
 * the html of the webpage follows. If there is an error creating the page then
 * false is returned. Otherwise, on success, true is returned.
 */
bool export_webpage_to_dir(int id, int depth, char *url, 
                           char *html, char *path);

void crawl(char *root, char *path, int max_depth);

#endif
