#!/bin/bash
# Alan Sun
# CS50, Fall 2021, Tiny Search Engine
# Tester for Indexer (indexer/testing.sh)
# 
# This script acts as a tester for indexer. For testing cases that are valid,
# the script does not do test clean up (deleting the outputted files). In
# these cases, the user testing the code must manually visually inspect
# the code to ensure that the outputs of the tests are correct. 
# The details about the tests being run here are explained in the TESTING.md
# file. Additionally, each test will have a header that briefly explain 
# what the test is and its expected result. 

OUTPUT_DIR="/thayerfs/courses/21fall/cosc050.02/workspace/cs50tse/tse-output"

# If the user provides a CHECK_MEMORY argument, then the program will run
# valgrind on all the subsequent commands.

if [ ! -z "$1" ]; then
  if [ $1 == "CHECK_MEMORY" ]; then
    VALGRIND="valgrind --leak-check=full --show-leak-kinds=all" 
    echo The following tests will check for memory leaks.
  fi 
else
  VALGRIND=""
  echo The following tests will run without checking for memory leaks.
fi

# Delete all old test output files and recompile the program.
make clean
make 

# Function for checking if two outputted index files are the same.
# Two args, the files that are being checked for equivalence.
# The first is the outputted file and the second is the expected file.
check_index_equal() {
  gawk -f indexsort.awk $1 > "$1".sorted
  gawk -f indexsort.awk $2 > testout.expected
  diff "$1".sorted testout.expected
  rm -f "$1".sorted testout.expected
}

# The following are the testable directories for indexer:
# cs50-index-0
# index.html
# letters-depth-0
# letters-depth-1
# letters-depth-2
# letters-depth-3
# letters-depth-4
# letters-depth-5
# letters-depth-6
# toscrape-depth-0
# toscrape-depth-1
# toscrape-depth-2
# wikipedia-depth-0
# wikipedia-depth-1
# wikipedia-depth-2

# Testing Invalid Number of Arguments
$VALGRIND ./indexer  
$VALGRIND ./indexer 0
$VALGRIND ./indexer hello
$VALGRIND ./indexer hello there are four
$VALGRIND ./indexer hello there are

# Testing Invalid Crawler Directory: Valid Remaning Arguments
mkdir test1  # test setup
$VALGRIND ./indexer test1 index
rm -rf test1  # cleanup

# Testing valid directory with no files, should result in empty file.
mkdir test2  # test setup
touch test2/.crawler
$VALGRIND ./indexer test2 test2.output
rm -rf test2

# Testing valid directory with only one file. 
$VALGRIND ./indexer $OUTPUT_DIR/letters-depth-0 test3.output
check_index_equal test3.output $OUTPUT_DIR/letters-index-0

# Testing valid directory with many multiple files. (Letters)
$VALGRIND ./indexer $OUTPUT_DIR/letters-depth-1/ test4.output
check_index_equal test4.output $OUTPUT_DIR/letters-index-1

$VALGRIND ./indexer $OUTPUT_DIR/letters-depth-2/ test5.output
check_index_equal test5.output $OUTPUT_DIR/letters-index-2

$VALGRIND ./indexer $OUTPUT_DIR/letters-depth-3/ test6.output
check_index_equal test6.output $OUTPUT_DIR/letters-index-3

$VALGRIND ./indexer $OUTPUT_DIR/letters-depth-4/ test7.output
check_index_equal test7.output $OUTPUT_DIR/letters-index-4

$VALGRIND ./indexer $OUTPUT_DIR/letters-depth-5/ test8.output
check_index_equal test8.output $OUTPUT_DIR/letters-index-5

# Testing valid directory with many multiple files. (ToScrape)
$VALGRIND ./indexer $OUTPUT_DIR/toscrape-depth-0/ test9.output
check_index_equal test9.output $OUTPUT_DIR/toscrape-index-0

$VALGRIND ./indexer $OUTPUT_DIR/toscrape-depth-1/ test10.output
check_index_equal test10.output $OUTPUT_DIR/toscrape-index-1

$VALGRIND ./indexer $OUTPUT_DIR/toscrape-depth-2/ test11.output
check_index_equal test11.output $OUTPUT_DIR/toscrape-index-2

# Testing valid directory with many multiple files. (Wikipedia)
$VALGRIND ./indexer $OUTPUT_DIR/wikipedia-depth-0/ test12.output
check_index_equal test12.output $OUTPUT_DIR/wikipedia-index-0

$VALGRIND ./indexer $OUTPUT_DIR/wikipedia-depth-1/ test13.output
check_index_equal test13.output $OUTPUT_DIR/wikipedia-index-1

$VALGRIND ./indexer $OUTPUT_DIR/wikipedia-depth-2/ test14.output
check_index_equal test14.output $OUTPUT_DIR/wikipedia-index-2




