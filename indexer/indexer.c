#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "word.h"
#include "pagedir.h"
#include "file.h"
#include "webpage.h"
#include "index.h"
#include "memory.h"

/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Indexer
 * Indexer (indexer.c)
 * 
 * The indexer program is a program that reads from files produced by the 
 * crawler. It then creates an index from these files, cataloging the words
 * in the file and exporting this "catalog" to a single, new file. The program
 * only acts on files that are created by the crawler. This is because there
 * is an assumed format in which crawler files are naturally produced. To
 * run the program, one can call the following from the command line:
 * 
 * ./indexer <directory> <new file name>
 *
 * The first argument is the path to the directory created by the crawler. 
 * The second argument is the file name of the new file that will be
 * created by the indexer. Details about the inputs/outputs/error handling
 * will be explaining in the header of the main function. Briefly though, 
 * the indexer will collate all of the files created by the crawler, and output
 * them into a new file. This file will aggregate a count of all of the words
 * that appear in each crawled file. The format of this file is as follows:
 *
 * <word1> <ID1> <count1> <ID2> <count2> <ID3> <count3>
 * <word2> <ID1> <count4> <ID2> <count5> <ID3> <count5>
 *
 * Where <wordi> is a word, <IDi> is the ID of the document where this word was
 * found, and <counti> is the count of the word in this document.
 *
 */

static void parse_args(int argc, char *argv[]);
static void read_crawled_file(FILE *fp, int id, index_t *indexer);
static index_t *read_directory(char *path);


/* (description): This is the main function that has all of the functionality 
 * that implements the description above.
 *
 * (inputs): The indexer takes two arguments. The first is a path to the
 * directory created by the crawler. The main function will specifically check
 * for this attribute, because it is assumed that all crawler created
 * directories will have a `.crawler` file in them. If the given directory
 * is not a directory created by the crawler, then the program will terminate
 * with a non-zero exit status. The second argument is the file name to where 
 * the results of the indexer will be exported. 
 *
 * (outputs): A file with the contents of the indexer data structure. The 
 * format of this file is described in the brief description of the program.
 *
 * (error handlinng): If any of the arguments are invalid, then a nonzero 
 * status will be returned. If at any point in the program memory allocation
 * fails, then the program will terminate immediately. This will happen
 * without freeing any existing allocations, thus, the user needs to careful
 * as this will cause unwanted memory leaks.
 *
 */
int main(int argc, char *argv[])
{
  index_t *indexer;
  FILE *fp;

  parse_args(argc, argv); 
  
  // Read contents of crawler directory into new index data structure.
  indexer = read_directory(argv[1]);
  fp = fopen(argv[2], "w");
  // Export the indexer data structure to the written file.
  index_save(fp, indexer);
  fclose(fp);
  index_delete(indexer);
  return 0;
}

/* (description): The `parse_args` function parses the arguments to the main
 * function. If any of the arguments are invalid, then the program will
 * terminate with a nonzero exit status.
 *
 * (inputs): The number of arguments that was passed into the main function
 * as well as the value of these arguments.
 *
 * (outputs): If all of the arguments are valid, then execution will leave this
 * function and return nothing.
 *
 * (error handling):
 *
 */
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

/* (description): This is a helper function that will loop through all the 
 * files in a crawlers directory and read each file by calling the other
 * utility function `read_crawled_file`. Since the directory given is created
 * by the crawler we can assume a few things:
 *
 *  - All the files in this directory are enumerated from 1...
 *  - All the files in this directory are written in a similar format:
 *    the url of the file, the depth of the url, and the html of the url.
 *  - The HTML in all of the files is valid html.
 *
 * We use these attributes to our advantage by looping through the directory
 * with a counter=1, then continuously increasing the counter until a file
 * with path <path>/<counter> cannot be found anymore. This means that we have
 * read all of the files in the given directory. We also assume that there 
 * can be no more than 999 files in a given directory.
 *
 * (inputs): The path to the crawler's directory.
 *
 * (outputs): The function will create a new index data structure and place
 * the results of its indexing into this data structure. This can then be
 * used for many purposes. 
 *
 * (error handling): Since this function is only called locally in this module
 * we assume that the path can never be NULL. Moreover, since the path has
 * already been checked by the `parse_args` function we also assume that the
 * function is given a path that points to a valid crawler directory.
 */
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

  // Clean up
  free(id_s);
  free(fname);
  
  return indexer;
}

/* (description): The `read_crawled_file` will read the contents of a given
 * crawler file and then output this content in a structure way to the given
 * indexer. It does this by discarding the first two lines of the file as these
 * are not useful to parsing the contents of the given pages. Since this 
 * function is only useful here, the function is made static and not exposed
 * to the user.
 *
 * (inputs): The first argument is the file which the function will process.
 * This argument is assuemd to point to a file that is actually created by
 * the crawler, and thus conforms to the format of the crawler. The second
 * argumetn is the ID if the given page being processed. This is important
 * for the indexer, because the indexer keeps aggregate count of all of the
 * words across multiple files. The last argument is the pointer to the indexer
 * where the processed information was stored.
 *
 * (outputs): The function will return nothing, however it will modify the
 * given indexer.
 *
 * (error handling): If memory allocation fails at any point the program will
 * simply terminate. This may result in memory leaks.
 *
 */
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


