#!/bin/bash
# Alan Sun
# CS50, Fall 2021, Tiny Search Engine
# Tester for Querier (querier/testing.sh)
# 
# This script acts as a tester for querier. For testing cases that are valid,
# the script does not do test clean up (deleting the outputted files). In
# these cases, the user testing the code must manually visually inspect
# the code to ensure that the outputs of the tests are correct. 
# The details about the tests being run here are explained in the TESTING.md
# file. Additionally, each test will have a header that briefly explain 
# what the test is and its expected result. 

# If the user provides a CHECK_MEMORY argument, then the program will run
# valgrind on all the subsequent commands.

DIR="/thayerfs/courses/21fall/cosc050.02/workspace/cs50tse/tse-output"

if [ ! -z "$1" ]; then
  if [ $1 == "CHECK_MEMORY" ]; then
    VALGRIND="valgrind --leak-check=full --show-leak-kinds=all" 
    echo The following tests will check for memory leaks.
  fi 
else
  VALGRIND=""
  echo The following tests will run without checking for memory leaks.
fi

# Testing invalid commands 
touch test1_invalids
echo "invalid!" >> test1_invalids 
echo "and this is not a valid query" >> test1_invalids
echo "or this is not right" >> test1_invalids
echo "  4892 " >> test1_invalids
echo "  this looks right and or is not" >> test1_invalids
echo " hello this is invalid or" >> test1_invalids
echo " hello and this is or and this is and" >> test1_invalids
$VALGRIND ./querier $DIR/letters-depth-6 $DIR/letters-index-6 < test1_invalids
rm -f test1_invalids

# Basic commands, ANDs only
touch test2_basic
echo "dartmouth college" >> test2_basic
echo "algorithms" >> test2_basic
echo "  tse playground for " >> test2_basic
echo " tse " >> test2_basic
echo " algorithm and for " >> test2_basic
echo " computational and biology " >> test2_basic
$VALGRIND ./querier $DIR/letters-depth-6 $DIR/letters-index-6 < test2_basic 
rm -f test2_basic

# Basic commands, ORs only
touch test3_basic
echo "dartmouth or college" >> test3_basic
echo "algorithms or computational or biology " >> test3_basic
echo "     tse or playground " >> test3_basic
echo "     biology or tse or computational" >> test3_basic
echo "    college or tse or nonexistent or algorithms " >> test3_basic
$VALGRIND ./querier $DIR/letters-depth-6 $DIR/letters-index-6 < test3_basic 
rm -f test3_basic

