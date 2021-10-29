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

