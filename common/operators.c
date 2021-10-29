#include<stdlib.h>
#include "counters.h"
/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Querier
 * Module for operators (operators.c)
 *
 * This is the header file for the operators module. It defines some basic 
 * functionalities for working with counters. Crucially, it defines two 
 * operations: intersections and unions. More about these functions are
 * described in detail in the function headers as well as the `README.md` of 
 * this directory.
 */

static void intersect_help(void *, const int, const int);
static void union_help(void *, const int, const int);
static void copy_counters(void *, const int, const int);

typedef struct carg {
  counters_t *c2;
  counters_t *c_new;
} carg_t;

/* (description): Find the intersection of two counters and stores the result in
 * a new counter that is dynamically allocated and returned to the user. Note,
 * that the user must free this struct using the counters_delete function.
 *
 * (inputs): Two counters, both arguments must be non-NULL. If either is NULL,
 * then NULL is returned. 
 *
 */
counters_t *intersections(counters_t *c1, counters_t *c2)
{
  if (c1 == NULL || c2 == NULL)
    return NULL;

  carg_t *carg = malloc(sizeof(carg_t));
  counters_t *ret;
  carg->c2 = c2;
  carg->c_new = counters_new();

  counters_iterate(c1, carg, intersect_help);  
  ret = carg->c_new;
  free(carg);
  return ret;
}

/* (description): A helper function that assists the intersections function,
 * since this function has no use outside of the scope of intersections,
 * it is declared static and not exposed to the user. 
 *
 * (inputs): The inputs of this helper function conform to the format of the
 * funtion pointer type of counters_iterate.
 *
 * (error handling):
 *
 */
static void intersect_help(void *arg, const int key, const int count)
{
  carg_t *counters = arg;
  counters_t *c2 = counters->c2;
  counters_t *c_new = counters->c_new;
  int scount = counters_get(c2, key);

  // The same key exists in both counters, then we perform the min of either.
  if (scount != 0) {
    counters_set(c_new, key, count < scount ? count : scount);
  }
}

/* (description): Find the union of two counters and stores the result in
 * the a new counter similar to counters intersection. Both provided 
 * counter remains unchanged. 
 *
 * (inputs): Two counters, both arguments must be non-NULL. If either is NULL,
 * then NULL is returned.
 *
 */
counters_t *unions(counters_t *c1, counters_t *c2)
{  if (c1 == NULL || c2 == NULL)
    return NULL;

  carg_t *carg = malloc(sizeof(carg_t));
  counters_t *ret;
  carg->c2 = c2;
  carg->c_new = counters_new();

  // We add all of the items in the second counter to the new one.
  counters_iterate(c2, carg->c_new, copy_counters);

  // Now we merge the now-copied second counter with the first one.
  counters_iterate(c1, carg, union_help);  
  
  ret = carg->c_new;
  free(carg);
  return ret;
}

/* (description): Given a counter, this function will dynamically duplicate
 * it, copying its items and keys as well as 
 *
 * (inputs):
 *
 * (outputs):
 *
 * (error handling):
 *
 */
counters_t *dup_counter(counters_t *counter)
{
  


}

static void copy_counters(void *arg, const int key, const int count)
{
  counters_set(arg, key, count);
}

/* (description): A helper function that assists the unions function, since
 * this function has no use outside of the scope of unions, it is declared
 * static and not exposed to the user.
 *
 * (inputs): The inputs of this helper function conform to the format of the
 * function pointer type of counters_iterate. 
 *
 * (error handling):
 *
 */
static void union_help(void *arg, const int key, const int count)
{
  carg_t *counters = arg;
  counters_t *c2 = counters->c2;
  counters_t *c_new = counters->c_new;
  int scount = counters_get(c2, key);

  // We add both of the counts together. 
  counters_set(c_new, key, count + scount);
}


// ##########################################################################
// ##########################################################################
//                         Unit Test for Operators
// ##########################################################################
// ##########################################################################
#ifdef UNIT_TEST
#include<assert.h>

int main(void)
{
  counters_t *c1 = counters_new();
  counters_t *c2 = counters_new();
  counters_t *new; 
  // Intersect and union two empty counters.
  counters_delete(intersections(c1, c2));
  counters_delete(unions(c1, c2));

  // Intersect and union a counter and an empty counter.
  counters_set(c1, 1, 2);
  counters_set(c1, 2, 4);
  counters_set(c1, 4, 1);
  counters_set(c1, 3, 7);
  new = intersections(c1, c2);
  assert(counters_get(new, 1) == 0);
  assert(counters_get(new, 2) == 0);
  assert(counters_get(new, 4) == 0);
  assert(counters_get(new, 3) == 0);
  counters_delete(new);

  new = unions(c2, c1);
  assert(counters_get(new, 1) == 2);
  assert(counters_get(new, 2) == 4);
  assert(counters_get(new, 4) == 1);
  assert(counters_get(new, 3) == 7);
  counters_delete(new);
  
  // Intersect and union two non-empty counters.
  counters_delete(c1);
  counters_delete(c2);

  c1 = counters_new(), c2 = counters_new();
  counters_set(c1, 1, 2);
  counters_set(c1, 2, 4);
  counters_set(c1, 4, 5);
  counters_set(c1, 3, 7);
  counters_set(c2, 1, 8);
  counters_set(c2, 2, 9);
  counters_set(c2, 4, 1);
  new = intersections(c1, c2);
  assert(counters_get(new, 1) == 2);
  assert(counters_get(new, 2) == 4); 
  assert(counters_get(new, 4) == 1);
  assert(counters_get(new, 3) == 0);
  counters_delete(new);
  
  counters_delete(c1);
  counters_delete(c2);

  c1 = counters_new(), c2 = counters_new();
  counters_set(c1, 1, 2);
  counters_set(c1, 2, 4);
  counters_set(c1, 4, 5);
  counters_set(c1, 3, 7);
  counters_set(c2, 1, 8);
  counters_set(c2, 2, 9);
  counters_set(c2, 4, 1);
  new = unions(c1, c2);
  assert(counters_get(new, 1) == 10);
  assert(counters_get(new, 2) == 13); 
  assert(counters_get(new, 4) == 6);
  assert(counters_get(new, 3) == 7);
  counters_delete(new);
  counters_delete(c1), counters_delete(c2);
}
#endif
