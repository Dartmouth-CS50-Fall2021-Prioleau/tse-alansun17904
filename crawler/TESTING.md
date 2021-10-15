# Unit Testing of Crawler
 
This document explains exactly how crawler is being unit tested. We start by
explaining how to run the tests, and then specifically the purpose and
rationale behind each test.

## Usage

There are two modes in which testing can be run: with memory checking or without.
I've found that when the tests are run with `valgrind` the waiting time becomes 
extremely long. Thus, for purely functional tests, it may be best to run the tests
without memory checking. Then, for memory tests, the testing file would be run with
`valgrind`. 

The program is tested using another shell tester program. At the inception of each 
test a directory is created for that test. This directory will be named `test<id>`,
where `<id>` represents the identifying test number. The exported files from the program
for that specific test will reside in that folder. 

Once testing is complete, the programmer can check each subdirectory of the tests to 
see if the outputted files are indeed correct. Once they have finished this, or decided
that these files are no longer useful they can clean the directory by calling 
```bash
make clean
```
This will remove all testing output subdirectories as well as the `testing.out` file, 
if there is one.

The author recommends running the following sequence of commands to test the program
without memory checking:
```bash
make clean
./testing.sh  &> testing.out
```
Otherwise, if the user desires memory to be checked, simply pass in `valgrind` as one of
the program arguments into `testing.sh`:
```bash
make clean
./testing.sh valgrind &> testing.out
```

## Test Cases
- Test invalid arguments to program. 
  - An invalid number of arguments: too many or too little.
  - The root link not being an internal link.
  - The test folder is not writable / does not exist.
  - The max depth given is negative.
  - *Note again that we assume the given max depth is numeric, giving a non-numeric
    input for this argument will result in undefined behavior*
- Test the root only
  - Only the page of the root.
- Test deep page: this refers to testing a page that has many links attached to it
  and even more links attached to its neighbors. This is to see if the same 
  working functionality in the previous tests is scalable.
- Test internal link pointed to outside: a page that has many links but some of them
  are pointed towards external links. We note here that this test case is tested 
  at the same time the previous test case is being tested.
- Test max depth > page depth: this refers to testing the program when the given
  max depth is greater than the maximum depth of the page. The program will need
  to not freak out.
- Test cyclic links: testing whether the program can handle a page whose neighbors
  possibly several links away is linked back to itself. This test case fails if the
  program starts entering an infinite loop.
- Test repeated links: test whether the program can handle a mass array of links. 
  This checkes to see if the program is able to go the wide breadth. Rather than
  deep.

