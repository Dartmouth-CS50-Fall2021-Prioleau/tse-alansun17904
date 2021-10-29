# Alan Sun
# CS50, Fall 2021, Tiny Search Engine: Querier
# Makefile for Querier (querier/Makefile)
# 
# This Makefile builds and compiles the querier program in querier.c.

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
LIBS = ../common/common.a ../libcs50/libcs50.a

.PHONY: clean test test-valgrind

querier: querier.o $(LIBS)
	$(CC) querier.o $(LIBS) -o querier

querier.o: querier.c ../common/tokenizer.h ../common/index.h \
				../libcs50/hashtable.h ../libcs50/counters.h ../common/operators.h
	$(CC) $(CFLAGS) -c querier.c

test: 
	./testing.sh &> testing.out

valgrind:
	./testing.sh CHECK_MEMORY &> testing.out
# Remove all the directories that start with test and end with a number.
clean:
	rm -rf testing.out
	rm -rf $$(ls | grep "^test[0-9]*$$" | tr "\n" " ") 
	rm -rf *.x *.o
