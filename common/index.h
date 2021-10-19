/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Indexer
 * Module for an Index Datastructure (index.h)
 *
 * This header file is for the module that implements many features of an
 * indexer datastructure. We note that an indexer is implemented as a
 * hashtable. Where the keys are words and the values are sets of counters.
 * Each counter represents the count of the word in a document, which are
 * uniquely identified by a document ID given by the crawler. This module
 * abstracts some of the behaviors of the indexer away for the user. Please 
 * see the `index.c` file for the details of the implementation and the
 * functionalities of the functions defined below.
 */ 

#ifndef INDEX_H
#define INDEX_H

#include<hashtable.h>
#include<stdbool.h>

typedef struct index {
  hashtable_t* table;
} index_t;

index_t *index_new(void);

void index_delete(index_t *index);

bool index_save(FILE *fp, index_t *index);

bool index_add(index_t *index, char *word, int document_id);

#endif
