CFLAGS=-Wall -pedantic -std=c11 -I. -g
PROGRAM=queue
OFILES=queue.o hash.o webpage.o pageio.o indexio.o lqueue.o lhash.o

all: $(OFILES)
	ar cr ../lib/libutils.a $(OFILES)
	rm -f *.o

%.o: %.c %.h 
	gcc $(CFLAGS) -c $<

