# Alan Sun
# CS50, Fall 2021, Tiny Search Engine
# Makefile for the crawler common library (common/Makefile)
# 
# This Makefile builds and compiles all the utility functions in the common
# directory into a archive library.

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc

.PHONY: clean

common.a: pagedir.o
	ar cr common.a pagedir.o

pagedir.o: pagedir.c pagedir.h
	$(CC) $(CFLAGS) -c pagedir.c

clean:
	rm -f *.o *.x
