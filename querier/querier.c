#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include "file.h"
#include "tokenizer.h"
#include "hashtable.h"
#include "pagedir.h"
#include "counters.h"
#include "operators.h"
#include "index.h"

/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Querier
 * Querier (querier.c)
 *
 * This program implements the main functionality of the querier. The main
 * function takes two arguments: the directory to the crawler generated files
 * as well as the path to the index file. Given these two parameters, the 
 * querier will then accept arguments to query the given indexed files.
 * The exact usage/implementation/design are described in the README.md,
 * IMPLEMENTATION.md and DESIGN.md files.
 */

typedef struct sorted {
  int *sort;
  int curr;
  int nums;
} sorted_t;

static bool check_valid_query(tokenizer_t *token);
static void parse_args(int argc, char *argv[]);
static counters_t *score_query(tokenizer_t *token, index_t *index);
static void num_items(void *, const int, const int);
static void print_results(sorted_t* sorted, counters_t *counts, char *path);
static sorted_t *sort(counters_t *count);

/* (description): The main function for the querier. The main parses the
 * arguments and checks them for valditiy. If these are valid, then it
 * will initiate the querier. The querier will first check if the given
 * query is syntatically and semantically correct, it will then collate the
 * queried files from the index file and present these files back to the 
 * user ranked in terms of score.
 * 
 * (inputs): The first argument is the path to the directory created by the
 * crawler and the second argument is the path to the corresponding index file
 * that follows the format outlined in (index.c).
 *
 * (outputs): Print out the documents which match the query patterns. Each page
 * is assigned a score. For ease, those documents with highest score (ones that
 * match the query most closely) are shown first, while documents with the
 * lowest scores (documents that match the query the least) are shown last.
 *
 * (error handling): If any of the given arguments are invalid then the querier
 * will terminate with an error message. If the given query is invalid (e.g. 
 * does not follow the syntax outlined in the README.md), then an error message
 * is printed and the querier will ask for another query.
 */
int main(int argc, char *argv[])
{
  char *query; 
  sorted_t *sorted;
  tokenizer_t *token;
  index_t *index;
  FILE *fp;

  parse_args(argc, argv); 
  fp = fopen(argv[2], "r");
  index = index_load(fp);
 
  printf("Query: ");

  while ((query = readlinep())) {
    printf("%s\n", query);
    token = tokenize(query);
    if (check_valid_query(token)) {
      counters_t *count = score_query(token, index);  
      if (count == NULL) {
        printf("Matches 0 documents.\n");
      } else {
        sorted = sort(count);
        print_results(sorted, count, argv[1]);
        free(sorted->sort);
        free(sorted);
        counters_delete(count);
      }
    }
    printf("Query: ");
    free(query);
    if (token != NULL)
      delete_tokenizer(token);
  }
  fclose(fp);
  index_delete(index);
  return 0;
}

/* (description): Parses the arguments given to the main function, if 
 * any of the arguments are invalid, then the program will terminate with an 
 * appropriate message.
 *
 * (inputs): The first argument represents the number of arguments, while the
 * second represents the actual arugments themselves.
 */
static void parse_args(int argc, char *argv[])
{
  // There can only be two arguments.
  FILE *fp;

  if (argc != 3) {
    fprintf(stderr, "usage: ./querier <crawlerDirectory> <indexFilename>\n");
    exit(1);
  } else if (!is_crawler_dir(argv[1])) {
    fprintf(stderr, "given directory argument was not a directoy created" \
                    " by the crawler.\n");
    exit(2);
  } else if ((fp = fopen(argv[2], "r")) == NULL) {
    fprintf(stderr, "given index file cannot be read.\n");
    exit(3);
  }
  fclose(fp);
}

/* (description): A helper function that copies the given keys of a counter
 * into an array that is wrapped into a `sorted` structure. 
 *
 * (inputs): The parameters correspond to the helper types that are required
 * for counters_iterate. The first parameter `void *arg` is a pointer to a 
 * `sorted_t` structure (an array wrapper containing the array itself, where
 * the array is currently "at" -- how full it currently is, and the length of
 * the array).
 */
static void copy_keys(void *arg, const int key, const int count)
{
  sorted_t *keys = arg;

  keys->sort[keys->curr] = key;
  (keys->curr)++;
}

