# Tiny Search Engine: Indexer
## Description
This directory serves the source code and testing of the indexer. Briefly, the 
indexer "indexes" files created by the crawler. It does this by tallying the 
words in each HTML file and then creating a data structure that keeps track
of the count of each word in each file. 

This part of the project will be useful for the querier as it uses existing
indices to determine how much a page matches what the user desires. 

To read more about the project, my design, and implementation, please go to
`DESIGN.md` and `IMPLEMENTATION.md`. More about testing is in the
`TESTING.md` file. 

## Usage
To run the indexer one needs to run the following sequence of commands:
```bash
make 
```
This will make both the indexer and the indextest. The latter reads in a
file created by the indexer, reinitializes the index data structure that
is associated with this index file and then re-outputs it to a given path.

The following command will run the indexer;
```bash
./indexer <path to crawler dir> <name of index file>
```
where the first argument is the path to the directory of files created by
the crawler, and the second argument is the name of the index file that
will be created by the indexer.

The following command will run indextest:
```bash
./indextest <path to index file> <path to new index file>
```
where the first argument is the path to an existing index file, and the second
argument is the path to the new file that the indexer will created.

## Assumptions
Here are some of the assumptions that my indexer makes, the user should be 
aware of these before running the program, as any inputs/functions outside
of the scope of these assumptions will be undefined behavior:

- A directory created by a crawler will always contain a `.crawler` file.
- The indexer will only read the files in the top-level directory inside
the crawler. Any files that are contained in folders in the crawler directory
will be ignored.
- The files in the directory of the crawler are labelled as ordinal numbers 
start at 1, increasing linearly by 1.
- There are no more than 999 total files in the crawler directory. 

Likewise, here are some of the assumption that my indextest program makes:

- If given an existing file for the first argument of the program, the
program will assume that this is a valid file created by the indexer.
- All words in the given file are no larger than 255 characters 
(not including the NULL character).

## References
Please note that I referenced the lecture notes as well as the activities 
we did in class extensively. These are given here:
```https://www.cs.dartmouth.edu/~cs50/```

