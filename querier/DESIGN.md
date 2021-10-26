# Design Specification for Querier

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

Design Spec TODO:
- UI
- Functional Modules
- Pseudocode (High Level)
- Data Structures
- Testing Plan

Implementation Spec TODO:
- Specific Function Prototypes
  - Brief description of what it does
- List of helper functions

word1 word2 and word3 word4 or word5 and word6 or word7 and word8

3 structures:
- the intersection of documents that contain word1, word2, word3, word4
- word5 and word6
- word7 and word8

Each word would be a counter that stores the count associated with word for
each document. 
`AND` = Intersection between two counters, and if the same document ID exists
in two counters, then we perform `min`.
`OR` = Union between two counters, and if the same document ID exists in two
counters, then we add then counts togehter.


