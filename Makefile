CFLAGS=-Wall -pedantic -std=c11 -I. -g
PROGRAM=list

all: test1 test2 test3 test4 test5 test6 test7 test8 test9 test10

$(PROGRAM).o: $(PROGRAM).c $(PROGRAM).h
	gcc $(CFLAGS) -c $(PROGRAM).c

car_utilities.o: car_utilities.c car_utilities.h
	gcc $(CFLAGS) -c car_utilities.c

test1.o: test1.c $(PROGRAM).h car_utilities.h
	gcc $(CFLAGS) -c test1.c

test2.o: test2.c $(PROGRAM).h car_utilities.h
	gcc $(CFLAGS) -c test2.c

test3.o: test3.c $(PROGRAM).h car_utilities.h
	gcc $(CFLAGS) -c test3.c

test4.o: test4.c $(PROGRAM).h car_utilities.h
	gcc $(CFLAGS) -c test4.c

test5.o: test5.c $(PROGRAM).h car_utilities.h
	gcc $(CFLAGS) -c test5.c

test6.o: test6.c $(PROGRAM).h car_utilities.h
	gcc $(CFLAGS) -c test6.c

test7.o: test7.c $(PROGRAM).h car_utilities.h
	gcc $(CFLAGS) -c test7.c

test8.o: test8.c $(PROGRAM).h car_utilities.h
	gcc $(CFLAGS) -c test8.c

test9.o: test9.c $(PROGRAM).h car_utilities.h
	gcc $(CFLAGS) -c test9.c

test10.o: test10.c $(PROGRAM).h car_utilities.h
	gcc $(CFLAGS) -c test10.c

test1: test1.o $(PROGRAM).o car_utilities.o
	gcc $(CFLAGS) test1.o $(PROGRAM).o car_utilities.o -o test1 -lm

test2: test2.o $(PROGRAM).o car_utilities.o
	gcc $(CFLAGS) test2.o $(PROGRAM).o car_utilities.o -o test2 -lm

test3: test3.o $(PROGRAM).o car_utilities.o
	gcc $(CFLAGS) test3.o $(PROGRAM).o car_utilities.o -o test3 -lm

test4: test4.o $(PROGRAM).o car_utilities.o
	gcc $(CFLAGS) test4.o $(PROGRAM).o car_utilities.o -o test4 -lm

test5: test5.o $(PROGRAM).o car_utilities.o
	gcc $(CFLAGS) test5.o $(PROGRAM).o car_utilities.o -o test5 -lm

test6: test6.o $(PROGRAM).o car_utilities.o
	gcc $(CFLAGS) test6.o $(PROGRAM).o car_utilities.o -o test6 -lm

test7: test7.o $(PROGRAM).o car_utilities.o
	gcc $(CFLAGS) test7.o $(PROGRAM).o car_utilities.o -o test7 -lm

test8: test8.o $(PROGRAM).o car_utilities.o
	gcc $(CFLAGS) test8.o $(PROGRAM).o car_utilities.o -o test8 -lm

test9: test9.o $(PROGRAM).o car_utilities.o
	gcc $(CFLAGS) test9.o $(PROGRAM).o car_utilities.o -o test9 -lm

test10: test10.o $(PROGRAM).o car_utilities.o
	gcc $(CFLAGS) test10.o $(PROGRAM).o car_utilities.o -o test10 -lm

clean:
	rm -f *.o test1 test2 test3 test4 test5 test6 test7 test8 test9 test10

