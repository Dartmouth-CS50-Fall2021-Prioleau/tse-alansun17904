#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "tokenizer.h"
/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Querier
 * Module for the tokenizer (tokenizer.c)
 *
 * This module implements some functionality that tokenizes a given query.
 * This means separating a string into words through its spaces. The module 
 * will then seek to return an array of words in the query. Details about the
 * specific endpoints are provided in the function headers.
 */

/* (description): The `tokenize` function takes in a string, assumes that its
 * a valid query in that it contains no whitespace characters other than 
 * space; it contains only alphabetic characters. The function will then 
 * loop through the string, replacing the spaces with null characters and
 * assign a pointer to the start of each word - store these words in a 2D 
 * array, as well as the number of words in the 2D array, wrapping these in
 * a tokenizer struct. The function will then return this struct. Note that
 * the passed in string must be dynamically allocated.
 *
 * (inputs): The function takes one argument: the query string. It assumes 
 * that this query string contains only alphabetic characters and spaces. Thus,
 * the user will need to pre-validate queries for such cases.
 *
 * (outputs): A tokenizer struct that is dynamicaly allocated containing an
 * array that is also dynamically allocated. The user will need to free both
 * of these later by calling the `delete_tokenizer` function.
 *
 * (error handling): If the program fails to allocate memory, it will terminate
 * its own process and the calling processes. This may result in memory leaks
 * in the calling processes. If the given string is NULL, then NULL is returned.
 */
tokenizer_t *tokenize(char *query)
{
  char c0 = '\0';
  char c1;
  int curr = 0;
  tokenizer_t *tokens;

  if (query == NULL)
    return NULL;
  /* We know that the number of words in the string must be less than the 
   * number of characters in the string. Thus, we allocate ample memory first
   * and then resize.
   */
  c1 = query[curr];
  tokens = malloc(sizeof(tokenizer_t));
  tokens->words = calloc(strlen(query), sizeof(char *));
  tokens->num = 0;

  while (c1 != '\0') {
    /* The first letter of a word is defined as a character whose previous
     * character is a space. 
     */
    if (!isalpha(c1) && !isspace(c1)) {
      delete_tokenizer(tokens);
      return NULL;
    } else if ((isspace(c0) || c0 == '\0') && isalpha(c1)) {
      tokens->words[tokens->num] = query + curr;
      (tokens->num)++;
    // If current character is a space, then we replace it with a null byte.
    } else if (isspace(c1)) {
      query[curr] = '\0'; 
    }
    // Increment the pointers, curr/prev character values. 
    c0 = c1;
    c1 = query[++curr];
  }
  return tokens;
}

/* (description): The `delete_tokenizer` function deletes a tokenizer struct.
 * It is advised for this function to be called when the user is done with
 * their usage of a tokenizer struct to avoid memory leaks. 
 *
 * (inputs): A pointer to a tokenizer. If this is NULL, then the function will
 * do nothing. Otherwise, it will free the memory associated with the array of
 * words in the tokenizer struct as well as the memory associated with the
 * pointer to the tokenizer struct.
 */
void delete_tokenizer(tokenizer_t *tokenizer)
{
  if (tokenizer == NULL)
    return;
  free(tokenizer->words);
  free(tokenizer);
}



// ########################################################################
// ########################################################################
//                         Unit Test for Tokenizer
// ########################################################################
// ########################################################################
#ifdef UNIT_TEST
#include<assert.h>

void test_tokenizer(void);
void test_deletion(void);

int main(void) {
  test_tokenizer();
  test_deletion();
}

/* A helper where given a string literal, it will create an dynamically
 * allocated version of this string.
 */
char *make(char *string) {
  char *s = malloc(strlen(string) + 1);
  strcpy(s, string);
  return s;
}

void test_tokenizer(void) {
  char *test;
  tokenizer_t *t;
  // Basic valid
  
  t = tokenize(test = make("       "));
  assert(t->num == 0);
  delete_tokenizer(t);
  free(test);

  t = tokenize(test = make("dartmouth    "));
  assert(t->num == 1);
  assert(strcmp(t->words[0], "dartmouth") == 0);
  delete_tokenizer(t);
  free(test);

  t = tokenize(test = make("  dartmouth\t   college  ")); 
  assert(t->num == 2);
  assert(strcmp(t->words[0], "dartmouth") == 0);
  assert(strcmp(t->words[1], "college") == 0);
  free(test);
  delete_tokenizer(t);

  // Invalid test cases
  assert(tokenize(test = make(" 4kj490 ")) == NULL);
  free(test);

  assert(tokenize(test = make("$")) == NULL);
  free(test);

  assert(tokenize(test = make("my name is alan!")) == NULL);
  free(test);

  assert(tokenize(NULL) == NULL);

  // Lengthy valid
  t = tokenize(test = make("     hello world   this  is a test      of" \
                  "         the      querier    tokenizer "));
  assert(t->num == 10);
  assert(strcmp(t->words[0], "hello") == 0);
  assert(strcmp(t->words[1], "world") == 0);
  assert(strcmp(t->words[2], "this") == 0);
  assert(strcmp(t->words[3], "is") == 0);
  assert(strcmp(t->words[4], "a") == 0);
  assert(strcmp(t->words[5], "test") == 0);
  assert(strcmp(t->words[6], "of") == 0);
  assert(strcmp(t->words[7], "the") == 0);
  assert(strcmp(t->words[8], "querier") == 0);
  assert(strcmp(t->words[9], "tokenizer") == 0);
  free(test);
  delete_tokenizer(t);
}

void test_deletion(void) {
  // Test invalid
  delete_tokenizer(NULL);
}

#endif 
