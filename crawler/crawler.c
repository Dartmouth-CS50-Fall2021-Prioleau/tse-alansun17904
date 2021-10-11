#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"
#include "webpage.h"
#include "hashtable.h"
#include "crawler.h"


int main(int argc, char* argv[]) {
  int depth; 

  // Check argument length.
  if (argc != 4) {
    fprintf(stderr, "Invalid number of arguments given.\n");
    exit(1);
  // Check root url argument valdity and directory writablility.
  } else if (!IsInternalURL(argv[1])) {
    fprintf(stderr, "Root URL is not an internal URL.\n");
    exit(2);
  } else if (!is_dir_writable(argv[2])) {
    fprintf(stderr, "Directory %s given is not writable.\n", argv[2]);
    exit(3);
  // Check max depth valdity must be >= 0.
  } else if ((depth = strtol(argv[3], NULL, 10)) < 0) {
    fprintf(stderr, "Depth given \"%d\" is invalid"
        ", depth must be >= 0.\n", depth);
    exit(4);
  }
  // TODO: strtol returns 0 when error, but 0 is a valid value.
  crawl(argv[1], argv[2], depth);
  return 0;
}

bool export_webpage_to_dir(int id, int depth, char *url, 
  char *html, char *path)
{
  FILE *fp;  
  char *id_s = malloc(2);
  char *fname = malloc(strlen(path) + 2);

  // Converting string id to int
  sprintf(id_s,"/%d", id);
  strcpy(fname, path);
  strcat(fname, id_s);

  if ((fp = fopen(fname, "w")) == NULL)
    return false;
  fprintf(fp, "%s\n", url);
  fprintf(fp, "%d\n", depth);
  fputs(html, fp);
  fclose(fp);
  free(id_s);
  free(fname);
  return true;
}

void crawl(char *root, char *path, int max_depth)
{
  int depth = 0;
  int id = 0;
  int pos = 0;
  char *next_url;
  webpage_t *next = NULL;
  bag_t *bag = bag_new();
  hashtable_t *visited = hashtable_new(10);
 
  // Insert the root into the bag.
  bag_insert(bag, webpage_new(root, 0, NULL));
  hashtable_insert(visited, root, ".");

  while ((next = bag_extract(bag)) != NULL) {
    // Fetch page HTML is successful parse all links.
    if (webpage_fetch(next)) {
       
      // Export the current page before getting links.
      if (!export_webpage_to_dir(++id, webpage_getDepth(next), 
          webpage_getURL(next), webpage_getHTML(next), path)) {
        fprintf(stderr, "The webpage %s could not be exported to a file;"
          "thus, it is being ignored.\n", webpage_getURL(next));
      }
      
      // Add all webpages from links in the current page to bag.
      if (webpage_getDepth(next) < max_depth) {
        while ((next_url = webpage_getNextURL(next, &pos))) {
          if (IsInternalURL(next_url) &&
              hashtable_insert(visited, next_url, ".")) 
            bag_insert(bag, webpage_new(next_url, depth, NULL));
        }
      }
      
    } else {
      fprintf(stderr, "The webpage %s either does"
          "not point to HTML or cannot be opened\n", webpage_getURL(next)); 
    }
    depth++;
  }
  hashtable_delete(visited, NULL);
  bag_delete(bag, webpage_delete);
}
