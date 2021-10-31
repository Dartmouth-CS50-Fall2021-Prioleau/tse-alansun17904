# Common
This directory and all of its programs in it serve as a library for some 
utility and helper functions for tiny search engine. We now proceed to
briefly describe each function and its use case. However, please note
that much of the detail is presented in the actual source files themselves.

## Usage
To start, run the following command.
```bash
$ make
```
This will initialize the library, compiling all of the files and organize
them into a static library called `common.a`. Later on, when make-ing other files, this can be included as one of the dependencies and linked with the other 
necessary object files. 
**Please note that if changes are made to any of the component source files
that that specific file needs to be compiled again, and the library will
need to be re-linked. Otherwise, its API endpoints may not be updated.**

## Structures

### `tokenizer`
The tokenizer structure is useful to the querier, it stores the results of the tokenized query.
This structure represents a simple wrapper of an array of strings and an integer denoting the 
length of this array. Since this structure itself is easy to digest and described in `tokenizer.c`, 
we will proceed by describing some of the functionalities of this structure. The unit tests for this
structure are self-contained within the `tokenizer.c` module.

#### `tokenize`
This function takes a query string as input and returns a newly created `tokenizer_t` structure. 
Please note that the user must call `delete_tokenizer` when they are done with the structure to 
avoid memory leaks. 
```
tokenizer_t *tokenize(char *query);
```

#### `delete_tokenizer`
Frees all of the memory associated with a given `tokenizer_t *` structure. We note that this
function does not free the string that the tokenizer is pointing at. If the string is dynamically
allocated, the user needs to free this manually after freeing the tokenizer.



### `index`
Here we describe the index data structure that is used throughout indexer. The details of how 
specifically this structure works and the motivation for this is in `index.c`. So, we will summarize
this briefly. The purpose of the index data structure is to store, for each word, a list of IDs of 
documents where this word appears as well as the number of times this word appears in each document. 

We accomplish this by using a hashtable whose keys are words and whose items are counters that store
the document ID and the number of times the given word has appeared in the document.

The following describes some of the functionalities that are avaliable to the user:

#### `index_new`
Creates a new index data structure and allocates memory for this. 
```C
index_t *index_new(void);
```

#### `index_add`
Increments the count of a word associated with a given document.
```C
bool index_add(index_t *index, int id);
```
The function will return true if the insertion was successful. Otherwise, the function
will return false.

#### `index_save`
Saves the contents of an index data structure to a given file.
```C 
bool index_save(FILE *fp, index_t *index);
```
The function will return true is this process was successful and false otherwise.

### `index_delete`
Deletes all of the memory associated with a given index data structure. 
```C
void index_delete(index_t *index);
```


## Functionalities
`pagedir.c` contains utility functionality for checking whether a given
directory is writable. 

### `pagedir`
Now, we describe the functions within the `pagedir` module. 

##### `is_dir_writable`
Checks whether a given directory is writable. If it is then true is outputted, otherwise, false is returned.
```C
bool is_dir_writable(char *path);
```

##### `is_crawler_dir`
Checks whether a given directory was created by the crawler. If it is then true is outputted, otherwise, false 
is returned.
```C
bool is_crawler_dir(char *path);
```

### `word`
Here, we describe the functions within the `word` module.

#### `NormalizeWord`
Normalizes a given word. It does this by converting all of the letters in the word to lowercase. This 
is extremely helpful for the indexer, as the indexer wants to keep track of the "raw" information,
regardless of typesetting. The function will return the same pointer that it is given, only now, the 
contents of this pointer will be all lower case letters.
```C
char *NormalizeWord(char *word);
```

### `operators`
This module implements some operators that can be performed on counters, namely, intersection
and unions. This is also extremely useful for querier because they can be used to represent 
the logical relationships of AND and OR. Here, we not describe the specific implementations of
these individual functions or the helper functions these functions call. This detailed information
can be found in the `operators.c` module.
#### `counters_t *intersections(counters_t *c1, counters_t *c2);`
The intersections function creates a newly dynamically allocated counter that represents the
intersection between the two given counters. If either of the counters is NULL, then NULL
is returned. If the same key appears in both counters, this key will be added to the newly 
dynamically allocated counter and the item of this key will be the minimum of the count in
`c1` and the count in `c2`. If there is no intersections between the two given counters, then
an empty counter is returned. Please note that the user must free the resulting counter, once
they are done working with it. Both of the provided counters remain unchanged.

#### `counters_t *unions(counters_t *c1, counters_t *c2);`
Find the union of two counters and stores the result in a newly dynmically allocated counter,
similar to counters intersection. If the same key exists in both counters, then this key will
be added to the new counter and its count will be the summation of the counts in `c1` and `c2`.
Similar, to `intersections` the user must free the resulting counter when they are done working
with it to prevent memory leaks. Both of the provided counters remain unchanged.

#### `counters_t *dup_counter(counters_t *counter);`
Creates a dynamically allocated duplicate of the given counter. This must be freed by the user
after they are done working with it. Note that the given counter will be left unchanged.

## Testing
There are unit tests are associated with the index data structure and the functions associated with 
this structure. To run the test one can run the following command from `make`:
```
make test_index
make test_tokenizer
make test_operators
```
This will compile the index, tokenizer, and operators and run all of the unit tests. Afterwards, the user can clean up the
results of the tests using:
```
make clean
```
Please note that some of these tests require visual inspection from the tester.
