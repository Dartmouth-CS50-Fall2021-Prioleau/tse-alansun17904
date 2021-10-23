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

## Testing
There are unit tests are associated with the index data structure and the functions associated with 
this structure. To run the test one can run the following command from `make`:
```
make test_index
```
This will compile the index and run all of the unit tests. Afterwards, the user can clean up the
results of the tests using:
```
make clean
```
Please note that some of these tests require visual inspection from the tester.
