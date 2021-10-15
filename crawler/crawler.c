#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"
#include "webpage.h"
#include "memory.h"

/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Crawler
 * Tiny Search Engine Crawler (crawler.c)
 * 
 * This program represents the crawler component of the tiny search engine.
 * The program takes exactly three arguments: the first denoting the root URL
 * of the webpage being crawled, the second denoting the directory where the
 * crawled pages will be stored, the last argument represents the depth of the
 * crawl. We note that a depth of 0 will only result in a crawl of the given
 * page. The program will crawl every page that is less than or equal to the
 * given depth. It will store each page in a separate text file in the given
 * directory. These files will be given a unique `id` - an integer that counts
 * up from 1. Within each file, the first line of the file will display the url
 * of the page. The second line of the exported file will show the depth of the
 * page with respect to the root, and finally, the html of the page will
 * follow. The user should also note that the program will only work on
 * internal URLs, ones that are in the cs50 domain. This is done to prevent
 * overloading traffic on unsuspecting webpages. Moreover, it also makes the
 * behavior of the program more well-defined. Thus, the program, at its
 * inception, will check whether or not the URL given is an internal one.
 * 
 * Please note that this program depends on the `libcs50` library which
 * includes an API for handling web scraping as well as an API for a `bag` and
 * `hashtable` data structure which are crucial to the functionality of this 
 * program.
 * 
 * If the program is given an invalid number of arguments, the program will
 * exit with a non-zero exit status. If the program is given a directory that
 * is not writable, then it will also exit with non-zero exit status. If a
 * URL is given that is not internal, the program will also exit with non-zero
 * exit status. We note here that the program does not check whether the third
 * argument is valid; thus, if an input like "20a" is given for the third 
 * argument, the program will run with a max depth of 20. Finally, if at any
 * point the program has trouble allocating memory it will terminate
 * immediately. This may result in a memory leak.
 */

static bool export_webpage_to_dir(int id, int depth, char *url, 
  char *html, char *path);

static void crawl(char *root, char *path, int max_depth);

/* (description): Please see the header of the program.
 * 
 * (inputs): 
 * 
 *        $ ./crawler <ROOT_URL> <DIRECTORY> <MAX_DEPTH>
 * 
 * As explained in the description of the program, ROOT_URL must be an internal
 * URL. Moreover, the directory given must exist and be writable. Lastly, the
 * given max depth must also be a nonnegative integer.
 *
 * (outputs): The program will crawl all the files of given MAX_DEPTH away from
 * the root page. It will then write all of these files into the given
 * directory. These files will be titled with their ID (a positive integer).
 */
int main(int argc, char* argv[]) {
  int depth; 
  char *root;

  // Check argument length. There must be exactly 3 arguments.
  if (argc != 4) {
    fprintf(stderr, "Invalid number of arguments given.\n");
    exit(1);
  // Check if the given URL is an internal one.
  } else if (!IsInternalURL(argv[1])) {
    fprintf(stderr, "Root URL is not an internal URL.\n");
    exit(2);
  // Check if the given directory is writable.
  } else if (!is_dir_writable(argv[2])) {
    fprintf(stderr, "Directory %s given is not writable.\n", argv[2]);
    exit(3);
  // Check max depth valdity must be >= 0.
  } else if ((depth = strtol(argv[3], NULL, 10)) < 0) {
    fprintf(stderr, "Depth given \"%d\" is invalid"
        ", depth must be >= 0.\n", depth);
    exit(4);
  }
  /* We hand-off to the crawler defined below. This implements all of the main 
   * functionality, assuming that the arguments given are all valid.
   */
  /* Malloc memory for root so that it can be collectively freed using
   * webpage_delete.
   */
  root = malloc(strlen(argv[1]) + 1);
  strcpy(root, argv[1]);
  crawl(root, argv[2], depth);
  return 0;
}

/* (description): The `export_webpage_to_dir` function will export a given
 * webpage and its contents to a file which is named by a unique ID. If the
 * file with the ID does not exist yet, it will be created and if the file
 * with the same ID already exists, then it will be overwritten. The file
 * will conform to the following format:
 *
 *  <URL to HTML>
 *  <DEPTH>
 *  <All HTML associated with the website>
 * 
 * with the first line showing the URL where the website HTML was taken from.
 * The second line denotes the depth this website is relative to the crawled
 * root. The depth is given as an input to the function. Lastly, the function
 * will append all of the HTML associated with the file.

 * (usage): The function assumes that all of the arguments are "nice" and
 * valid. This makes the function less robust and less useful outside of the
 * use case of this module. Therefore, it is declared static and is used purely
 * as a helper function.
 *
 * (inputs): (int) `id` represents the unique integer ID which the exported
 * file will be named. (int) `depth` represents the depth of the website
 * relative to the root. This argument is exported as the second line of the 
 * file. (char *) `url` represents the URL from which this HTML was taken from.
 * (char *) `html` is the actual HTML that will be appended to the end of the 
 * exported file. Lastly, (char *) `path` is the path where the file is
 * exported. This path is assumed to be valid and writable.
 * 
 * (outputs): Writes a file in `path` that has the name `id`. The format of
 * this file is explained in the description.
 *
 * (error handling): If for some reason memory allocation has failed, then the
 * function will simply terminate. Note that at this point, the function will
 * not free any additional used memory, and this may result in memory leaks.
 */
