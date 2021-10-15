# Common
This directory and all of its programs in it serve as a library for some 
utility and helper functions for tiny search engine. We now proceed to
briefly describe each function and its use case. However, please note
that much of the detail is presented in the actual source files themselves.

## Usage
To start, run the following command.
```bash
$ make
```
This will initialize the library, compiling all of the files and organize
them into a static library called `common.a`. Later on, when make-ing other files, this can be included as one of the dependencies and linked with the other 
necessary object files. 
**Please note that if changes are made to any of the component source files
that that specific file needs to be compiled again, and the library will
need to be re-linked. Otherwise, its API endpoints may not be updated.**

## Functionalities
`pagedir.c` contains utility functionality for checking whether a given
directory is writable. 

### `pagedir`
Now, we describe the functions within the `pagedir` module. 

##### `is_dir_writable`
Checks whether a given directory is writable. If it is then true is outputted, otherwise, false is returned.
```C
bool is_dir_writable(char *path);
```
