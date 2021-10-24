/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Indexer
 * Module for an Indexer Data Structure (index.c)
 *
 * This module implements a indexer datastructure for the indexer. The general
 * idea is that this data structure tabulates the number of times a given 
 * word appears in a document. It then does this for all the documents in a 
 * given crawler directory. This structure is implemented as a hashtable whose
 * keys are the words being tabulated and the values are counters which have
 * the document IDs as their key and the count of the word in each document
 * as their value. A diagram of this data structure is shown below:
 *
 *
 * |-----|   |====|    |------------|    |------------|    |------------| 
 * |word1| : |head| -> |id1 : count1| -> |id2 : count2| -> |id3 : count3|
 * |-----|   |====|    |------------|    |------------|    |------------|
 *    |
 *    v
 * |-----|   |====|    |------------|    |------------|    |------------| 
 * |word2| : |head| -> |id1 : count4| -> |id2 : count5| -> |id3 : count6|
 * |-----|   |====|    |------------|    |------------|    |------------|
 *    |
 *    v
 * |-----|   |====|    |------------|    |------------|    |------------| 
 * |word3| : |head| -> |id1 : count7| -> |id2 : count8| -> |id3 : count9|
 * |-----|   |====|    |------------|    |------------|    |------------|
 *
 * This module also provides some utility functions that go along with this
 * datastructure. Each of these are described in detail in their function 
 * headers. 
 *
 * Please note that unit tests are included at the end of the source code.
 *
 */

#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include "hashtable.h"
#include "counters.h"
#include "index.h"
#include "memory.h"

/* (description): The `index_new` function creates a new index data structure.
 * It will allocate memory for this structure as well as its underlying 
 * hashtable. 
 * 
 * (inputs):
 *
 * (outputs): The function will return a pointr to this newly allocated data
 * structure. The user is responsible for freeing this memory by calling
 * `index_delete` on the returned pointer when they are done using it, or 
 * else there will be memory leaks.
 *
 * (error handling): If memory allocation fails, the function will cause
 * the program calling the function to terminate. The user needs to be 
 * careful because this will most likely result in memory leaks of unfreed
 * blocks. 
 */
index_t *index_new(void)
{
  index_t *index = malloc(sizeof(index_t));
  
  // Check memory allocation failure.
  assertp(index, "Memory for index could not be allocated.\n");

  /* Create a hashtable of where the keys are words and the values are a
   * set of counters, where each counter's key is the document id and the
   * count is the instances of that word in the document.
   */
  index->table = hashtable_new(3);
  // Check memory allocation failure.
  assertp(index->table, "Memory allocation for hashtable in index created " \
                        "has failed.\n");
  return index;
}

/* (description): A helper function that prints the contents of a counter. 
 * Since this function is only useful in the context of this module it is
 * made static and not exposed to the user.
 *
 * (inputs): It follows the format required by the counters print in the
 * counters module. The first argument is the file pointer where the printing
 * will be outputted, the second and third argument represent the key/count
 * that is being printed. 
 *
 * (outputs): Outputs to the given file pointer the printer format: 
 * the key followed by a space followed by the count. 
 *
 * (error handling): If given file pointer is NULL, then the function will do
 * nothing.
 */
static void print_counts(void *fp, const int key, const int count)
{
  if (fp == NULL) 
    return;
  fprintf(fp, " %d %d", key, count);
}

/* (description): A helper function that writes out the counts associated 
 * with each word. Since this function is only useful in the context of this
 * module it is made static and not exposed to the user.
 * 
 * (inputs): The function takes in three arguments. These arguments conform to
 * the function header required for hashtable iterate. The first argument is 
 * the pointer to the file where the results will be exported; the second 
 * argument represents the word that is being printed, and the last argument
 * is the counter that is associated with this word. This will be written
 * to the file using the `print_counts` helper function and the counters
 * iteratre.
 *
 * (outputs): The function will write to the given file pointer the counts
 * of every word with the associated document IDs.
 *
 * (error handling): If any of arguments are NULL, the function will simply
 * do nothing.
 */
