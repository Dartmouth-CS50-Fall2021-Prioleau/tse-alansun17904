# Alan Sun
# CS50, Fall 2021, Tiny Search Engine
# Makefile for the crawler common library (common/Makefile)
# 
# This Makefile builds and compiles all the utility functions in the common
# directory into a archive library.

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc 
LIBS = ../libcs50/libcs50.a ../common/common.a

.PHONY: clean

indexer.x: indexer.o $(LIBS) 
	$(CC) indexer.o $(LIBS) -o indexer.x 

indexer.o: indexer.c ../libcs50/hashtable.h ../libcs50/counters.h \
				../common/pagedir.h ../common/word.h
	$(CC) $(CFLAGS) -c indexer.c

clean:
	rm -f *.o *.x
