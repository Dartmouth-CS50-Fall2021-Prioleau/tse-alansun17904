# Testing
This file describes how I tested my querier.
## Usage
To run the tests I have created, one can run the following commands:
```
make clean
make 
make test
```
This will create a new `testing.out` file that contains the output of all the
tests described herein. Please note that the testing program will take a 
decently long time, especially for the last black box test with fuzzquery on
wikipedia. The result of these tests can be manually inspected by the 
tester.

To run the test with valgrind, the tester can simply replace `test` with 
`valgrind` as such:
```
make valgrind
```

## Tests
Here, we will describe the tests for the querier.
- Test invalid cases:
  - Invalid symbols in the query (numbers and non-letter symbols).
  - Leading and trailing logical operators.
  - Logical operators that are not sandwiched by non-operators.
- Test basic ANDs only:
  - Only words and no logical operators.
  - Words chained with ANDs
  - Non-existent words
- Test basic ORs only:
  - Non-existent words with words connected with ORs
  - Words exclusively chained with ORs.
- Test simple chained ANDs and ORs with only one document
  - Chained nonexistent words with existing words
  - One OR chaining together two statements with many ANDs
  - Chained words with ORs and ANDs --- no nonexisting words
- Test same as the previous with varied casing.
- Black box testing with many documents on wikipedia
  - The words and commands are generated through `fuzzquery`.
  - 20 randomly generated words.
