# Alan Sun
# CS50, Fall 2021, Tiny Search Engine
# Makefile for indexer (indexer/Makefile)
# 
# This Makefile builds and compiles all the indexer and also includes 
# functionality for testing.

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc 
LIBS = ../common/common.a ../libcs50/libcs50.a
.PHONY: clean

all: indexer indextest

indexer: $(LIBS) indexer.o  
	$(CC)  indexer.o $(LIBS) -o indexer

indexer.o: indexer.c ../libcs50/hashtable.h ../libcs50/counters.h \
				../common/pagedir.h ../common/word.h ../common/index.h \
				../libcs50/memory.h
	$(CC) $(CFLAGS) -c indexer.c

indextest: $(LIBS) indextest.o
	$(CC) indextest.o $(LIBS) -o indextest

indextest.o: indextest.c ../common/index.h ../libcs50/memory.h \
				../libcs50/file.h 
	$(CC) $(CFLAGS) -c indextest.c

test:
	./testing.sh &> testing.out

valgrind:
	./testing.sh CHECK_MEMORY &> testing.out

clean:
	rm -f *.o indexer indextest
	rm -rf $$(ls | egrep "^test[0-9]+.output$$" | tr "\n" " ")	
	rm -f testout.expected
	rm -f test10.output.sorted
	rm -f testing.out
