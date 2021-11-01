# Design Specification for Querier
Here, we describe the design of the querier, the data structures design, as well as 
the general logic of the program.

## UI
When the querier is run as is described in the `README.md`, it will prompt the user
with the following:
```
Query:
```
The user will type in a query on the same line. When a new line is given the querier
will process the previous line (as one query) and output the results to the user.
If no results were found, then the querier will print out
```
Matches 0 documents.
```
Otherwise, it will display a list of the documents found relating to the query. The
querier will organize this information into three columns: score (how closely the
webpage matches the query), doc (the ID of the page assigned by the crawler), and
url (the real URL of the page). Note that these pages are sorted in no particular
order. A sample output is shown below:
```
Query: liar OR cwa autoconfirmed southernspaces AND clinger OR schwaz 
Matches 10 documents (ranked):
score    2 doc 1546: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Robotics.html
score    1 doc 1422: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Mining_engineering.html
score    1 doc 1666: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Algorithm.html
score    1 doc 1639: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Undecidable_problem.html
score    1 doc 1108: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/W._W.html
score    1 doc  370: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Shonda_Rhimes.html
score    1 doc  368: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Scandal_(TV_series).html
score    2 doc  355: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/24_(TV_series).html
score    2 doc  351: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Friday_Night_Lights_(TV_series).html
score    1 doc  284: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_pong.html
-----------------------------------------------
```

## Pseudocode (High Level)
This subsection describes some of the pseudocode that was used to implement the querier. More detailed,
language specific pseudocode is presented in `IMPELEMENTATION.md`. 
1. Parse arguments 
2. Tokenize arguments, check if there are invalid characters.
3. Take care of the `AND` sequences first --- evaluate these first --- 
then do accumulation with the `OR` operators. 
  1. For each word, get the corresponding counter from the index struct, until we hit
  and `OR` logical operator. We store these in a set.
  2. Now, we have list of these counters, perform the intersection of these counters,
  where if the same document ID exists in two counters, then we perform `MIN`.
  3. Now, the `AND`s have all been processed. So, we are left with only `OR` operators.
  Thus, we perform unions on the remaining counters, where if the same document ID 
  exists in two counters, then we perform `SUM`.
4. There should only exists a set with one item: the counter that has the 
aggregate information. Now we need to sort this counter. First call counters iterate
to figure out how many items, and then create a new array of `int`s. 
Then, perform insertion sort. 
5. Using the array of ints, get the files from the crawler directory and URLs 
in order.

## Data Structure: Tokenizer
The querier will need a tokenizer data structure. This is a simple wrapper for an
array of strings (representing the tokenized words), containing both the array 
itself and the number of items in the array. 

Given a query string, the querier will first check if any of the characters are 
non-alphabetic. If so, thie query will end and an error message will be printed.
The querier will then tokenize the string. 

The same string will have its spaces replaced with `\0` characters. Each index 
in the array will then point to the first letter that proceeds a space. This is 
illustrated in the diagram below:
```
tokenize {

  word =====> 0 ---------> f
                           i
                           r
                           s
                           t
                           \0
                           \0
              1 ---------> q
                           u
                           e
                           r
                           y
                           \0
              2 ---------> h
                           i
                           \0
  num ======> 3
}

```
where `word` is the array of strings that represent the individual words, and `num`
represents the number of words.

## Testing Plan
The datastructures and individual helper modules are unit-tested in their own files.
These can be run by turning on the `-DUNIT_TEST` flag during compilation. On the
other hand, the `querier` is tested through integration testing. This is done through
some basic white-box testing as well as black-box testing using `fuzzquery.c`. These
integration tests can be found in `testing.sh`. The user can easily run these tests by
calling either 
```
make test
make valgrind
```
The former will run the tests without valgrind, while the latter will.