/* (description): A helper function that copies the given counts of a counter
 * into an arrya that is wrapped into a `sorted` structure.
 *
 * (inputs): The parameters correspond to the helper types that are required
 * for counters_iterate. The first parameter `void *arg` is a pointer to a 
 * `sorted_t` structure (an array wrapper containing the array itself, where
 * the array is currently "at" -- how full it currently is, and the length of
 * the array).
 */
static void copy_items(void *arg, const int key, const int count)
{
  sorted_t *items = arg;

  items->sort[items->curr] = count;
  (items->curr)++;
}

/* (description): The `sort` function will sort a given counter in terms of 
 * its counts. It accomplishes this by creating two wrapped arrays, sorting
 * one of the arrays (the one containing the items), while simutaneously
 * changing the order of the keys as well. 
 *
 * (inputs): A pointer to a non_empty counter
 *
 * (ouputs): A wrapped array that contains the keys of the given counter in
 * the order of descending counters. For example, a counter with the following
 * keys and counts:
 *
 *              1 -> 5 ; 2 -> 8 ; 4 -> 1 ; 6 -> 3 ; 
 * 
 * Will be exported in a sorted array of keys according to their counts as 
 * follows:
 *
 *              [    2       1      6       1     ]
 * 
 * (error handling): The function assumes a non-empty counter, given a 
 * empty counter will result in undefined behavior (most likely some sort
 * of error).
 */
static sorted_t *sort(counters_t* count)
{
  int num = 0, max = 0, temp;
  sorted_t *keys = malloc(sizeof(sorted_t));
  sorted_t *items = malloc(sizeof(sorted_t));
  
  // Copy the keys and items into separate arrays.
  counters_iterate(count, &num, num_items);
  keys->sort = calloc(num, sizeof(int));
  keys->nums = num;
  keys->curr = 0;
  items->sort = calloc(num, sizeof(int)); 
  items->nums = num;
  items->curr = 0;
  counters_iterate(count, keys, copy_keys);
  counters_iterate(count, items, copy_items);

  // Sort based on descending order, largest score in the front.
  for (int i = 0; i < items->nums; i++) {
    max = i;
    // Find where the max is in the remaining array.
    for (int j = i; j < items->nums; j++) {
      if (items->sort[j] > items->sort[max]) {
        max = j;
      }
    }
    // Swap max with the current.
    temp = items->sort[i];
    items->sort[i] = items->sort[max];
    items->sort[max] = temp;
    // Do the same for keys.
    temp = keys->sort[i];
    keys->sort[i] = keys->sort[max];
    keys->sort[max] = temp;
  }
  // Free unused wrapped items array.
  free(items->sort);
  free(items);
  return keys;
}

/* (description): The `print_results` function prints the results of a given
 * counter and its sorted keys. The output is organized into three columns:
 * the score, the doc ID, as well as the URL where this document is found.
 * Examples of this is shown in the UI subsection of the design spec.
 *
 * (inputs): The function takes three arguments, the first is the wrapped
 * array that contains the keys sorted with respect to their counts in 
 * descending order, the second is the counter that is bring displayed, and
 * the last is the path to the crawler directory where the target file IDs
 * correspond to.
 *
 * (outputs): Printing to stdout the aforementioned format, with each row
 * representing a separate matched file.
 */
static void print_results(sorted_t *sorted, counters_t *counts, char *path)
{
  FILE *fp;
  char *id_s, *fname, *url;
  int key;

  printf("Matches %d documents (ranked):\n", sorted->nums);

  // The keys in the `sorted` array are already in descending order, so
  // we just need to traverse this wrapped array in order.
  for (int i = 0; i < sorted->nums; i++) {
    id_s = malloc(6);
    fname = malloc(strlen(path) + 7);
    key = sorted->sort[i];
    // Construct filename for given HTML file.
    sprintf(id_s, "/%d", key);
    strcpy(fname, path);
    strcat(fname, id_s);

    fp = fopen(fname, "r");
    if (fp == NULL) {
      // No corresponding page in crawler that was in the index file.
      printf("couldn't find page %d in %s\n", key, fname);
      free(id_s), free(fname);
    } else {
      // Get the URL of the target page.
      url = freadlinep(fp);
      printf("score %4d doc %4d: %s\n", counters_get(counts, key), key, url);
      
    }
    free(url), free(id_s), free(fname);
    fclose(fp);
  }
  printf("-----------------------------------------------\n");
}

