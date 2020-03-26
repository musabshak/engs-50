/* list.c --- 
 * 
 * 
 * Author: Musab Shakeel
 * Created: Wed Oct  2 16:16:51 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdlib.h>
#include <inttypes.h>
#include <list.h>
#include <string.h>
#include <stdio.h>

static car_t *front = NULL;

int32_t lput(car_t *cp) {

	if (front == NULL) {
		front = cp;
	}
	else {
		car_t * tmp = front;
		front = cp;
		cp->next = tmp;
	}
	return 0;
}

car_t *lget() {

	if (front == NULL) {
		return NULL;
	}
	else {
		car_t *tmp = front;
		front = front->next;
		return tmp;
	}
}

void lapply(void (*fn)(car_t *cp)) {

	car_t *p;
	
	for (p=front; p!=NULL; p=p->next) {
		fn(p);
	}
}

car_t *lremove(char *platep) {

	car_t *p;
	car_t *f;
	for (p=front; p!=NULL; p=p->next) {
		if (strcmp(p->plate, platep) == 0) {
			if (p->next == NULL) { // element to be removed is at end
				f->next = NULL;
			}
			else if (p == front) { // element to be removed is at beginning
				front = front->next;
			}
			else { // element in middle
				f->next = p->next;
			}
			return p;
		}
		f = p;
	}

	return NULL;
}
