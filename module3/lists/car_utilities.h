#pragma once
/*
 * car_utilities.h -- public interface to car utility functions
 *
 */

/* make_car(): Dynamically allocate memory to a car struct and return a pointer
 * to the beginning of that allocated memory
 */
car_t * make_car(char *plate, double price, int year);

/* printcar(): Given a pointer to a car struct, print the details of the car
 */ 
void printcar(car_t *p);
