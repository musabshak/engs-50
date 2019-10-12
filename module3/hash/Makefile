CFLAGS=-Wall -pedantic -std=c11 -I. -g
PROGRAM=queue

all: test1 test2 test3 test4 test5 test6 test7 test8 test9 test10

hash.o: hash.c queue.h
	gcc $(CFLAGS) -c hash.c

queue.o: queue.c 
	gcc $(CFLAGS) -c queue.c

test1.o: test1.c hash.h queue.h queue.c
	gcc $(CFLAGS) -c test1.c

test1: test1.o hash.o queue.o
	gcc $(CFLAGS) test1.o queue.o hash.o -o $@

clean:
	rm -f *.o test1

