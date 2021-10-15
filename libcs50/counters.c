#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "counters.h"
#include "memory.h"

/* Alan Sun
 * CS50, Fall 2021, Lab 3
 * Counters (counters.c)
 *
 * Foreword: I acknowledge that this code is ported from the `bag.c` file given
 * to us at the start of the assignment.
 * This module implements a counter datastructure that is based on linked
 * lists. A counter is defined as a structure with keys and values. Both
 * keys and values are defined to be integers with the values representing
 * the "count" of the keys. The keys in the data structure are unique and
 * can be leveraged by the user to search for the values. The counter
 * structure is useful for keeping count of integers as it offers much less
 * overhead compared to the set. Please note that this structure is unordered.
 * Briefly, the functions and strcutres in this module define a
 * structure `counters_t` that is avaliable to the user which points to 
 * `countersnode` structures which are hidden to the user. There exists
 * seven functionalities that is avaliable to the user. These are described
 * briefly here; please refer to the function headers for a more detailed
 * description:
 *
 *	- `counters_new` creates a new counter and returns a pointer to it.
 *	- `counters_add` adds a new key to the counter and increments its counter.
 *	- `counters_get` gets the count of a given key.
 *	- `counters_set` sets the count of a given key.
 *	- `counters_print` prints the contents of the counter.
 *	- `counters_iterate` iterates through the counter with the given function.
 *	- `counters_delete` frees all the memory associated with the given counter.
 */

typedef struct countersnode {
	int key;                         // key
  int count;                       // count for given key 
  struct countersnode *next;       // link to next node
} countersnode_t;

typedef struct counters {
  struct countersnode *head;       // head of the list of items in counters
} counters_t;

static countersnode_t* countersnode_new(int key);

/* **********
 * The `counters_new` function creates an empty counter whose head points to
 * NULL. A pointer to this counter is returned to the user. Since memory is
 * dynamically allocated for this counter, it must be released when the user
 * is finished with the counter with `counters_delete(<name of counter>)`.
 * If for any case, memory cannot be allocated for this counter, then NULL
 * is returned.
 */
counters_t* counters_new(void)
{
  counters_t* counters = malloc(sizeof(counters_t));

  if (counters == NULL) {
    return NULL;              // error allocating counters
  } else {
    // initialize contents of counters structure
    counters->head = NULL;
    return counters;
  }
}

/* *********
 * The `counters_add` function takes in two arguments. The first, `counters`
 * represents a pointer to a counter structure defined above. The second, `key`
 * represents an integer that is a key to a counter in the counter. If the key
 * is present in the counter, then its value is incremented and the incremented
 * value is returned. Otherwise, if the given counter is NULL or if the key is
 * invalid, then 0 is returned. Additionally, if the key does not exist in the
 * counter, than a new "entry" (node) is created for this given key and its 
 * count is incremented to 1.
 */
int counters_add(counters_t* counters, const int key)
{
	countersnode_t* new;
	countersnode_t* curr;

  // We add a new node if the arguments are valid
  if (counters != NULL && key >= 0) {
		/* If the given key already exists, we increment the counter and return
		 * the value of the count. Otherwise, if the key does not exist, then we
		 * add a new node with that key into the list and increment its counter.
		 */
		curr = counters->head;
		while (curr != NULL) {
			// Check for key equality. 
			if (curr->key == key) {
				 // Pre-increment and return the key's count.
				 return ++(curr->count);
			}
			curr = curr->next;
		}
		// Node does not exist, so we create a new node with the given key.
		new = countersnode_new(key);
		if (new == NULL)
			return 0;
		// Add the new counter to the front of the linked-list.
		new->next = counters->head;
		counters->head = new;
		return new->count;
	}
	return 0;
}

/* **********
 * The `countersnode` function is a static function that creates helper
 * functionaltiies for the action of creating a new node. It takes in a key
 * and initalizes the count associated with this key to 1. The address that
 * it points to `next` is set to NULL.
 */