static bool export_webpage_to_dir(int id, int depth, char *url, 
  char *html, char *path)
{
  FILE *fp;  
  char *id_s = malloc(5); // "/" + 3 digits + null character
  char *fname = malloc(strlen(path) + 6);

  // Check for memory allocation failure.
  assertp(id_s, "`export_webpage_to_dir` could not allocate memory for" \
                  "for a new filename.");
  assertp(fname, "`export_webpage_to_dir` could not allocate memory for" \
                  "for a new filename.");

  // Converting string id to int.
  sprintf(id_s,"/%d", id);
  /* Append the name of the file to the given path. We allocate memory for a
   * string id of three digits, this means that the maximum id that we will be
   * able to store is 999. Thus, we are assuming that the user will not 
   * crawl more than 999 pages at a time.
   */
  strcpy(fname, path);
  strcat(fname, id_s);


  // Check if the file with the name is writable.
  if ((fp = fopen(fname, "w")) == NULL)
    return false;

  // Export URL, depth and HTML to the file.
  fprintf(fp, "%s\n", url);
  fprintf(fp, "%d\n", depth);
  fputs(html, fp);
  fclose(fp);
  free(id_s);
  free(fname);
  return true;
}

/* (description): The `crawl` function crawls a given webpage to the given 
 * depth. It then stores all of the crawled webpages and their contents
 * into the given `path`. The function performs a search starting from the
 * root. The function keeps track of the depth of the current node. If the
 * current depth is equal to the max_depth, then the crawl will end. 
 * Otherwise, the function will add all webpages linked to the current web-
 * page to a bag. The function will then extract a node from the bag, 
 * check if the link has already been visited, and if it has been visited then
 * we continue; else, we continue with the same algorithm.
 *
 * (usage): The function, like the `export_webpage_to_dir` function assumes 
 * that all the arguments are "nice" and valid. Therefore, this function would
 * not be too useful outside of this program. Therefore, it is declared static
 * and used purely as a helper function.
 *
 * (inputs): (char *) `root` represents the URL of the root node. The function
 * will start the crawl at this URL. The (char *) `path` parameter is the path
 * where all of the exported HTML will end up. Lastly, the `max_depth` arg is
 * the maximum depth of the crawl.
 * 
 * (outputs): The function will export the files associated with the webpages
 * being crawled. The format of these files are explained in the 
 * `export_webpage_to_dir`.
 *
 * (error handling): If for some reason memory allocation has failed, then the
 * function will terminate. Note that at this point, the function will not free
 * any additional used memory, and this may result in memory leaks.
 */
static void crawl(char *root, char *path, int max_depth)
{
  int id = 0;
  int pos = 0;
  char *next_url;
  webpage_t *next = NULL;
  bag_t *bag = bag_new();
  hashtable_t *visited = hashtable_new(10);
 
  // Insert the root into the bag.
  bag_insert(bag, webpage_new(root, 0, NULL));
  hashtable_insert(visited, root, ".");

  // If there are still webpages to be processed.
  while ((next = bag_extract(bag)) != NULL) {
    // Fetch page HTML is successful parse all links.
    if (webpage_fetch(next)) {
       
      // Export the current page before getting links.
      if (!export_webpage_to_dir(++id, webpage_getDepth(next), 
          webpage_getURL(next), webpage_getHTML(next), path)) {
        fprintf(stderr, "The webpage %s could not be exported to a file;"
          "thus, it is being ignored.\n", webpage_getURL(next));
      }
      
      /* Only if the depth of the current node is less than the
       * maximum depth, do we add its neighbors to the bag.
       */
      if (webpage_getDepth(next) < max_depth) {
        while ((next_url = webpage_getNextURL(next, &pos))) {
          /* Here, we only proccess the next link if the neighboring URL
           * is an internal one, and if we have already visited the node.
           */
          if (IsInternalURL(next_url) &&
              hashtable_insert(visited, next_url, ".")) 
            bag_insert(bag, webpage_new(next_url, 
                  webpage_getDepth(next) + 1, NULL));
        }
      }
      
    } else {
      fprintf(stderr, "The webpage %s either does"
          "not point to HTML or cannot be opened\n", webpage_getURL(next)); 
    }
    webpage_delete(next);
  }
  hashtable_delete(visited, NULL);
  bag_delete(bag, webpage_delete);
}
