# Tiny Search Engine: Querier
This is the directory for the last part of the tiny search engine project: the querier. 
The querier processes user queries and spits out the URLs that most closely match 
these queries. 

## Description
Brielfy, the querier will accept a user input. This input must be in the correct query
syntax:
- Queries are not case sensitive.
- Logical operators in queries include: AND, OR. 
- If words are chained together without logical operators, the querier will implicitly 
assume that these words are chained together with ANDs. 
- Logical operators must be surrounded by non-logical operator words. 
- Queries must only contain alphabetic characters. 
- All whitespace except for new lines are ignored.

The querier will tokenize a given query, if it is syntatically and semantically correct,
it will then proceed to search the file produced by the indexer for webpages that most
closely match the given query.

The querier will the return a list of URLs and their respective scores to the user. *Please
note that I was unable to implement a sorting functionality that would allow the query to
sort its results before presenting it to the user.*

## Usage
To run the querier, one needs to first compile the program by running the following sequences
of commands:
```bash
make
```
This will make the `querier` as well as the `fuzzquery` program which can randomly generate
queries given an index file.

The following command will then start the querier:
```bash
./querier <path to crawler dir> <name of index file>
```
The first argument represents the path to the crawler directory where the querier will
search for URLs, and the second argument represents the path to the index file where
the corresponding crawler directory has been indexed. The querier will check if the
first argument is valid and will terminate if not. However, it will not check if the
index file is valid. 

## Assumptions
Here are some of the assumptions that my `querier` makes, the user should be aware of these
before running the program, as any inputs/functions outside of the scope of these assumptions
will result in undefined behavior:
- The querier always assumed that the argument index file (if there is one) is in the valid
indexer format (see `indexer`).
- There are no more than 9999 files in the crawler directory.
- A directory is created by the crawler if it contains the hidden file `.crawler`.
- All of the files referenced in the `index` file actually exist in the given crawler 
directory.

## References
Please note that I referenced the lecture notes as well as the activities that we did 
in class extensively (specifically the set intersection and union activity). These are 
given here:
```
https://www.cs.dartmouth.edu/~cs50/
```