/* (description): The `check_valid_query` function checks if a given tokenized
 * query is syntatically and semantically valid. These definitions are given in
 * the README.md.
 *
 * (inputs): A tokenized query in the form of a pointer to a tokenizer struct.
 *
 * (outputs): True if the given query is valid, and false if otherwise.
 * An accompanying message will also be printed if the command is invalid.
 */
static bool check_valid_query(tokenizer_t *token)
{
  if (token == NULL || token->num == 0) {
    fprintf(stderr, "Error: invalid character detected, characters" \
                    " must be alphabetic.\n");
    return false;
  }
  for (int i = 0; i < token->num; i++) {
    if (strcmp(token->words[i], "and") == 0 || 
                    strcmp(token->words[i], "or") == 0) {
      // Check if trailing or leading logical operator.
      if (i == 0 || i == token->num - 1) {
        fprintf(stderr, "Error: there cannot be trailing or leading logical " \
                        "operators in the query.\n");
        return false;
      // The previous word and next word must not be a logical operator.
      } else if (strcmp(token->words[i-1], "and") == 0 || 
                 strcmp(token->words[i-1], "or")  == 0 ||
                 strcmp(token->words[i+1], "and") == 0 ||
                 strcmp(token->words[i+1], "or")  == 0) {
        fprintf(stderr, "Error: there cannot be consecutive logical " \
                        "operators in the query.\n");
        return false;
      }
    }
  }
  return true;
}

/* (description): The `score_query` function does most of the heavy lifting 
 * associated with the querier. It parses the tokenized command and returns
 * a counter that represents the documents that most closely match to the
 * given command. The counts of this counter represent the score (how closely
 * a document matches with the given query) of the key. The pseudocode for this
 * function is given in IMPELMENTATION.md. 
 *
 * (inputs): This function takes two arguments, the first is the pointer to the
 * tokenized query. It is assumed that the given tokenized contains a valid
 * query that adheres to the syntax described in the README.md The second
 * argument a pointer to an index struct that contains the words and their
 * associated counts for each document. 
 *
 * (outputs): A counter whose keys are document IDs and items are scores. This
 * is unsorted and must be organized using the `sort` function.
 */
static counters_t *score_query(tokenizer_t *token, index_t *index)
{
  counters_t *curr, *local = NULL, *global = NULL, *temp;
  local = NULL;
    
  // Parse query based on query syntax described in REQUIREMENTS.md
  for (int i = 0; i < token->num; i++) {
    /* If the token is an "or" then we combine what we currently have
     * as the "local" counter with the "global" counter.
     * Otherwise, if the token is an "and" we ignore and continue appending
     * to the local counter.
     */
    if (!(strcmp(token->words[i], "or") == 0)) {
      // If not a logical operator, then we get the counter for curr word.
      if (!(strcmp(token->words[i], "and") == 0)) {
        /* If the word cannot be found then we continue. Otherwise, we perform
         * an intersection of the current counter with local.
         */
        if ((curr = index_get(index, token->words[i])) != NULL) {
          // At the first word.
          if (local == NULL)
            local = dup_counter(curr);
          temp = local;
          // perform the AND operation with previous words.
          local = intersections(local, curr);
          counters_delete(temp);
        } else {
          counters_delete(local);
          local = NULL;
        }
      }
    // We get an "or" so now we must perform union between local and global.
    } else {
      if (global == NULL) {
        global = local;
        local = NULL;
      } else if (local != NULL) {
        temp = global;
        // Perform the OR operation with previous AND-ed words.
        global = unions(local, global);
        counters_delete(temp), counters_delete(local);
        local = NULL;
      }
    }
  }
  // Nothing found.
  if (global == NULL && local == NULL) {
    return NULL;
  // Query only had ANDs
  } else if (global == NULL) {
    return local;
  // Right-hand-side of OR could not be found.
  } else if (global != NULL && local == NULL) {
    return global;
  // Merge right-hand-side of OR with left-hand-side.
  } else {
    temp = global;
    global = unions(local, global);
    counters_delete(temp), counters_delete(local);
  }
  return global;
}

/* (description): A helper that couples with `counters_iterate` this function
 * counters the number of keys that are in the counter. It is useful for
 * checking the number of matched documents as well as sorting.
 *
 * (inputs): This function and its paramters also conform to the type of 
 * function pointer that is required by `counters_iterate`.
 */
static void num_items(void *arg, const int key, const int count) 
{
  int *num = arg;
  (*num)++;
}

