# Alan Sun
# CS50, Fall 2021, Tiny Search Engine: Crawler
# Makefile for Crawler (crawler/Makefile)
# 
# This Makefile builds and compiles the crawler program in crawler.c.

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
LIBS = ../libcs50/libcs50.a ../common/common.a

.PHONY: clean test test-valgrind

crawler.x: crawler.o $(LIBS)
	$(CC) crawler.o $(LIBS) -o crawler.x

crawler.o: crawler.c ../libcs50/webpage.h ../common/pagedir.h ../libcs50/hashtable.h ../libcs50/bag.h
	$(CC) $(CFLAGS) -c crawler.c

test: 
	./testing.sh &> testing.out

# Remove all the directories that start with test and end with a number.
clean:
	rm -rf testing.out
	rm -rf $$(ls | grep "^test[0-9]*$$" | tr "\n" " ") 
	rm -rf *.x *.o
