# CS50 TSE: Indexer Design Spec
Here we describe the design of the indexer: the data structure that was designed for
this part of the project as well as the general logic of the program.

## Data Structure: Inverted Index
The indexer will need a data structure called the inverted index. This is a data structure
that combines both the hashtable data structure and the counters data structure. We describe 
this in full here.

The goal of the indexer is to keep track of keywords across all of the crawled documents. 
This will be incredibly useful later on when these key words will be used to assist in querying.
Thus, we want to store a collection of significant words across all documents, and for each
word we want to know in which documents this word was found and how many times it was 
found in each of these documents. 

We accomplish this design by coupling a hashtable with a counter. The hashtable will have its
keys be words, and its items be counters (whose keys will be document ids and items will be counts 
of the number of times this word has appeared in this target document).

A diagram of this data structure is illustrated in `index.c`, and is copied here:
```
|-----|   |====|    |------------|    |------------|    |------------| 
|word1| : |head| -> |id1 : count1| -> |id2 : count2| -> |id3 : count3|
|-----|   |====|    |------------|    |------------|    |------------|
   |
   v
|-----|   |====|    |------------|    |------------|    |------------| 
|word2| : |head| -> |id1 : count4| -> |id2 : count5| -> |id3 : count6|
|-----|   |====|    |------------|    |------------|    |------------|
   |
   v
|-----|   |====|    |------------|    |------------|    |------------| 
|word3| : |head| -> |id1 : count7| -> |id2 : count8| -> |id3 : count9|
|-----|   |====|    |------------|    |------------|    |------------|
 
```
The `head` represents the head of the counter which is really just a linked list.

## Indexer Procedure
This subsection describes the general procedure of the indexer. 

1. Reads arguments to determine validity.
2. Initialize index data structure
3. Go through each file in the given crawler directory, read its contents and add it to the 
index.
4. Save the contents of the index
5. Clean up the data structure.

## Indextest Procedure
This subsection describes the general procedure of the indextest program, which reads a file
created by the indexer loads it in an index data structure and then outputs this to a 
new file.

1. Read arguments to determine validity.
2. Initialize index struct.
3. Read the index file store its words and counts into the newly created struct.
4. Export the contents of this new struct to the given new file.
5. Clean up structure.
