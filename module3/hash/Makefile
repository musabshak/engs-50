CFLAGS=-Wall -pedantic -std=c11 -I. -g
PROGRAM=queue

all: test1 test2 test3 test4 test5 test6 test7 test8

hash.o: hash.c queue.h
	gcc $(CFLAGS) -c hash.c

queue.o: queue.c 
	gcc $(CFLAGS) -c queue.c

test1.o: test1.c hash.h queue.h queue.c
	gcc $(CFLAGS) -c test1.c

test1: test1.o hash.o queue.o
	gcc $(CFLAGS) test1.o queue.o hash.o -o $@

test2.o: test2.c hash.h queue.h queue.c
	gcc $(CFLAGS) -c test2.c

test2: test2.o hash.o queue.o
	gcc $(CFLAGS) test2.o queue.o hash.o -o $@  

test3.o: test3.c hash.h queue.h queue.c
	gcc $(CFLAGS) -c test3.c

test3: test3.o hash.o queue.o
	gcc $(CFLAGS) test3.o queue.o hash.o -o $@

test4.o: test4.c hash.h queue.h queue.c
	gcc $(CFLAGS) -c test4.c

test4: test4.o hash.o queue.o
	gcc $(CFLAGS) test4.o queue.o hash.o -o $@

test5.o: test5.c hash.h queue.h queue.c
	gcc $(CFLAGS) -c test5.c

test5: test5.o hash.o queue.o
	gcc $(CFLAGS) test5.o queue.o hash.o -o $@

test6.o: test6.c hash.h queue.h queue.c
	gcc $(CFLAGS) -c test6.c

test6: test6.o hash.o queue.o
	gcc $(CFLAGS) test6.o queue.o hash.o -o $@

test7.o: test7.c hash.h queue.h queue.c
	gcc $(CFLAGS) -c test7.c

test7: test7.o hash.o queue.o
	gcc $(CFLAGS) test7.o queue.o hash.o -o $@

test8.o: test8.c hash.h queue.h queue.c
	gcc $(CFLAGS) -c test8.c

test8: test8.o hash.o queue.o
	gcc $(CFLAGS) test8.o queue.o hash.o -o $@


clean:
	rm -f *.o test1 test2 test3 test4 test5 test6 test7 test8

