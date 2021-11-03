# Lab5 rubric
**99/100**

## (9/10) Overall
  * Proper git tag `lab5submit`.   
  * git commit history - logical groupings of changes in each commit, and meaningful commit messages.
  * tse directory and subdirectories `indexer` and `common` exist, and `indexer` must include appropriate `.gitignore`.  
  * no executable files, object files, core dump files, editor backup files, or extraneous files existing in any directory.
    *  -1 no need to track common/common.a and libcs50/libcs50.a since they are built with respective Makefile


## (10) Makefiles
  * `tse/Makefile` was provided; it's ok if they needed to comment-out the lines about querier.
  * `indexer/Makefile`
	  * has proper targets to build `indexer` and `indextest`
	  * uses CS50 standard CC flags.
	  * has an appropriate `clean` rule.
	  * has an appropriate `test` rule.
  * directory `common/` exists; `common/Makefile`
	  * has proper targets to build `common.a`
	  * uses CS50 standard CC flags.
	  * has an appropriate `clean` rule.
  * Builds `indexer` and `indextest` with no compilation errors or warnings *when compiled from the top-level directory*. -1 point per compile warning (-3 at most).


## (10) Documentation
The solution provides examples. Their files must be in Markdown although they won't need much Markdown syntax.  *Watch for messages from student to grader,* which may be in any of the `README.md` files.

In `tse/indexer/`,

  * `README.md`, how to build `indexer`.
  * `DESIGN.md`, the abstract design of the indexer with no language-specific details.
  * `IMPLEMENTATION.md`: describes the implementation of `indexer`.
  * `testing.sh`, which should have good comments.


## (10) Testing
They should have done a thorough job of testing their indexer, as documented by the test cases *and* comments. They don't have to have tested every case you/we thought to test, but they are expected to have put real effort into testing normal and edge cases.

  * `tse/indexer/testing.sh` should have good test cases and good comments
  * `tse/indexer/testing.out` should be the output of `make test &> testing.out`

## (20) Coding Style
  * [CS50 C program coding style](http://www.cs.dartmouth.edu/~cs50/Resources/CodingStyle.html).
  * Their indexer-specific code *should* be isolated from the data strucure code.
  * Their indexer code should not 'peek' inside opaque types.
  * They *need not* use my [*parse_args*]({{site.lectures}}/parse_args) approach, but *should* have a clean, well-organized code for parsing and validating command-line args that is not interwoven with the overall program flow & logic.
  * They *may* use `assertp()` or similar methods to exit when `malloc` fails or other sanity checks fail.
  * (-5) if the whole indexer is written in `main`


## (30) Functionality
  * Validation of command-line arguments in both `indexer` and `indextest`.
  * Correct outputs on a variety of test cases - compare with solution `indexer` and `indextest`.
  * Their `indextest` properly loads and saves the index and they've demonstrated (through testing) that the output index is equivalent to the input index.

## (10) Memory leaks, according to valgrind
check both `indexer` and `indextest`.

  * free of *memory errors*.
  * free of *lost memory blocks*.
  * free of *still reachable* memory blocks.
