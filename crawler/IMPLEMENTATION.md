# CS50 TSE: Crawler Implementation Spec
This is the implementation spec for Crawler. It contains a basic 
description of the program: how to run it, its assumptions. It also
contains some of my thought processes for the implementation of these
functionalities: pseuodo code for the crawler.

## Purpose
As stated in the `README.md` in this directory, for a search engine to work
it must have an idea of what every single webpage looks like. This way when
the user searches something, the search engine will know exactly where to 
look. This relies on have a "crawler" a program that will be able to "crawl"
all of the webpages in its vicinity and save these webpages for later use
by the indexer and the query. Here, we implement this functionality. 

## Usage and Functionality
To run the program, the following commands must be run beforehand:
```bash
$ make
$ ./crawler ROOT_URL DATA_DIR MAX_DEPTH
```
We note here that if the libraries `common.a`, `libcs50.a` are uncompiled,
then they also need to be compiled, linked, and archived. This can be done
by going into each of these directories and running:
```bash
$ make
```

### Inputs
The crawler program requires three arguments to run:
- `ROOT_URL` a url where the crawler starts. We note that this URL must be 
an internal one: it must be in the domain `http://cs50tse.cs.dartmouth.edu/`
this is done to avoid flooding unsuspecting websites with a huge amount of
bot traffice.
- `DATA_DIR` a writable directory where the data that the crawler has "crawled" will be stored. The crawler will write to this directory all of the websites
it has reached, such that each website will have its own file with its HTML 
content.
- `MAX_DEPTH` an integer that denotes the depth of the crawl. A crawl with
depth 0 means that only the current webpage is being downloaded. A crawl of
depth 1 means that the root and all of its neighbors are being crawled...

### Outputs 
If the program runs successfully, with a non-zero exit status, then it will
output nothing to `stdout`. Instead it will export all of the pages it has
crawled into the given directory. These files are given unique ordinal 
names. The filenames/IDs are not assigned in a well-defined order. Rather,
they are just assigned based on the order the crawler sees them. 
Additionally, the contents of the exported files are formatted such that
the first line displays the url from which the HTML contents was taken
from. The second line displays the depth of this website relative to the root.
Lastly, the HTML contents of the webpage follows. An example of this is
shown below:
```
<filename: 1>
http://cs50tse.cs.dartmouth.edu/
0
<head><title>Index of /</title></head>
<body>
<h1>Index of /</h1><hr><pre><a href="../">../</a>
<a href="tse/">tse/</a>                                               01-Feb-2020 04:10                   -
<a href="tse-output/">tse-output/</a>                                        01-Feb-2020 04:22                   -
</pre><hr></body>
```
### Pseudo-Code
This subsection describes my basic idea for implementing this program. It contains
some pseudo-code which may make it easier to understand for the reader. Please
note that in the pseudo-code I will be refering to the `bag` and `hashtable` 
datastructures in the `libcs50` library. A `bag` is simply defined as an 
unordered collection, its order of extraction is also undefined. On the otherhand,
a hashtable stores keys and values, the catch is that its keys must be unique.
With this we proceed to display the pseudo-code:
```
<start of program>

check if the arguments given are valid:
  there must be exactly three provided arguments
  the given url must be a valid url AND it must also be an internal one
  the given directory must be writable
  the given depth must be nonnegative

create hashtable of visited websites
create bag of websites to be visited

from this point all arguments are assumed to be valid
<start loop>

we begin crawling the root first
  export the content of the root into file id=1
  add the root to the hashtable denoting visited websites
  
  if the depth of root is less than max_depth:
    add all internal, unvisited links root is connected to to bag

  extract link from the bag and crawl that link
  continue until either the bag is empty or max_depth is reached

<end loop>

<end of program>
```

## Assumptions
- `MAX_DEPTH` must be provided as a valid integer input. Arguments that are
examples are invalid integer input are `20a` or `b4e4`. In such cases, the
behavior of the program is undefined and could also lead to errors.
- If a website's HTML could not be fetched at any given time it is ignored
and all of its neighbors will also be ignored. However, the program will
continue by crawling the remaining websites which conform to the given 
arguments.
- Suppose that memory allocation fails at any point, then the program will 
simply terminate without freeing any existing memory. If this happens, such
edge cases may yield memory leaks.

## Security and Privacy 
As acknowledged from the start, we are only crawling websites that are
specifically designed for this project. This will not only make the behavior
of the crawler more well defined and easier to test, but it also ensures
that we are not crawling random websites without permission. Please note that
the program will access the given directory on the user's machine and export
files to this directory.

## Error Handling and Recovery
- On any errors associated with the given arguments, the program will print a
message to `stderr` and it will terminate.
- If there are any errors with memory allocation, the program will print a 
message to `stderr` and it will terminate immediately. This edge case may 
result in memory leaks.

## Persistent Storage
As stated in the introduction as well as usage. The function will write out 
the contents of the HTML pages into the given directory. These pages will then
be continually used for the other parts of the project: indexer and query.
