# Grading and Commentary for CS50 Lab 6

## Student: Alan Sun

## Grader: Amittai

&nbsp;

### **Final Scores**

-------------------
|Problem|Score|
|:-------|------:|
|Delivery|10/10|
|Makefiles|12/12|
|Documentation|12/12|
|Testing|6/6|
|Code Quality|20/20|
|Functionality|23/30|
|Memory Usage|10/10|
|**TOTAL**|**93**|

&nbsp;

## (10/10) Delivery

* (3) Proper git tag `lab6submit`.
* (2) git commit history - logical groupings of changes in each commit, and meaningful commit messages.
* (3) Proper file structure: tse directory and subdirectories `querier` and `common` exist, and `querier` must include appropriate `.gitignore`.  
* (2) no executable files, object files, core dump files, editor backup files, or extraneous files existing in any directory.

## (12/12) Makefiles

* (+4) `querier/Makefile`.
  * (1) exists
  * (1) has proper target to build `querier`.
  * (1) uses standard CFLAGS (or with `c11` changed to `gnu11`).
  * (1) has an appropriate `clean` rule.
* (+4) directory `common/` exists and `common/Makefile`.
  * (1) exists.
  * (1) has proper targets to build `common.a`.
  * (1) uses CS50 standard CFLAGS.
  * (1) has an appropriate `clean` rule.
* (+4) Builds `querier` (and optionally `fuzzquery`) with no compilation errors or warnings *when compiled from the top-level directory*. -1 point per compile warning (-3 at most).

## (12/12) Documentation

~~-1 for each doc written with *no* Markdown syntax.~~

* (2) `README.md`, which says how to build `querier`.
* (4) `DESIGN.md`, which describes the abstract design of the querier with no language-specific details.
* (4) `IMPLEMENTATION.md`: describes the implementation of `querier`.

  > This is minor, but you shouldn't write pseudocode in prose! When I did Algorithms in spring there were penalties for writing pseudocode in prose. You can think of proper pseudocode as in between English and Python. Don't write actual code, but also don't write complex multi-line sentences explaining what to do if X or what else to do Y... Make it code-like.
  > Here's an example for one of your functions (the one that sorts keys and counts by the counts array).

  ```pseudocode
  sort(Counters[keys, counts])
      keys_copy[1..n] <- keys
      counts_copy[1..n] <- counts

      for i <- 1..n: 
          m <- index of max value in counts_copy[i..n]
          swap counts_copy[i], counts_copy[m]
          swap keys_copy[i], keys_copy[m]
      return keys_copy
  ```

* (2) `testing.sh`, which may refer to testing results included in a separate file (e.g., testing.out, testing.txt)

## (6/6) Testing

* (5) `tse/querier/testing.sh` should have good test cases and good comments
* (1) `tse/querier/testing.out` should be the output of `make test &> testing.out`

## (20/20) Coding Style

* [CS50 C program coding style](http://www.cs.dartmouth.edu/~cs50/Resources/CodingStyle.html)
* Their querier-specific code should be isolated from the data structure code.
* Their querier code should not 'peek' inside opaque types.
* They *need not* use my [*parse_args*]({{site.lectures}}/parse_args) approach, but *should* have a clean, well-organized code for parsing and validating command-line args that is not interwoven with the overall program flow & logic.
* The code should be rigorous in checking for memory-allocation errors or file I/O methods. They are allowed to use `assertp()` or similar methods to exit when `malloc` or `fopen` or constructors (e.g., index_new()) fails or other sanity checks fail.
* Well-designed code structure
  * avoid deep nested loops or conditional blocks.
  * consistent indentation and spacing.
  * sufficient comments.

> Your `NormalizeWord` function does not need to return the word it's given -- since you normalize the word in place, and you never directly use the return value of the function, why not just make it a void function and document that it normalizes the word in place?
> On the other hand, your function returning a pointer to memory it didn't allocate creates a potential loophole for someone to free and re-use the same pointer -- and probably seg-fault the program.
> If curious, check out `C++`, it has specific extensions to `C` such as unique that do not allow aliasing, vs shared pointers. Also, classes (and `string`s) -- kinda a bridge between `C` and higher-level languages that accepts all `C` code. GNU has a C++ compiler, `g++`, so you shouldn't have trouble getting started since you know your way around the GNU eco-system already.
>
> Your tokenizer functionality is unneeded. The lecture notes discuss how to easily split a sentence into words by marching down the sentence, saving pointers to the first letters, and replacing sentences with `\0`. That is easier and *more efficient*, since you're not allocating space for all these different words, memory which you have to track and later free. You can free all words at once by freeing the block of memory containing the sentence. Suppose you had a query with 2000 words. That would be 2000 unneeded allocations! Sure, C is efficient and can run through all that in less than a second, but if performance is critical (a main reason people use C over Python and other nicer languages), then you probably want your program to be as efficient as possible.
>
> I've seen in multiple places structs being defined in the header files (so they're transparent to the user), but it's standard in CS50 to define structs in the source files and only declare them in the header files so they are opaque to outside scopes, which ensures no one tries doing funny stuff with your block of memory.
>
> Great work with the comprehensive comments in your code! Top-tier stuff.
>
> Good job making your testing script also run valgrind.
>
## (23/30) Functionality

