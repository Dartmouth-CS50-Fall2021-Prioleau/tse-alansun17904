# Lab 5

### Delivery (20%)

 * 5/5 points: proper use of git (submitted via git, with the right tag)

 * 3/3 points: Follows the required file structures without extra data files, .o files, backup files, etc...

 * 5/5 points: Makefiles provided that can build all targets and includes a 'clean' target.      
      * Excellent clean target by the way! Good choices on which files to remove 

 * 5/5 points: "make" compiles properly with correct flags, produces 'indexer';

 * 2/2 points: "make clean" does what it should


### Documentation & Testing (22%)

 * 4/4 points: excellent README.md! Very thorough

 * 5/6 points: well-written IMPLEMENTATION.md, but did not include function headings and descriptions (-1)

 * 4/4 points: well-written testing.sh summarizing representative test cases. Very nice.

 * 2/2 points: documented assumptions in README.md

 * 6/6 points: source files and scripts have relevant and necessary comments

### Code Quality (20%)
  * 9/10 points: Well-structured and easy to follow; very readable, with meaningful comments. cohesive and concise.

    * A little comment heavy (big blocks of text should be split up or put in DESIGN or IMPLEMENTATION documents). Could be much more concise and better formatted. (-1)

  * 10/10 points: Follows [CS50 style](http://www.cs.dartmouth.edu/~cs50/Resources/CodingStyle.html);



### Functionality (38%)

   * 12/12 points: Testing bad inputs

   * 21/21 points: Testing index-creating inputs

   * 2/5 points: Memory leakage in most test cases (shown below from testing.sh) (-3)
```
==10066== HEAP SUMMARY:
==10066==     in use at exit: 45,951 bytes in 730 blocks
==10066==   total heap usage: 2,508 allocs, 1,778 frees, 114,751 bytes allocated
==10066== 
==10066== LEAK SUMMARY:
==10066==    definitely lost: 106 bytes in 3 blocks
==10066==    indirectly lost: 0 bytes in 0 blocks
==10066==      possibly lost: 0 bytes in 0 blocks
==10066==    still reachable: 45,845 bytes in 727 blocks
==10066==         suppressed: 0 bytes in 0 blocks
==10066== Rerun with --leak-check=full to see details of leaked memory
```


# Overall: 95/100