static void print_words(void *fp, const char *key, void *item)
{
  if (fp == NULL || key == NULL || item == NULL)
    return;
  fprintf(fp, "%s", key);
  counters_iterate(item, fp, print_counts);
  fprintf(fp, "\n");
}

/* (description): A helper function that deletes a counter. This function
 * must exist because if we just past the counters_delete function into
 * the hashtable_delete function there will be incompatiable types. Thus,
 * we need this wrapper. Please see the counters.c module for more information
 * about how the counter is being deleted.
 */
static void c_delete_helper(void *counter)
{
  counters_delete(counter);
}

/* (description): The index_delete function deletes an index data structure. 
 * It does this by first deleteing the counters in the hashtable. It does this
 * by calling the hashtable_delete function and then passing in the 
 * c_delete_helper function. Then it deletes the hashtable and its keys.
 * Lastly, it will delete the memory associated with the pointer associated
 * with the index. 
 * 
 * (inputs): A pointer to the index that is being deleted.
 *
 * (error handling): If the given pointer is NULL, then nothing will happen
 * and the program will exit.
 */
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

/* (description): The `index_add` function increments the count of a word
 * associated with a given document. It does this by either appending
 * a new counter to the set for the given word or increment the counter
 * of an existing counter.
 * 
 * (inputs): The function takes in three arguments: a pointer to an index
 * data structure, a word, and an integer document ID. All of these arguments
 * must be non-NULL. We note that the index will make a copy of the given word,
 * so the user is able to freely modify the given word and this will not change
 * the values stored in the data structure.
 *
 * (outputs): The function will output true if the counter has been
 * successfully incremented, and false if the increment did not succeed. 
 *
 * (error handling): If the pointer to index or the word are NULL pointers
 * then the function will return false. The document IDs are assumed to be 
 * > 1, but if the given ID is less than 1, then false is also returned.
 */
bool index_add(index_t *index, char *word, int document_id)
{
  counters_t *count;

  // Check argument validity.
  if (index == NULL || word == NULL || document_id < 1) {
    return false;
  /* The word did not previously exist, then we need to create a new set
   * of counters for this word.
   */
  } else if (hashtable_find(index->table, word) == NULL) {
    count = counters_new();
    hashtable_insert(index->table, word, count);
    
    // Increment the counter that was just added to the set.
    counters_add(count, document_id);
    return true;
  } else {
    // The word already exists in the hashtable, so we just add to the counter.
    count = hashtable_find(index->table, word);
    counters_add(count, document_id);
    return true;
  }
}

/* (description): The `index_set` function sets the counter for a document ID
 * of a given word. This is useful for re-initializing the index file from 
 * the saved index. The function will modify the index being passed in.
 *
 * (inputs): The first argument represents the point to the index data 
 * structure that is being set. We note that this argument must be non-NULL
 * The second argument represents the target word whose document ID is being 
 * set. This argument must point to a non-NULL word. If the word does not 
 * exist yet in the index data structure, then it will be added. 
 * The third argument is the document ID whose count is being set, and the
 * last argument is the count. Both of these arguments must be greater than 0.
 *
 * (outputs): The function will return `true` if the index data struct was 
 * successfully modified. Otherwise, the function will return `false`. 
 *
 * (error handling): If at any point memory allocation fails, the program will
 * terminate. This may result in memory leaks.
 *
 */
bool index_set(index_t *index, char *word, int document_id, int count)
{
  counters_t *ct;

  if (index == NULL || word == NULL || document_id < 1 || count < 1)
    return false;
  
  // If the word does not exist we add a new counter with the count.
  if ((ct = hashtable_find(index->table, word)) == NULL) {
    ct = counters_new();
    hashtable_insert(index->table, word, ct);
    counters_set(ct, document_id, count);
  } else {
    counters_set(ct, document_id, count);
  }
  return true;
}


#ifdef UNIT_TEST
/* Unit Tests for index.c
 *
 * The following lines of code are unit tests for index.c. These tests can be
 * run using the -DUNIT_TEST option during compilation. There will be tests
 * for all of the functions that are exposed to the user. The headers for
 * each of the testing functions will describe the tests indepth. 
 */

#include<assert.h>

