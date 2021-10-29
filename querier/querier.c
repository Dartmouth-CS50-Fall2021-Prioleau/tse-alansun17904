#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include "file.h"
#include "tokenizer.h"
#include "hashtable.h"
#include "counters.h"
#include "index.h"


bool check_valid_query(tokenizer_t *token);

int main(void)
{
  char *query; 
  tokenizer_t *token;

  printf("Query: ");
  while ((query = readlinep())) {
    token = tokenize(query);
    if (!check_valid_query(token)) {
      if (token != NULL)
        delete_tokenizer(token);
      free(query);
    } else {
      printf("valid query\n");

    }
  }
  return 0;
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


