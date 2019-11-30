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

#include <lqueue.h>
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

//open, close, put, get, apply, search, remove, concat

int main(void) {
	person_t *pp1, *pp2, *pp3, *pp4, *pp5, *pp6;
	lqueue_t *qp;
	lqueue_t *qp2;
	//lqueue_t *qp3;

	qp = lqopen();
	pp1 = make_person("Fred", 20, 40.0);
	pp2 = make_person("Ali", 22, 100);
	pp3 = make_person("Suraj", 18, 50);
	lqput(qp, pp1);
	lqput(qp, pp2);
	lqput(qp, pp3);
	qp2 = lqopen();
	pp4 = make_person("1", 1, 1);
	pp5 = make_person("2", 2, 2);
	pp6 = make_person("3", 3, 3);
	lqput(qp2, pp4);
	lqput(qp2, pp5);
	lqput(qp2, pp6);

	//qp3 = qopen();
	lqconcat(qp, qp2);

	lqapply(qp, printelement);

	free(pp1);
	free(pp2);
	free(pp3);
	free(pp4);
	free(pp5);
	free(pp6);
	lqclose(qp);
	
}
