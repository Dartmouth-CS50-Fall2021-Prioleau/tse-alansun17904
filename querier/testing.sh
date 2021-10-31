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
echo "for" >> test2_basic
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

# Simple chained ANDs and ORs with only one document
touch test4
# basket: 1 20; products: 1 3; erotica: 1 1; fiction: 1 6; add 1 21
# (20 or 1 or 21) = matches 1 with 42
echo "basket or products and erotica and fiction or add" >> test4
# 1 and 0 or 1 = matches 1 with 1
echo "stock and nonexistent or christian and cultural and soumission" >> test4
# 1 or 6 or 1 and 1 or 1 or 1 = 7 or 2 or 1 or 1 = matches 1 with 11
echo "little or fiction or academic and contemporary or fantasy or objects" >> test4
# matches 1 with 2 
echo "free and light and assigned and precious or basket and search and stories" >> test4
$VALGRIND ./querier $DIR/toscrape-depth-0 $DIR/toscrape-index-0 < test4
rm -f test4

# Simple chained ANDs and ORs with different case, should be same output as above.
touch test5
# basket: 1 20; products: 1 3; erotica: 1 1; fiction: 1 6; add 1 21
# (20 or 1 or 21) = matches 1 with 42
echo "basKeT or ProductS and erOticA and fictIon or adD" >> test5
# 1 and 0 or 1 = matches 1 with 1
echo "stocK aNd NoneXisTent or ChrIsTiaN And CuLTurAl And SouMIssion" >> test5
# 1 or 6 or 1 and 1 or 1 or 1 = 7 or 2 or 1 or 1 = matches 1 with 11
echo "littlE or fictioN or AcademiC and conTemporAry oR FanTaSy or obJects" >> test5
# matches 1 with 2 
echo "free aNd Light and Assigned and precious or basket and search and stories" >> test5
$VALGRIND ./querier $DIR/toscrape-depth-0 $DIR/toscrape-index-0 < test5
rm -f test5

# Blackbox testing with fuzzquery.c
./fuzzquery $DIR/wikipedia-index-2 20 0 | $VALGRIND ./querier $DIR/wikipedia-depth-2 $DIR/wikipedia-index-2
