CFLAGS=-Wall -pedantic -std=c11 -I ../utils -L ../lib -g
LIBS=-lutils -lcurl -lpthread

crawler: crawler.c
	gcc $(CFLAGS) crawler.c $(LIBS) -o $@

conc_crawler: conc_crawler.c
	gcc $(CFLAGS) conc_crawler.c $(LIBS) -o $@

run_conc_crawler1: conc_crawler
	rm ../pages/*
	conc_crawler "https://thayer.github.io/engs50/" "../pages" 1

run_crawler0: crawler
	rm ../pages/*
	crawler "https://thayer.github.io/engs50/" "../pages" 0

run_crawler1: crawler
	rm ../pages/*
	crawler "https://thayer.github.io/engs50/" "../pages" 1

run_crawler2: crawler
	rm ../pages/*
	crawler "https://thayer.github.io/engs50/" "../pages" 2

run_crawler3: crawler
	rm ../pages/*
	crawler "https://thayer.github.io/engs50/" "../pages" 3

clean:
	rm -f *.o crawler

