#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "set.h"
#include "jhash.h"

/* Alan Sun
 * CS50, Fall 2021, Lab 3
 * Hashtable (hashtable.c)
 *
 * This module creates a generic hashtable datastructure that is based
 * on bucketing. The hashtable operates similar to a set whereas it is defined
 * as having entries with unique keys and these keys map to items who may not
 * be unique. Though the functionalities are the same, the hashtable represents
 * a much more efficient datastructure. The hash function used here, is given as
 * `hash_jenkins` in hash.c. The hashtable_t structure stores a list of buckets
 * implemented as sets; If a collision happens then items are added to the set
 * associated with the duplicate index. The user is able to specify how many
 * buckets the hashtable is initialized with; generally, as the amount of
 * stored data grows, a larger number of buckets is more efficient. The 
 * `hashtable_t` structure is exposed to the user and there exists five
 * functionalities that the user can call. These are described in short here;
 * please refer to the function headers for a more detailed description:
 *
 *	- `hashtable_new` defines a new hash table given a user-defined number
 *	  of buckets.
 *	- `hashtable_insert` inserts a key/value into the given hashtable.
 *	- `hashtable_find` finds the item of a key in a given hashtable.
 *	- `hashtable_print` prints out the contents of the hashtable.
 *	- `hashtable_iterate` iterates through the hashtable while applying a given
 *	  function on its keys and values.
 *	- `hashtable_delete` frees all of the hashtable's associated memory.
 */

typedef struct hashtable {
	int num;
	set_t** buckets; // pointer to an array of pointers to a set.
} hashtable_t;

/* ***********
 * The `hashtable_new` function requires one argument: int `num_slots`. Since
 * the hashtable we are implementing is based on separate chaining, the
 * hashtable must require the user to specify the number of buckets or "chains"
 * that the hashtable will be using. This function will then return a pointer
 * to the newly created hashtable. If for any reason there is an error
 * allocating memory, then NULL is returned. Note that it is more efficient for
 * larger datasets to have a larger bucket size. This will reduce collisions
 * and shorten search time. Please note that since memory is being dynamically
 * allocated for the hashtable, the hashtable must be freed using the
 * `hashtable_delete` function.
 */
hashtable_t* hashtable_new(const int num_slots)
{
	hashtable_t* hashtable;
	
	// Invalid slots
	if (num_slots <= 0)
		return NULL;
	
	hashtable = malloc(sizeof(hashtable_t));

	if (hashtable == NULL) {
		// Error allocating hashtable.
		return NULL;
	} else {
		// Initialize the buckets based on the slots.
		hashtable->num = num_slots;
		hashtable->buckets = calloc(sizeof(set_t*), num_slots);
		return hashtable;
	}
}

/* ************
 * The `hashtable_insert` function requires three arguments: a pointer to a 
 * hashtable, a string key, and an arbitrary type item. Inserting into a hash
 * table from the user's point of view is the same as a set: there cannot be
 * items with the same key, and none of the arguments can be NULL. If any of
 * these conditions are true, then the function will return false. In the case
 * that all three arguments are valid the key with its associated item will be
 * inserted into the hashtable. This will be done by hashing the key to get its
 * bucket's location in the array, then inserting the key into the bucket at
 * its hash index. If there does not exist a bucket at its hash index, then a 
 * new one is created. `true` is returned if the item was inserted successfuly.
 */
bool hashtable_insert(hashtable_t* ht, const char* key, void* item)
{
	int index;

	// Check argument validity.
	if (ht != NULL && key != NULL && item != NULL) {
		/* Check for key duplication, if NULL at corresponding bucket index then
		 * the key must not exist; otherwise, try to insert in the corresponding
		 * set; if the response is false, then there is duplication.
		 */
		index = JenkinsHash(key, ht->num);
		if ((ht->buckets)[index] == NULL) {
			// If a bucket doesn't exist at the hash index, then create new set.
			(ht->buckets)[index] = set_new();
			return set_insert((ht->buckets)[index], key, item);
		} else {
			return set_insert((ht->buckets)[index], key, item);
		}
	}
	return false;
}

