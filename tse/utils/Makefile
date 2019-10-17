CFLAGS=-Wall -pedantic -std=c11 -I. -g
PROGRAM=queue
OFILES=queue.o hash.o webpage.o
TESTS=test1 test2 test3 test4 test5 test6 test7 test8

all: $(OFILES)
	ar cr ../lib/libutils.a $(OFILES)
	rm -f *.o

%.o: %.c %.h 
	gcc $(CFLAGS) -c $<

test1: test1.o hash.o queue.o
	gcc $(CFLAGS) test1.o queue.o hash.o -o $@

test2: test2.o hash.o queue.o
	gcc $(CFLAGS) test2.o queue.o hash.o -o $@  

test3: test3.o hash.o queue.o
	gcc $(CFLAGS) test3.o queue.o hash.o -o $@

test4: test4.o hash.o queue.o
	gcc $(CFLAGS) test4.o queue.o hash.o -o $@

test5: test5.o hash.o queue.o
	gcc $(CFLAGS) test5.o queue.o hash.o -o $@

test6: test6.o hash.o queue.o
	gcc $(CFLAGS) test6.o queue.o hash.o -o $@

test7: test7.o hash.o queue.o
	gcc $(CFLAGS) test7.o queue.o hash.o -o $@

test8: test8.o hash.o queue.o
	gcc $(CFLAGS) test8.o queue.o hash.o -o $@

clean:
	rm -f *.o $(TESTS)

