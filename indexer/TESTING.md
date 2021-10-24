# Testing
This file describes how I tested both my indexer and my indextest programs. 

## Usage
To run the tests that I have created one can run the folloing commands:
```
make clean
make 
make test 
```
This will create a new file `testing.out` that contains all of the outputs of the tests. 
Please note that the testing program will output some files that contain the index files
created by the indexer. These will not need to be manually inspected by the tester as its
contents will be automatically compared to the expected test results given in the shared
directory.

To run the test with valgrind, the tester can simply replace the last command with:
```
make valgrind
```

## Tests
In the first subsubsection, we will describe the tests for the indexer; then, in the 
second subsubsection, we will describe the tests for the indextest program.

### `indexer` Tests
- Test with an invalid number of arguments: less than 2 or greater than 2. This
should result in the program printing out an error message.
- Test with an invalid directory: a directory that does not have a `.crawler` file.
- Test with a valid directory that contains no files. This should result in an empty
index file.
- Test with a directory that only contains one files created by the crawler. 
- Test with a directory that contains multiple files:
  - Letters on many depths.
  - To scrape on many depths. 
  - Wikipedia on many depths (this is to try and see if the indexer can be overloaded).

We note that the expected index files are also in the shared directory. Thus, using
the provided awk script that sorts an index file, we can automatically check if our
outputted index file is correct. To learn more about this, see the `check_index_equal`
funtion in the `testing.sh` script.

### `indextest` Tests
- Test with an invalid number of arguments: less than 2 or greater than 2.
This also should result in the program terminating and printing out an error message.
- Test with an invalid file: one that is nonexistent.
- Test with an empty file: this should output a new index file that is also empty.
- Test with valid cases:
  - Letters index on many depths.
  - To scrape index on many depths.
  - Wikipedia index on many depths.

Similar to the `indexer` tests we also automatically check to see if the outputted file 
is the same as the given indexer file.

