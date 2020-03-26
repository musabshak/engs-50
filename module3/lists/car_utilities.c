/* car_utilities.c --- 
 * 
 * 
 * Author: Musab Shakeel
 * Created: Thu Oct  3 15:07:49 2019 (-0400)
 * Version: 
 * 
 * Description: Useful car functions for testing
 * 
 */

#include <string.h>
#include <list.h>
#include <stdlib.h>
#include <stdio.h>

car_t * make_car(char *plate, double price, int year) {
  car_t * pp;

	if (!(pp = (car_t *) malloc(sizeof(car_t)))) {
    return NULL;
  }

  pp->next = NULL;
  strcpy(pp->plate, plate);
  pp->price = price;
  pp->year = year;
	
  return pp;
}

void free_car(car_t *cp) {
	free(cp);
}

void printcar(car_t *p) {
  printf("plate: %s, price: %lf, year: %d\n", p->plate, p->price, p->year);
}
