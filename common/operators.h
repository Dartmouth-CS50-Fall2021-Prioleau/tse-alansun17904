/* Alan Sun
 * CS50, Fall 2021, Tiny Search Engine: Querier 
 * Module for operators (operators.h)
 *
 * This is the header file for the operators module. It defines some basic
 * functionalities for working with counters. Crucially, it defines two
 * operators: intersections and unions. 
 */

#ifndef OPERATORS_H
#define OPERATORS_H

#include "counters.h"

counters_t *intersections(counters_t *c1, counters_t *c2);

counters_t *unions(counters_t *c1, counters_t *c2);

counters_t *dup_counter(counters_t counter);

#endif
