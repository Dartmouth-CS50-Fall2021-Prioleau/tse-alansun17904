# Functions to-Write
Here contains a list of functions that need to be written for the querier and a brief 
description of each one. 

## Common
- [] Tokenizer - checks if a query is a valid query (must have all alphabetic
characters; read until a new line; only whitespace other than \n at the end 
must be spaces), tokenizes words (separate the query into many substrings 
where each substring is a word that contains no whitespace).
```
<begin function>

  we first need to figure out how much memory to allocate (how many pointers
  to characters to allocate); same time make all characters lowercase.

  store the first non-whitespace char into the array
  continue until there is a white space
  continue until the first non-whitespace character
  store the first non-whitespace char into the array
  ...
  ...

  return this tokenized array, it needs to be freed later on

<end function>
```
- [] Set Intersections - gets the intersections of two sets, if an element
exists in both sets then we simply take the min of the two elements. 
```

```

- [] Set Unions - gets the union of two sets, if an element exists in both sets
then we simply add the two values together. 
```

```

## Querier 
- [] Validate arguments - checks if the query is semantically valid (there must 
be no starting or trailing logical arguments; there cannot be consecutive 
logical arguments;