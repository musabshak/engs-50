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

void printelement(void* elementp) {
	person_t *tmp = (person_t *)elementp;
	printf("name: %s, age: %d, rate: %f\n", tmp->name, tmp->age, tmp->rate);
}

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

bool search_age(void* elementp, const void* keyp) {
	person_t *pp = (person_t *) elementp;
	int *age = (int *) keyp;
	
	if (pp->age == *age)  {
		return true;
	}
	else {
		return false;
	}
	
}

int main(void) {
	person_t *pp4, *pp5, *pp6;
	queue_t *qp;
	queue_t *qp2;
	queue_t *qp3;

	qp = qopen();
	/*pp1 = make_person("Fred", 20, 40.0);
	pp2 = make_person("Ali", 22, 100);
	pp3 = make_person("Suraj", 18, 50);
	qput(qp, pp1);
	qput(qp, pp2);
	qput(qp, pp3);*/

	qp2 = qopen();
	pp4 = make_person("1", 1, 1);
	pp5 = make_person("2", 2, 2);
	pp6 = make_person("3", 3, 3);
	qput(qp2, pp4);
	qput(qp2, pp5);
	qput(qp2, pp6);

	qp3 = qopen();
	qconcat(qp, qp3);

	qapply(qp, printelement);

	/*free(pp1);
	free(pp2);
	free(pp3);*/
	free(pp4);
	free(pp5);
	free(pp6);
	qclose(qp);
	qclose(qp2);
	qclose(qp3);
	
}
