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


static bool check_valid_query(tokenizer_t *token);
static void parse_args(int argc, char *argv[]);
static counters_t *score_query(tokenizer_t *token, index_t *index);
static void num_items(void *, const int, const int);
static void print_counters(void *, const int, const int);
static void print_results(counters_t *counts, char *path);


int main(int argc, char *argv[])
{
  char *query; 
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
        print_results(count, argv[1]);
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

static void print_counters(void *arg, const int key, const int count)
{
  char *path = arg; 
  FILE *fp;
  char *id_s = malloc(6); // "/" + 4 digits + \0 character
  char *fname = malloc(strlen(path) + 7);
  char *url;

  // Construct filename for given HTML file.
  sprintf(id_s, "/%d", key);
  strcpy(fname, path);
  strcat(fname, id_s);

  // Get the URL of the target page.
  fp = fopen(fname, "r");
  if (fp == NULL) {
    printf("couldn't find page %d in %s\n", key, fname);
    free(id_s), free(fname);
  } else {
    url = freadlinep(fp);
    printf("score %4d doc %4d: %s\n", count, key, url);
    free(url), free(id_s), free(fname);
    fclose(fp);
  }
}

static void print_results(counters_t *counts, char *path)
{
  int count = 0;

  counters_iterate(counts, &count, num_items);
  printf("Matches %d documents (unranked):\n", count);
  counters_iterate(counts, path, print_counters); 
  printf("-----------------------------------------------\n");
}

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

static void num_items(void *arg, const int key, const int count) 
{
  int *num = arg;
  (*num)++;
}

