#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"
#include "memory.h"

/* Alan Sun
 * CS50, Fall 2021, Lab 3
 * Set (set.c)
 * 
 * Please note that a significant portion of this code is taken from the 
 * template code given in bag.c. 
 * This module implements a set datastructure for arbitrary data types. 
 * A set is defined as a datastructure with unique string keys and arbitrary
 * items corresponding to keys. In this module, we implement sets as linked
 * lists. The keys in the structure can be used by the user to search for its
 * corresponding values. Please note that the set is unordered. Briefly, the 
 * functions and structures in this module define a structure `set_t` that is
 * exposed to the user of the module. `set_t` points to the head of the linked
 * list whose nodes are represented using `setnode_t`. There exists six 
 * functionalities that are avaliable to the user. These are described in short
 * here; please refer to the function headers for a more detailed description:
 *
 *	- `set_new` defines a new empty set and returns a pointer to this set.
 *	- `set_insert` inserts a new key and item into the set.
 *	- `set_find` given a key, this function returns the associated value.
 *	- `set_print` prints out the contents of the set with a given print
 *	  formatter function. 
 *	- `set_iterate` iterates through the set while applying the given function
 *	  on each key and item.
 *	- `set_delete` frees all the dynamically allocated memory associated with
 *	  the set.
 */

typedef struct setnode {
	char* key;
  void* item;                 // pointer to data for this item
  struct setnode *next;       // link to next node
} setnode_t;

typedef struct set {
  struct setnode *head;       // head of the list of items in set
} set_t;

static setnode_t* setnode_new(char* key, void* item);

/* ***********
 * The `set_new` function requires no arguments. It creates a new empty set
 * and returns a pointer to this set. Note that since the memory for this
 * new set is dynamically allocated, it must also be freed by calling
 * `set_delete(<pointer to set>)`. If for any case, memory cannot be 
 * allocated for this set, then NULL is returned.
 */
set_t* set_new(void)
{
  set_t* set = malloc(sizeof(set_t));

	// Check error allocating set.
  if (set == NULL) {
    return NULL;
  } else {
    // Initialize contents of set structure.
    set->head = NULL;
    return set;
  }
}

/* **********
 * The `set_insert` function requires three arguments: a pointer to the target
 * set, a string key, and a pointer to the target item. This function attempts
 * to insert this key -> item into the set. Since the set is defined by unique
 * keys, if the key given already exists in the set, then false will be
 * returned. If `set`, `item` or `key` is NULL, then false is also returned.
 * In the case that all three arguments are valid the key with its value is 
 * inserted into the set. During this insertion, the key that is being inserted
 * into the set is a copy of the given key. However, the pointer to the item is
 * unchanged. Therefore, changing the given key will not alter the set;
 * changing the value of the item pointer will change the set. If the insertion
 * is successful, then true is returned.
 * Please note that the order of this insertion is undefined. 
 */
bool set_insert(set_t* set, const char* key, void* item)
{
	setnode_t* new;
	setnode_t* curr;
	char* key_copy;

  // We add a new node if the arguments are valid
  if (set != NULL && item != NULL && key != NULL) {
		/* Iterate through the entire set, if the key being inserted already
		 * exists in the set then false is returned; otherwise, we insert at the
		 * start of the set and return true.
		 */
		curr = set->head;
		while (curr != NULL) {
			// Check for string inequality. 
			if (strcmp(curr->key, key))
				curr = curr->next;
			else
				return false;
		}
		// Create a copy of the key.
		key_copy = malloc(strlen(key) + 1);
		strcpy(key_copy, key);
		new = setnode_new(key_copy, item);
		if (new == NULL)
			return false;
		// Insert the new node at the head of the list.
		new->next = set->head;
		set->head = new;
		return true;
	}
	return false;
}

/* ***********
 * The `setnode_new` function creates a new node given its key and value. 
 * It initializes its `next` value to NULL. Moreover, the function will 
 * return NULL if memory cannot be allocated. This function is a local helper
 * that will not be visible to users of the module.
 */
static setnode_t* setnode_new(char* key, void* item)
{
  setnode_t* node = malloc(sizeof(setnode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
		node->key = key;
    node->item = item;
    node->next = NULL;
    return node;
  }
}

/* **********
 * The `set_find` function requires two arguments: a pointer to the target set
 * and a string key which identifies the key of interest. If either the set or 
 * key is NULL, then NULL is returned. If the key does not exist in the set,
 * then NULL is also returned. If the key is in the set, then the function will
 * return a pointer to the item associated with that key. The function will not
 * extract this key or the item associated with this key.
 */
void* set_find(set_t* set, const char* key)
{
	setnode_t* curr;

	// Set is NULL or key is NULL
  if (set == NULL || key == NULL) {
    return NULL;             
	// Set is empty.
  } else if (set->head == NULL) {
    return NULL;              
  } else {
		/* We loop through the set to find a key that matches the given key,
		 * if no such key is found then a NULL-pointer is returned. Otherwise,
		 * the pointer to the item is returned.
		 */
		curr = set->head;
		while (curr != NULL) {
			if (!strcmp(curr->key, key))
				return curr->item;
			else
				curr = curr->next;
		}
		return NULL;
  }
}

/* ********
 * The `set_print` function requires three arguments: a pointer to the target
 * set, a function pointer that takes in three arguments, as well as a file
 * pointer to direct the output of the printing. Here, the `itemprint`
 * parameter handles all the heavy lifting of printing. This function will 
 * simply wrap its outputs with curly braces and delimit its reponses with 
 * commas. If any of the given pointers are NULL, then the function will 
 * do nothing. If the given set is empty, then "{}" will be outputted.
 */
void set_print(set_t* set, FILE* fp, 
	void (*itemprint)(FILE* fp, const char* key, void* item))
{
	// Check valid file pointer.
  if (fp != NULL) {
		// Check valid set.
    if (set != NULL) {
      fputc('{', fp);
      for (setnode_t* node = set->head; node != NULL; node = node->next) {
        // print this node
        if (itemprint != NULL) { // print the node's item 
          (*itemprint)(fp, node->key, node->item); 
          fputc(',', fp);
        }
      }
      fputc('}', fp);
    } else {
      fputs("(null)", fp);
    }
  }
}

/* ********
 * The `set_iterate` function requires three arguments: a pointer to the 
 * target set, a function pointer that takes in three arguments, as well as
 * an argument that is redirected as an input to the function. This function
 * will iterate through the entire set and at each unique key, it will apply
 * the given function. The given function may change the contents of the set.
 * Note that if any of the arguments are NULL, or if the set is empty, nothing
 * will be done.
 */
void set_iterate(set_t* set, void* arg, 
	void (*itemfunc)(void* arg, const char* key, void* item) )
{
  if (set != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (setnode_t* node = set->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->item); 
    }
  }
}

/* ********
 * The `set_delete` function requires two arguments: a pointer to the target
 * set and a function pointer that is responsible for deleting the `item` 
 * associated with each key. This function will free all memory associated with
 * the set. If either pointer is NULL, then nothing will be done. Please note
 * that since the `key` is a part of the allocated memory it will be freed as
 * well; however, the module is not responsible for freeing the original key
 * pointer passed into set insert.
 */
void set_delete(set_t* set, void (*itemdelete)(void* item))
{
  if (set != NULL) {
    for (setnode_t* node = set->head; node != NULL; ) {
			if (itemdelete != NULL)
      	(*itemdelete)(node->item);      // delete node's item
			free(node->key);
      setnode_t* next = node->next;     // remember what comes next
      free(node);                   // free the node
      node = next;                      // and move on to next
    }
    free(set);
  }
}
