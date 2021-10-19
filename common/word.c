#include<string.h>
#include<ctype.h>

/* The `NormalizeWord` function converts all of the letters of the given
 * string into lower case letters. It ignores any non-alphabetic characters.
 * Moreover, if the given string is NULL then NULL is returned. We note that
 * the string returned is modified in place. It will not dynamically 
 * allocated new memory for the string that is returned.
 */
char *NormalizeWord(char *word) {
  if (word == NULL)
    return word;
  for (int i = 0; i < strlen(word); i++) {
    word[i] = tolower(word[i]);
  }
  return word;
}
