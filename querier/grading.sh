#!/usr/bin/env bash
#
# Grading tests for CS50 Lab 6: Querier
# Usage: ./grading.sh
# Author: Amittai Siavava

DATA_DIR=/thayerfs


# build querier
make

echo -e "\nInvalid tests...\n"
### Small test cases (12 = 1 * 12, 1 point for each)
#### T1: check # of arguments
./querier $DATA_DIR/letters-depth-6/ DATA_DIR/letters-index-6 data

#### T2: check crawler directory
./querier ../common $DATA_DIR/letters-index-6

#### T3: check existence of index file
./querier $DATA_DIR/letters-depth-6/ ../index

### Syntax errors in query input (1 point each)
###Run the querier with any folder names, e.g

echo -e "\nTEST 0\n"
./querier $DATA_DIR/letters-depth-6/ $DATA_DIR/letters-index-6 <<EOF
and Dartmouth
or Dartmouth
Dartmouth or
Dartmouth and
MIT and or Dartmouth
Boston or and Hanover
computer science 60
I love computer science!
Tiny Search-engine
EOF

# read -rp "Press enter to continue..."

### Big test cases (18 = 2 * 9, 2 points each, focus on ranking and whether the result is complete)
#### T13-T21

echo -e "\nTEST 1\n"
./querier $DATA_DIR/toscrape-depth-1/ $DATA_DIR/toscrape-index-1 <<EOF
years
book or story
book and history
art price
music band or painting
modern music or classical poem
political nonfiction or modern art or humor story
eat fat get thin or the time keeper or the black maria
paris kitchen recipes baking or white cat bear whale or best reviews
EOF

# function to run a query and check the output
# Usage: run <data-source> <index-file> <fuzzquery? 0 or 1> <seed-value>
function run() {

  echo -e "\n\nRunning $*..."
  if [[ -t 1 ]]; then
    read -rp "Press enter to continue..."
  fi

  if ! (( $# < 6 )); then
    echo "Usage: run <data-source> <index-file> <fuzzquery? 0 or 1> <valgrind? 0 or 1> <seed-value>"
    return 1
  fi

  if (( $4 == 1 )); then
    valgrind="valgrind --leak-check=full --show-leak-kinds=all"
  else
    valgrind=""
  fi

  if (( $3 == 1 )); then

    queries=$(./fuzzquery "$DATA_DIR/$2" 5 "$5")

    echo -e "Queries: \n$queries\n\n"

    $valgrind ./querier "$DATA_DIR/$1" "$DATA_DIR/$2" <<< "$queries"
  else
    $valgrind ./querier "$DATA_DIR/$1" "$DATA_DIR/$2"
  fi

  return 0
}




# fuzz testing

run letters-depth-0 letters-index-0 1 0 199009

# read -rp "Press enter to continue..."
run letters-depth-1 letters-index-1 1 0 199090

# read -rp "Press enter to continue..."
run letters-depth-2 letters-index-2 1 0 199099

# read -rp "Press enter to continue..."
run letters-depth-3 letters-index-3 1 0 199900

# read -rp "Press enter to continue..."
run letters-depth-4 letters-index-4 1 0 199909

# read -rp "Press enter to continue..."
run letters-depth-5 letters-index-5 1 0 199990

# read -rp "Press enter to continue..."
run letters-depth-6 letters-index-6 1 0 199999

# read -rp "Press enter to continue..."
run toscrape-depth-1 toscrape-index-1 1 199000

# read -rp "Press enter to continue..."
run wikipedia-depth-1 wikipedia-index-1 1 199001



run letters-depth-0 letters-index-0 1 1 199009

# read -rp "Press enter to continue..."
run letters-depth-1 letters-index-1 1 1 199090

# read -rp "Press enter to continue..."
run letters-depth-2 letters-index-2 1 1 199099

# read -rp "Press enter to continue..."
run letters-depth-3 letters-index-3 1 1 199900

# read -rp "Press enter to continue..."
run letters-depth-4 letters-index-4 1 1 199909

# read -rp "Press enter to continue..."
run letters-depth-5 letters-index-5 1 1 199990

# read -rp "Press enter to continue..."
run letters-depth-6 letters-index-6 1 1 199999

# read -rp "Press enter to continue..."
run toscrape-depth-1 toscrape-index-1 1 1199000

# read -rp "Press enter to continue..."
run wikipedia-depth-1 wikipedia-index-1 1 1 199001
