#include<stdio.h>
#include<string.h>
#include "file.h"
#include "index.h"
#include "memory.h"

/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Indexer
 * Index Test (indextest.c)
 *
 * This program reads in an index file loads it to an indexer data structure
 * then outputs the contents of the given data structure to the given output
 * file. The program can be run using the following command:
 *
 * ./indextest <indexFILE> <outputFILE> *
 * where <indexFILE> is the path to the indexer file being read, and 
 * <outputFILE> is the path to the output file where the contents of the 
 * constructed indexer will be placed.
 */

index_t *index_load(FILE *fp);

/* (description): The program will read from an index file, store its contents
 * into a newly created index struct, and then re-save the contents of this 
 * index struct to the new file that is given. 
 * * (inputs): The program takes two arguments, the first is the path to the path
 * to the index file, while the second is the path to the desired new index
 * file. If either of these arguments are invalid, meaning that the index file
 * does not exist, or that the new file cannot be created, then the program 
 * will exit with a non-zero exit status. If the index file exists, for the
 * sake of simplicity, the program will assume that this file conforms to the
 * format of an index file that was outputted by the indexer.
 *
 * (outputs): The program will write to the given output file the contents of
 * the newly created index data structure, or it will created a new file with 
 * this name if the given file does not already exist.
 *
 * (error handling): If at any point in execution the program cannot allocate 
 * memory it will terminate at that point, without freeing any existing memory.
 * This will most likely result in memory leaks.
 *
 */
int main(int argc, char *argv[])
{
  FILE *fpr, *fpw;
  index_t *index;

  // Check if the number of arguments is valid.
  if (argc != 3) {
    fprintf(stderr, "Invalid number of arguments.\n");
    exit(1);
  }

  // Check if the input index file can be read.
  fpr = fopen(argv[1], "r");
  if (fpr == NULL) {
    fprintf(stderr, "Indexer file coult not be read.\n");
    exit(2);
  }

  // Check if the output file can be written to.
  fpw = fopen(argv[2], "w");
  if (fpw == NULL) {
    fprintf(stderr, "Output file could not be read.\n");
    exit(2);
  }

  index = index_load(fpr);
  index_save(fpw, index);

  fclose(fpw);
  fclose(fpr);
  index_delete(index);
  return 0;
}

/* (description): The `read_index_file` will read the contents of a file 
 * produced by the indexer, and load this into an indexer data structure.
 * This function assumes that the file if it exists is actually created by
 * the indexer. This implies that the formatting of this file must conform
 * to the format described in the `index.c` file.
 *
 * (inputs): A file pointer to the file that will be read.
 *
 * (outputs): The function will return an index data structure that contains
 * all of the information from the file that has just been read in.
 *
 * (error handling): If memory allocation fails at any point the function will
 * cause the program to terminate. This may result in memory leaks. Thus, the 
 * user needs to be weary of this.
 *
 */
index_t *index_load(FILE *fp)
{
  index_t *index;
  char *word, *line;
  int id, count, buff, num_read;
  int line_count;

  // Create a line and word buffer. 
  line = NULL;
  word = malloc(256);

  assertp(word, "Word buffer could not be allocated.\n");

  if (fp == NULL) {
    free(word);
    assertp(fp, "The file point is NULL.");
  }

  index = index_new();
  /* Each line is characterized by a word following by pairs of integers. We
   * loop through each line and read the word first,  then
   * we continue with sscanf for the remaining digits.
   */
  line_count = lines_in_file(fp);
  for (int i = 0; i < line_count; i++) {
    // Read the word that defines the line.
    line = freadlinep(fp);
    sscanf(line, "%s%n", word, &buff);

    while (buff < strlen(line)) {
      /* We keep track of where we are relative to the start of the line, by 
       * remembering how many characters we just read in. 
       */
      sscanf(line + buff, "%d %d%n", &id, &count, &num_read);
      buff += num_read;
      // Set the count for the word and associated doc ID.
      index_set(index, word, id, count);
    }
    free(line);
  }
  free(word);
  return index;
}


