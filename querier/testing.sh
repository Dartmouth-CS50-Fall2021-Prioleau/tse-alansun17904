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
$VALGRIND ./querier < test1_invalids
rm -f test1_invalids
