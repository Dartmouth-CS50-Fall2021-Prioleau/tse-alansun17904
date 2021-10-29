/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Querier 
 * Module for tokenizer (tokenizer.h)
 *
 * This is the header file for the tokenizer module. It defines a tokenizer
 * struct that is basically just a structure that wraps an array of strings
 * except it also encapsulates the number of items that are in the array.
 * 
 * It also defines some functionalities that tokenizes a string, as well a
 * function to delete the tokenizer.
 */

#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef struct tokenizer {
  char **words;
  int num;
} tokenizer_t;


tokenizer_t *tokenize(char *query);

void delete_tokenizer(tokenizer_t *tokenizer);


#endif
