CFLAGS=-Wall -pedantic -std=c11 -I ../utils -L ../lib -g
LIBS=-lutils -lcurl

all: hash_test7 hash_test8

hash_test7: hash_test7.c clean
	gcc $(CFLAGS) hash_test7.c $(LIBS) -o $@

hash_test8: hash_test8.c clean
	gcc $(CFLAGS) hash_test8.c $(LIBS) -o $@

queue_test: queue_test.c clean
	gcc $(CFLAGS) queue_test.c $(LIBS) -o $@

pageio_test: pageio_test.c clean
	gcc $(CFLAGS) pageio_test.c $(LIBS) -o $@

clean:
	rm -f *.o pageio_test queue_test hash_test7 hash_test8