Refer to the [published rubric](https://www.cs.dartmouth.edu/~cs50/Labs/Lab6/#grading) for the number of points to award for progressive levels of functionality.

However, take off points if

* ~~(-4) Improper validation of command-line arguments.~~
* ~~(-1) Cannot handle empty and blank queries, or mishandles EOF on stdin.~~
* ~~(-5) Incorrect output (or no error message) for invalid test queries. (Note that some "invalid" queries are valid for the lowest functionality level.)~~

### Note: I did 9 valid tests, x2 = 18. The remaining 12 points came from how the program handles invalid / boundary cases. See [grading-official-output](./grading-official-output) for official output using the professor's provided program, and [grading-output](./grading-output) for output using your program. For the specific tests, see [grading.sh](./grading.sh). Reach out if you have any questions about the tests, and *if you can prove that your program produces the correct feedback* then I'll return points for the specific tests you prove. (Also, for that case, reach out if you want access to the Prof's solution!)

#### Failed tests

##### 1. letters-depth-4 letters-index-4 **(-1 because most matched)**

**Expected:**

```text
Query: page or impugning fast or for 
Matches 8 documents (ranked):
score   2 doc   1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
score   1 doc   2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
score   1 doc   3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
score   1 doc   4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
score   1 doc   5: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/G.html
score   1 doc   6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/F.html
score   1 doc   7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
score   1 doc   8: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
-----------------------------------------------
```

Actual:

```text
Query: page OR impugning fast OR for 
Matches 8 documents (ranked):
score    2 doc    1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
score    2 doc    6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/F.html       (wrong)
score    1 doc    4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
score    1 doc    5: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/G.html
score    1 doc    7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
score    1 doc    8: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
score    1 doc    2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
score    1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
```

##### letters-depth-5 letters-index-5 **(-2)**

**Expected:**

```text
Query: cartwheeled breadth
No documents match.
```

**Actual:**

```text
Matches 1 documents (ranked):
score    1 doc    3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
-----------------------------------------------
```

##### letters-depth-0 letters-index-0 **(-2)**

**Expected:**

```text
Query: for home and vermouth event the 
No documents match.
-----------------------------------------------
```

**Actual:**

```text
Query: for home AND vermouth event the 
Matches 1 documents (ranked):
score    1 doc    1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
-----------------------------------------------
```

##### letters-depth-6 letters-index-6 **(-2)**

**Expected:**

```text
Query: home first algorithm or traversal and guatemala huffman 
No documents match.
-----------------------------------------------
```

**Actual:**

```text
Query: home first algorithm OR traversal AND Guatemala huffman 
Matches 1 documents (ranked):
score    1 doc    6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/H.html
-----------------------------------------------
```

## (10/10) Memory leaks, according to valgrind

See the solution's `querier/Makefile` for a method to test for memory leaks.

* (4) free of *memory errors*.
* (4) free of *lost memory blocks*.
* (2) free of *still reachable* memory blocks.

See [grading-output](./grading-output) for valgrind output.
