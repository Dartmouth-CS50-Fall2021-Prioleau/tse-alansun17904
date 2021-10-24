# CS50 TSE: Indexer Implementation Spec
This is the implementation specificiation for the indexer component of the tiny search engine.
It contains a basic description of the progam: how to run it. It also contains some of my
thought processes for the implementation of these functionalities: pseudocode for the crawler.

## Purpose
As stated in the `README.md` in this directory, for a search engine to work it must require
three components: a crawler which collects all of the pages that can be searched, an indexer
which sorts and organizes all of the data of the crawler into a data structure that can
be easily manipulated, and a quierier which can process the user's queries and return the 
desired webpage. This part of the projet implements the indexer.

## Usage and Functionality
To run the program, the following commadns must be run beforehand:
```bash
$ make
$ ./indexer CRAWLER_DIR INDEX_PATH
```
In addition, we note that if `common.a` or `libcs50.a` do not exist, these need to be compiled 
and linked for the program to compile. This can be done by going into those respective programs
and running:
```
make
```

### Inputs
The indexer requires two arguments to run:
- `CRAWLER_DIR` a path to the a directory that is created by the crawler. We note that the program
validates that this directory is indeed created by the crawler if it contains a hidden `.crawler`
file. Moreover, within this directory the program assumes that the files in the directory are
labelled from 1... up increasing by 1 for each file.
- `INDEX_PATH` is the path to the index file where the indexer will save the contents of its 
data structure.

### Outputs
If the program runs successfully, with a non-zero status, then it will output nothing to `stdout`. 
Instead, it will save the contents of its newly created index data structure to the given path.
This file will be formatted as follows:
```
word1 id1 count1 id2 count2 id3 count3 ...
word2 id1 count4 id2 count5 id3 count6 ...
word3 id1 count7 id2 count8 id3 count9 ...
```

where the `word<i>` corresponds to the word whose count across many documents is being collated, 
`id<i>` corresponds to the ID (or filename) of the document where this word was found, and 
`count<i>` corresponds to the number of times this word was found in the document before it.

### Pseudo-code
This subsection describes my basic idea for implementing this program. It contains some pseudo-code
which may clarify the source code to the reader.
Please note that the hashtable/counters/index data structures that I am refering to are in the
`common.a` and `libcs50.a` archives. An `index` structure is simply a hashtable whose keys are words and whose items are counters.

```
<start of program>

check if the given arguments are valid:
  there must be two arguments
  the given crawler dir must be created by the crawler.
  the given writable index file must be writable.


create an index struct
since the files in the crawler are enumerated starting with 1 we loop 
until we can no longer read more files.

<start loop>
we begin with filename=1
  read and discard the first line -> url
  read and discard the second line -> depth
  place the remaining html into a webpage dir and read all of the words
  <start loop>
  we begin read every single word in the html
  we add a counter associated with this word into the hashtable
  the counters key will be the filename and its value will be incremented.
  <end loop>

if the filename+1 file does not exist then it means that we are done reading
<end loop>
<end of program>
```

### Error Handling and Recovery
- on any erros associated with the given arguments, the program will print as message
to `stderr` and it will terminate.
- If there are any errors with memory allocation, the program will print a message to
`stderr` and terminate immediately. This edge case may result in memory leaks.

### Persistent Storage
As states in the introduction, the program will create a new file whose name is 
dictated by the user. This file will contain the contents of the index data structure
that was created. For later use by the querier, this file can then be used to 
re-initialize an index struct by calling `read_index_file` in the indextest program.
