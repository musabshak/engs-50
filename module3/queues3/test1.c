/* test1.c --- 
 * 
 * 
 * Author: Musab Shakeel
 * Created: Sun Oct  6 16:48:50 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <queue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct person {
	char name[20];
	int age;
	double rate;
} person_t;

person_t* make_person(char *name, int age, double rate) {
	person_t *personp;

	if (!(personp = (person_t *) malloc(sizeof(person_t)))) {
		return NULL;
	}

	strcpy(personp->name, name);
	personp->age = age;
	personp->rate = rate;

	return personp;
}

int main(void) {
	person_t *pp;
	queue_t *qp;

	printf("hi\n");

	qp = qopen();
	pp = make_person("Fred", 20, 20.0);
	printf("howaboutnow\n");
	qput(qp, (void*) pp); // Queue is agnostic of the data passed to it
	printf("and now\n");

	pp = (person_t*) qget(qp); // qget() returns void *
	printf("comeon\n");
	qclose(qp);

}
