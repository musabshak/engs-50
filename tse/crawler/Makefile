CFLAGS=-Wall -pedantic -std=c11 -I ../utils -L ../lib -g
LIBS=-lutils -lcurl -lpthread

crawler: crawler.c
	gcc $(CFLAGS) crawler.c $(LIBS) -o $@

conc_crawler: conc_crawler.c
	gcc $(CFLAGS) conc_crawler.c $(LIBS) -o $@

run_conc_crawler3: conc_crawler
	rm ../pages1/*
	conc_crawler "https://thayer.github.io/engs50/" "../pages1" 3 20

run_crawler0: crawler
	rm ../pages1/*
	valgrind crawler "https://thayer.github.io/engs50/" "../pages1" 0

run_crawler1: crawler
	rm ../pages1/*
	valgrind crawler "https://thayer.github.io/engs50/" "../pages1" 1

run_crawler2: crawler
	rm ../pages1/*
	valgrind crawler "https://thayer.github.io/engs50/" "../pages1" 2

run_crawler3: crawler
	rm ../pages/*
	valgrind crawler "https://thayer.github.io/engs50/" "../pages" 3

clean:
	rm -f *.o crawler conc_crawler

