/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Indexer
 * Module for an Indexer Data Structure (index.c)
 *
 */

#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include "index.h"
#include "hashtable.h"
#include "counters.h"

index_t *index_new(void)
{
  index_t *index = malloc(sizeof(index_t));

  // Create a hashtable of where the keys are words and the values are a
  // set of counters, where each counter's key is the document id and the
  // count is the instances of that word in the document.
  index->table = hashtable_new(1000);
  return index;
}

static void print_counts(void *fp, const int key, const int count)
{
  if (fp == NULL) 
    return;
  fprintf(fp, "%d %d", key, count);
}

static void print_words(void *fp, const char *key, void *item)
{
  if (fp == NULL)
    return;
  fprintf(fp, "%s: ", key);
  counters_iterate(item, fp, print_counts);
  fprintf(fp, "\n");
}

void c_delete_helper(void *counter)
{
  counters_delete(counter);
}

void index_delete(index_t *index)
{
  if (index == NULL)
    return;
  hashtable_delete(index->table, c_delete_helper);
  free(index);
}

/* (description): The `index_save` function saves the contents of an index 
 * struct into a given file pointer. The function will export the contents 
 * of the given struct in the following format:
 *
 * <word1>: <doc_id1> <count1> <doc_id2> <count2>
 * <word2>: <doc_id1> <count1> <doc_id2> <count2>
 *
 * Where the `<word1>` represents the word that is being counted across all
 * the documents and the `<doc_id>` represents the id of the document where
 * the target word is being tallied, and `<count>` represents the count of
 * the target word in the document.
 * We note that since the hashtable is inherently unordered, the order in 
 * which these lines are printed are also not guaranteed.
 *
 * (inputs): The function takes in two inputs: a file pointer where the
 * contents of the index struct will be outputted, the second argument is a
 * pointer to the index struct that is being outputted.
 *
 * (outputs): The function will return `true` if the function was successful
 * in outputting the contents of the given index. Otherwise if there are any
 * errors, then the function will return false.
 *
 * (error handling): If either argument is false, then the function will return
 * false. We also note that in the case. In this case, the function will also
 * do nothing.
 */
bool index_save(FILE *fp, index_t *index)
{
  if (fp == NULL || index == NULL)
    return false;
  hashtable_iterate(index->table, fp, print_words);
  return true;
}

bool index_add(index_t *index, char *word, int document_id) {
  counters_t *count;

  // Check argument validity.
  if (index == NULL || word == NULL || document_id < 1) {
    return false;
  /* The word did not previously exist, then we need to create a new set
   * of counters for this word.
   */
  } else if (hashtable_insert(index->table, word, counters_new())) {
    count = hashtable_find(index->table, word); 
    
    // Increment the counter that was just added to the set.
    counters_add(count, document_id);
    return true;
  } else {
    count = hashtable_find(index->table, word);
    counters_add(count, document_id);
    return true;
  }
}
