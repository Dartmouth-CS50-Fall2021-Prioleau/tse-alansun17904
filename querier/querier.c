#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include "file.h"
#include "tokenizer.h"
#include "hashtable.h"
#include "counters.h"
#include "operators.h"
#include "index.h"


bool check_valid_query(tokenizer_t *token);
void parse_args(int argc, char *argv[]);

int main(int argc, char *argv[])
{
  char *query; 
  tokenizer_t *token;

  parse_args(argc, argv); 

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


