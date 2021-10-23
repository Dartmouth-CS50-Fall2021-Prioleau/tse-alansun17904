#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "word.h"
#include "pagedir.h"
#include "file.h"
#include "webpage.h"
#include "index.h"

/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Indexer
 * Indexer (indexer.c)
 *
 */

static void parse_args(int argc, char *argv[]);
static void read_crawled_file(FILE *fp, int id, index_t *indexer);
static index_t *read_directory(char *path);

int main(int argc, char *argv[])
{
  index_t *indexer;
  FILE *fp;

  parse_args(argc, argv); 
  indexer = read_directory(argv[1]);
  fp = fopen(argv[2], "w");
  index_save(fp, indexer);
  fclose(fp);
  index_delete(indexer);
  return 0;
}

static void parse_args(int argc, char *argv[])
{
  FILE *fp; 

  if (argc != 3) {
    fprintf(stderr, "Invalid number of arguments provided.\n");
    exit(1);
  } else if (!is_crawler_dir(argv[1])) {
    fprintf(stderr, "The given directory was not created by the crawler.\n");
    exit(2);
  // Check if the given file is writable.
  } else if ((fp = fopen(argv[2], "w")) == NULL) {
    fprintf(stderr, "The given file for exporting the" \
                    "index is not writable.\n");
    exit(3);
  } else {
    fclose(fp);
  }
}

static index_t *read_directory(char *path)
{
  FILE *fp;
  int id = 1;
  char *id_s = malloc(5); // "/" + 3 digits + null character
  char *fname = malloc(strlen(path) + 6);
  index_t *indexer = index_new();

  // We continue to get a new page until all the next page cannot be read.
  // Read the first page: convert string id to int.
  sprintf(id_s, "/%d", id);
  strcpy(fname, path);
  strcat(fname, id_s);

  while ((fp = fopen(fname, "r")) != NULL) {
    read_crawled_file(fp, id++, indexer);

    // We read the next file.
    sprintf(id_s, "/%d", id);
    strcpy(fname, path);
    strcat(fname, id_s);
  }

  free(id_s);
  free(fname);
  
  return indexer;
}

static void read_crawled_file(FILE *fp, int id, index_t *indexer)
{
  char *url;
  char *html;
  char *result;
  int pos = 0;
  webpage_t *page;


  if (fp == NULL || indexer == NULL)
    return;
  
  /* We assume that the given file was created by the crawler so it also
   * conforms to the format of the crawler. We read everything except for
   * the second line (read and then free) because it is irrelevant.
   */
  url = freadlinep(fp);
  free(freadlinep(fp));
  html = freadfilep(fp);
  page = webpage_new(url, 0, html); 
  
  // Add all words from the webpage in the index struct.
  while ((result = webpage_getNextWord(page, &pos)) != NULL) {
    // Check if the strlen >= 3, if not we do not consider it.
    if (strlen(result) >= 3)
      index_add(indexer, NormalizeWord(result), id);
    free(result);
  }
  webpage_delete(page);
  fclose(fp);
}


