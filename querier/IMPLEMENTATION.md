# Implementation Specification for Querier
Here, we describe the implementation of the querier. This includes a description of 
each of the function prototypes and the how these are connected together through
pseudocode.

## Gist
This subsection gives a brief overview of the pseudocode for the main function. Note
that since the main function calls almost every other function, we show the 
pseudocode for these helpers in their own subsubsections. In the pseudocode, functions
that are denoted with `$$` are functions that exist separately. 
```C
<begin program>
  $parse_args$

  <begin loop (loop until EOF from user)>
    print prompt
    read the query from stdin
    $check_valid_query$
    $score_query$ (return counter associated with scores and documents)
    $sort$ the returned counter
    $print_results$ in the format specified in the README.dm
    clean up by freeing all the memory used.
  <end loop>
<end program>
```

## Function Prototypes
This subsection describes the function prototypes that are involved in the querier.
### Tokenizer
```C
tokenizer_t *tokenizer(char *query);
```
The tokenizer checks if a query is a valid query (must have all alphabetic
characters; read until a new line; only whitespace other than \n at the end 
must be spaces). It will replace these white space characters with null characters
and then add the first letter of every word to a `tokenizer` data structure. 
```
<begin function>

  the number of words must be less than the number of characters so, we 
  allocate the same number of spaces for words as the number of characters.

  <begin loop>
    if the current character is non-alphabetic or non-whitespace 
    then the query is invalid.

    if the character is a whitespace replace it with a null byte

    if the character is a alphabetic character that proceeds a null byte
    or whitespace then store the pointer to this character to the tokenizer

  <end loop>

  return this tokenized array, it needs to be freed later on

<end function>
```
The tokenizer then needs to be freed using the following function, whose header
is shown below.
```C
void delete_tokenizer(tokenizer_t *tokenizer);
```

### Querier
```C
static bool check_valid_query(tokenizer *token);
```
This function checks if the tokenized query is a valid query semantically. 
The query must not have a leading or trailing logical argument; it must also
not have a logical operator that is not wrapped by two non-logical words on 
either side.

```C
static void parse_args(int argc, char *argv[])
```
The function checks if the given arguments to the main function are valid. The
first argument must be a valid path to a crawler directory, and the second must
be a valid path to an index file. The function checks the validity of this first
argument by calling the `is_crawler_dir` function from the `pagedir` module (
essentially checking if the given directory has a `.crawler` hidden file). The
function checks if the second argument is valid by seeing if it is readable. It
does not check if the format is consistent with the indexer.

```C 
static counters_t *score_query(tokenizer_t *token, index_t *index);
```
The `score_query` function does most of the heavy lifting required for the querier.
The function takes two arguments a pointer to the tokenizer and a pointer to the 
inverted index that stores the relations between the words, counts, and document IDs.
The function will then output a counter that tallies the score associated with each
document. 
If the logical operator is `AND`, then the two counters in question will perform 
intersection, with the score for each counter being "min"-ed together. Otherwise,
the logical operator is `OR`, then the two counters in question will perform union,
with the score for each counter being summed together. The pseudocode for
this function is described below:
```
<start function>
  we keep track of a "global" and "local" variable
  <loop through the tokenizer>
    if the word is just a word and not a logical operator
    we perform intersection with the local variable

    if the word is "and" we ignore and continue

    if the word is "or" then we merge the local and global
    variables through union.
  <end loop>
  return the global variable if it is non-NULL, otherwise,
  reuurn the local variable. If both variables are NULL, then
  nothing was found.
<end function>
```

```C
static sorted_t *sort(counters_t *count) 
```
The `sort` function will sort a counter based on its counts. It will then place
these values into an array. It assumes that the given counter is non-NULL. The
pseudocode for the function is shown below:
```C
<start function>
  copy the keys and counts into separate arrays

  <begin loop (through the items)>
    <begin loop (through all the items)>
      find the maximum item
    <end loop>
    replace the maximum item with the current item
    peform the same swap with the keys, so they
    move in parallel.
  <end loop>
  clean up 
  return the array of keys
<end function>
```
```C
static void print_results(sorted_t *sorted, counters_t *counts, char *path)
```
The function will print the sorted array and the corresponding score for each 
document ID. The function accepts three inputs: a pointer to a wrapped array
of keys that are sorted with respect to their values (in descending order),
a pointer to the corresponding counter, and the path to the crawler dir where
the program will get the URLs for each document.

## Helper Prototypes
```C
static void num_items(void *, const int, const int);
```
Counts the number of items in a counter and stores them in the pointer to an 
integer given as a void pointer.

```C
static void copy_keys(void *, const int, const int);
```
Copies all of the keys in the counter into a given array.

```C
static void copy_items(void *, const int, const int);
```
Copies all of the items in the counter into a given array.

