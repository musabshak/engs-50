CFLAGS=-Wall -pedantic -std=c11 -I. -g
PROGRAM=queue
OFILES=queue.o hash.o webpage.o pageio.o
TESTS=test1 test2 test3 test4 test5 test6 test7 test8

all: $(OFILES)
	ar cr ../lib/libutils.a $(OFILES)
	rm -f *.o

%.o: %.c %.h 
	gcc $(CFLAGS) -c $<

clean:
	rm -f *.o $(TESTS)