/* **************
 * The `hashtable_find` function requires two arguments: a pointer to the
 * target hashtable, and a string key. If either of the arguments given is
 * false, then NULL is returned. If the hashtable is valid, but the key does
 * not exist in the hashtable, then NULL is also returned. The hashtable finds
 * by string comparing the key with the existing keys in the hashtable.
 * If successful, the function should return a pointer to the item stored with
 * the given key.
 */
void* hashtable_find(hashtable_t* ht, const char* key)
{
	int index;

	// Check for invalid arguments.
	if (ht == NULL || key == NULL) {
		return NULL;
	} else {
		// Go to bucket in hashtable associated with the hashed key index.
		index = JenkinsHash(key, ht->num);
		// If the target bucket is empty, then the key must not exist in the table.
		if ((ht->buckets)[index] == NULL) {
			return NULL;
		} else {
			// Found bucket that key is a part of; search using "set" functions.
			return set_find((ht->buckets)[index], key);
		}
	}
}

/* *********
 * The `hashtable_print` function requires three arguments: a pointer to the
 * target hashtable, a file pointer where the results of the function will be 
 * outputted, and a function that is responsible for printing out the key and
 * the item associated with that key. Here the `itemprint` function handles all
 * the heavy lifting of printing. The function will print out the contents of
 * each of its buckets in separating lines by leveraging the printing
 * functionality of the underlying bucket datastructure: `set`. Please see
 * the function header for `set_print` in `set.c` for a detailed description
 * of this. In the case that `fp` is NULL, the function will do nothing. 
 * Otherwise, if `ht` is NULL, then the function will print "(null)". Note that
 * the order in which the items are printed within each bucket are undefined.
 */
void hashtable_print(hashtable_t* ht, FILE* fp,
	void (*itemprint)(FILE* fp, const char* key, void* item))
{
	int i;

	if (ht == NULL || itemprint == NULL || fp == NULL) {
		if (ht == NULL)
			fprintf(fp, "(null)");
		return;
	}
	for (i = 0; i < ht->num; i++) {
		set_print((ht->buckets)[i], fp, itemprint);
		printf("\n");
	}
}

/* **********
 * The `hashtable_iterate` function requires three arguments: a pointer to the
 * target hashtable, a void pointer to an argument delivered to `itemfunc` and
 * a function pointer `itemfunc` that is applied to the each item in the
 * hashtable. If any of `ht` is NULL or `itemfunc` is NULL, then the function 
 * does nothing. Otherwise, the function will first loop through every bucket
 * since each bucket is a set, it wil apply the function on each item in the
 * set through the `set_iterate` function. The function `itemfunc` may make
 * modifications on the items in the hashtable.
 */
void hashtable_iterate(hashtable_t* ht, void* arg,
	void (*itemfunc)(void* arg, const char* key, void* item))
{
	int i;
	if (ht == NULL || itemfunc == NULL)
		return;
	for (i = 0; i < ht->num; i++) {
		set_iterate((ht->buckets)[i], arg, itemfunc);
	}
}

/* **********
 * The `hashtable_delete` function requires two arguments: a pointer to the
 * target hashtable, a function pointer to a function that is responsible
 * for freeing the memory associated with the item of the hashtable. If 
 * either argument is NULL, then nothing happens. This function frees all of
 * the memory associated with the hashtable, it keys, and the array of sets.
 * Please note that the function is not responsible for freeing the original
 * pointer that passed into `set_insert` as a copy of the key is made in the
 * structure.
 */ 
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item))
{
	int i;

	if (ht == NULL)
		return;
	for (i = 0; i < ht->num; i++) {
		set_delete((ht->buckets)[i], itemdelete);
	}
	free(ht->buckets);
	free(ht);
}