static void test_index_add(void);
static void test_index_delete(void);
static void test_index_save(void);
static void test_index_set(void);

int main(void)
{
  test_index_add();
  test_index_delete();
  test_index_save();
  test_index_set();
  return 0;
}

/* Tests the adding function of the index module.
 */
static void test_index_add(void)
{
  index_t *index = index_new();
  
  // NULL Arguments – Expect false
  assert(index_add(NULL, "hi", 1) == false);
  assert(index_add(index, NULL, 1) == false);
  assert(index_add(index, "hi", -1) == false);

  // Basic Functionality of Adding To Index
  assert(index_add(index, "hello", 1));
  assert(index_add(index, "hello", 1));
  assert(index_add(index, "hello", 1));
  assert(index_add(index, "yo", 2));
  assert(index_add(index, "yo", 1));

  hashtable_t *ht = index->table;
  counters_t *hello_count = hashtable_find(ht, "hello");
  counters_t *yo_count = hashtable_find(ht, "yo");
  assert(counters_get(hello_count, 1) == 3);
  assert(counters_get(yo_count, 1) == 1);
  assert(counters_get(yo_count, 2) == 1);

  index_delete(index);
}

/* Tests the delete function of the index module.
 */
static void test_index_delete(void)
{
  index_t *index = index_new();

  // NULL Arguments -- Expect nothing
  index_delete(NULL);
  
  // Test Basic Functionality of Deleting
  index_add(index, "hello", 1);
  index_add(index, "hello", 1);
  index_add(index, "hello", 1);
  index_add(index, "yo", 2);
  index_add(index, "yo", 1);

  index_delete(index);

  // Check for memory leakage, when previous variable has been 
  // destroyed.
  index = index_new();
  index_add(index, "this is new", 1);

  index_delete(index);
}

/* Tests the save function of the index module.
 */
static void test_index_save(void)
{
  index_t *index = index_new();
  FILE *fp1 = fopen("test1.output", "w");
  FILE *fp2 = fopen("test2.output", "w");

  // Invalid arguments.
  assert(index_save(NULL, index) == false);
  assert(index_save(fp1, NULL) == false);
  
  // Saving an empty index should be nothing.
  assert(index_save(fp1, index));


  // Basic Functionality – The tester needs to visually inspect these files.
  index_add(index, "hello", 1);
  index_add(index, "hello", 1);
  index_add(index, "hello", 1);
  index_add(index, "yo", 2);
  index_add(index, "yo", 1);

  assert(index_save(fp2, index)); 

  index_delete(index);
  fclose(fp1);
  fclose(fp2);
}

static void test_index_set(void)
{
  index_t *index = index_new();
  hashtable_t *ht;
  counters_t *ct;

  // Add some basic items to the index.
  index_add(index, "hello", 1);
  index_add(index, "hello", 1);
  index_add(index, "hello", 1);
  index_add(index, "yo", 2);
  index_add(index, "yo", 1);

  // Set indicies invalid arguments
  assert(index_set(NULL, NULL, 0, 0) == false);
  assert(index_set(index, NULL, -1, 0) == false);
  assert(index_set(index, "hello", 2, -1) == false);

  // Change existing counts of documents
  assert(index_set(index, "hello", 1, 10));
  assert(index_set(index, "yo", 1, 3));
  assert(index_set(index, "yo", 2, 5));

  ht = index->table;
  ct = hashtable_find(ht, "hello");
  assert(counters_get(ct, 1) == 10);
  ct = hashtable_find(ht, "yo");
  assert(counters_get(ct, 1) == 3);
  assert(counters_get(ct, 2) == 5);

  // Set index of non-exisiting words.
  assert(index_set(index, "this", 1, 10));
  assert(index_set(index, "new_word", 2, 5));
  assert(index_set(index, "word", 3, 50));

  ct = hashtable_find(ht, "this");
  assert(counters_get(ct, 1) == 10);
  ct = hashtable_find(ht, "new_word");
  assert(counters_get(ct, 2) == 5);
  ct = hashtable_find(ht, "word");
  assert(counters_get(ct, 3) == 50);

  index_delete(index);
}

#endif

