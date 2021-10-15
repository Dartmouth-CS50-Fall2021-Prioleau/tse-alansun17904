#!/bin/bash
# Alan Sun
# CS50, Fall 2021, Tiny Search Engine: Crawler
# Crawler Tester (testing.sh)
#
# This is a testing program for the crawler. It will run the program many
# times with different arguments, including edge cases. The program at the
# start will set up each test by setting up the appropriate directories that
# the test program will require. It is up to the tester to actually inspect
# the outputs of the test program to see if crawler is valid.

if [[ -v MEM_CHECK ]]; then 
        mem = "valgrind"
else
        mem = ""
fi 

echo $mem

# Test Invalid Arguments
mkdir test1
valgrind ./crawler.x https://www.google.com/ test1 0
valgrind ./crawler.x http://cs50tse.cs.dartmouth.edu/tse/ test1 -1
valgrind ./crawler.x http://cs50tse.cs.dartmouth.edu/tse/ nonexistent 0
valgrind ./crawler.x http://cs50tse.cs.dartmouth.edu/tse/ nonexistent -1
rm -r test1

# Test Root Only
# mkdir test1
# valgrind ./crawler.x http://cs50tse.cs.dartmouth.edu/tse/ test1 0

# Test Non-Internal URL on Multi-Depth
mkdir test2
valgrind --leak-check=full ./crawler.x http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ test2 1 
mkdir test3
valgrind ./crawler.x \
        http://cs50tse.cs.dartmouth.edu/tse-output/letters-depth-0/1 test3 4