static countersnode_t* countersnode_new(int key)
{
  countersnode_t* node = malloc(sizeof(countersnode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
		node->key = key;
		node->count = 1;
		node->next = NULL;
    return node;
  }
}

/* **********
 * The `counters_get` function takes in two arguments: a pointer to a 
 * counter and an integer key. Similar to the `counters_add` function it
 * searches for this key in the counter, but instead of adding to it the 
 * `counters_get` function returns its associated count. If the an invalid
 * key, a key that does not exist, or a NULL-counter is given, then the
 * function will return 0;
 */
int counters_get(counters_t* counters, const int key)
{
	countersnode_t* curr;

  if (counters == NULL || key < 0) {
    return 0;              // bad counters
  } else if (counters->head == NULL) {
    return 0;              // counters is empty
  } else {
		/* We loop through the counters to find a key that matches the given key,
		 * if no such key is found then 0 is returned. Otherwise, the key's count 
		 * is returned.
		 */
		curr = counters->head;
		while (curr != NULL) {
			if (curr->key == key)
				return curr->count;
			else
				curr = curr->next;
		}
		return 0;
  }
}

/* **********
 * The `counters_set`function takes in three arguments: a pointer to a counter
 * an integer key, and an integer count. It searchers for the `key` in the
 * given `counters` and if found, it will change the key's associated count to
 * the given `count` value. If this is successful, then true is returned.
 * Otherwise, if the counter is NULL, key is invalid (< 0) or count is invalid
 * (< 0) then false is returned. If the key does not already exist in the
 * counter then and the given `count` is valid, then it creates a new counter
 * node for the given key and sets its associated count to `count`.
 */
bool counters_set(counters_t* counters, const int key, const int count)
{
	countersnode_t* curr; 
	// Check invalid counter, negative key, or negative count.
	if (counters == NULL || key < 0 || count < 0) {
		return false;
	} else if (counters_get(counters, key) == 0) {
		/* If the key is nonexistent, but the count is valid, then we first add a
		 * new node to the counter (just as a place holder), then change the value
		 * of its count.
		 */
		counters_add(counters, key);
	} 
	curr = counters->head;
	while (curr != NULL) {
		// Check for key equality.
		if (curr->key == key) {
			curr->count = count;
			return true;
		}
		curr = curr->next;
	}
	return false;
}


/* **********
 * The `counters_print` function takes in two arguments: a pointer to a counter
 * and a file pointer. It exports a textual representation of the given
 * counter to the file pointer. An example of this format is given below:
 *
 *	{key1=count1,key2=count2,key3=count3,}
 *
 * Note that if the given file pointer is null than nothing is printed.
 * Otherwise, if the given counter is null then "(null)" is exported to the 
 * given file pointer.
 */
void counters_print(counters_t* counters, FILE* fp) 
{
	// Check for invalid file pointer.
  if (fp != NULL) {
		// Check invalid counter.
    if (counters != NULL) {
      fputc('{', fp);
      for (countersnode_t* node = counters->head; node != NULL; node = node->next) {
        // print this node
				fprintf(fp, "%d=%d", node->key, node->count);
        fputc(',', fp);
      }
      fputc('}', fp);
    } else {
      fputs("(null)", fp);
    }
  }
}

/* **********
 * The `counters_iterate`function takes in three arguments: a pointer to a 
 * counter, an void pointer `arg` and a function pointer that returns nothing
 * but takes 3 arguments `arg`, an int `key`, and an int `count`. 
 * This function iterates through the counter structure, and at each
 * node/unique key it will run the given `itemfunc` on that key and its 
 * associated count. The `arg` that is given will also be passed into the 
 * function at each timestep. Note that this function will not modify the 
 * contents of the counter structure. If the given counter is NULL, or if 
 * the function pointer is NULL, then this function will do nothing.
 */
void counters_iterate(counters_t* counters, void* arg, 
	void (*itemfunc)(void* arg, const int key, const int count))
{
  if (counters != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each key and count
    for (countersnode_t* node = counters->head; node != NULL; 
					node = node->next) {
      (*itemfunc)(arg, node->key, node->count); 
    }
  }
}

/* **********
 * The `counters_delete` function takes in one argument: a pointer to a
 * counter. It frees the counter and all its associated nodes. Since the nodes
 * contents are not dynamically allocated it does not need to free these.
 */
void counters_delete(counters_t* counters)
{
  if (counters != NULL) {
    for (countersnode_t* node = counters->head; node != NULL; ) {
      countersnode_t* next = node->next; // remember what comes next
      free(node);                    // free the node
      node = next;                       // and move on to next
    }

    free(counters);
  }
}
