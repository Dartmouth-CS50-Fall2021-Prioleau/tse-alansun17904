#include<string.h>
#include<ctype.h>

/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine
 * Word Normalization Helper (word.c)
 *
 * The function in this module is a utility function that normalizes a
 * given string. This means that the string will have its alphabetic 
 * characters converted into all lowercase. This is necessary for the
 * indexer, as the indexer wants to consider all words regardless of
 * the casing; in this way it can then perform comparisons across 
 * various documents without losing information.
 */

/* (description): The `NormalizeWord` function converts all of the letters 
 * of the given string into lower case letters. 
 *
 * (input): The function requires a single argument that is a string. 
 *
 * (output): The function will modify the given string in place. It will return
 * a pointer to the string that was passed in, except now all the characters
 * in the string will be lower case. We note that the function ignores any 
 * non-alphabetic characters. 
 *
 * (error handling): If the given string is NULL then NULL is returned. 
 * We also note that the function will not dynamically allocated new memory
 * for the string that is returned.
 */
char *NormalizeWord(char *word)
{
  if (word == NULL)
    return word;
  for (int i = 0; i < strlen(word); i++) {
    word[i] = tolower(word[i]);
  }
  return word;
}

