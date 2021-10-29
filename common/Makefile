# Alan Sun
# CS50, Fall 2021, Tiny Search Engine
# Makefile for the crawler common library (common/Makefile)
# 
# This Makefile builds and compiles all the utility functions in the common
# directory into a archive library.

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc 

.PHONY: clean test_index

common.a: pagedir.o index.o word.o tokenizer.o operators.o
	ar cr common.a pagedir.o index.o word.o tokenizer.o operators.o

pagedir.o: pagedir.c pagedir.h ../libcs50/memory.h
	$(CC) $(CFLAGS) -c pagedir.c

index.o: index.c index.h ../libcs50/hashtable.h ../libcs50/counters.h \
				../libcs50/memory.h
	$(CC) $(CFLAGS) -c index.c

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c

operators.o: operators.c operators.h
	$(CC) $(CFLAGS) -c operators.c

test_index:
	$(CC) $(CFLAGS) -DUNIT_TEST index.c ../libcs50/libcs50.a -o index
	valgrind --leak-check=full ./index &> testing.out

test_tokenizer:
	$(CC) $(CFLAGS) -DUNIT_TEST tokenizer.c -o tokenizer
	valgrind --leak-check=full ./tokenizer &> testing.out

test_operators:
	$(CC) $(CFLAGS) -DUNIT_TEST operators.c ../libcs50/libcs50.a -o operators
	valgrind --leak-check=full ./operators &> testing.out

word.o: word.c word.h
	$(CC) $(CFLAGS) -c word.c


clean:
	rm -f *.o *.x index test1.output test2.output testing.out tokenizer
