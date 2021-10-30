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


bool check_valid_query(tokenizer_t *token);
void parse_args(int argc, char *argv[]);
counters_t *score_query(tokenizer_t *token, char *index_filename);

int main(int argc, char *argv[])
{
  char *query; 
  tokenizer_t *token;

  parse_args(argc, argv); 

  printf("Query: ");
  while ((query = readlinep())) {
    printf("%s\n", query);
    token = tokenize(query);
    if (!check_valid_query(token)) {
      if (token != NULL)
        delete_tokenizer(token);
      free(query);
    } else {
      counters_t *count = score_query(token, argv[2]);  
      counters_print(count, stdin); 
      counters_delete(count);
    }
  }
  return 0;
}

void parse_args(int argc, char *argv[])
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

bool check_valid_query(tokenizer_t *token)
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

counters_t *score_query(tokenizer_t *token, char *index_filename)
{
  FILE *fp;
  counters_t *curr, *local, *global, *temp;
  local = NULL;

  if (token == NULL || index_filename == NULL)
    return NULL;
  fp = fopen(index_filename, "r");
  index_t *index = index_load(fp);
   
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
        }
      }
    // We get an "or" so now we must perform union.
    } else {
      if (global == NULL) {
        global = local;
      } else {
        temp = global;
        global = unions(local, global);
        counters_delete(temp);
      }
    }
  }
  return global == NULL ? local : global; 
}
