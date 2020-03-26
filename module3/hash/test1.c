/* test1.c --- 
 * 
 * 
 * Author: Musab Shakeel
 * Created: Fri Oct 11 16:01:52 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <hash.h>
#include <queue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct person {
	char name[20];
	int age;
	double rate;
} person_t;

void print_queue_element(void* elementp) {
	// element is a queue now
	
	person_t *tmp = (person_t *)elementp;
	printf("%s; ", tmp->name);
}

void print_htp_element(void* elementp) {
	queue_t *qp = (queue_t *) elementp;
	
	qapply(qp, print_queue_element);
	printf("\n");
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

bool search_name(void* elementp, const void* keyp) {
	person_t *pp = (person_t *) elementp;
	char name[20];
	strcpy(name, keyp);

	if (strcmp(pp->name,name)==0)  {
		return true;
	}
	else {
		return false;
	}
	
}

int main() {

	hashtable_t *htp1;

	person_t *pp1 = make_person("Fred", 20, 50);
	person_t *pp2 = make_person("Farid", 90, 100);
	person_t *pp3 = make_person("Farrukh", 50, 150);
	person_t *pp4 = make_person("Alan", 20, 50);
	person_t *pp5 = make_person("Stjep", 40, 100);
	person_t *pp6 = make_person("Maria", 50, 150);
	person_t *pp7 = make_person("Shakeela", 50, 150);
	person_t *pp8 = make_person("El", 50, 150);
	person_t *pp9 = make_person("Selim", 19, 150);

	
	htp1 = hopen(8);
 
	hput(htp1, pp1, pp1->name, 4);
	hput(htp1, pp2, pp2->name, 5);
	hput(htp1, pp3, pp3->name, 7);
	hput(htp1, pp4, pp4->name, 4);
	hput(htp1, pp5, pp5->name, 5);
	hput(htp1, pp6, pp6->name, 5);
	hput(htp1, pp7, pp7->name, 8);
	hput(htp1, pp8, pp8->name, 2);
	hput(htp1, pp9, pp9->name, 5); 

	
	happly(htp1, print_htp_element);

	hclose(htp1);
	
	free(pp1);
	free(pp2);
	free(pp3);
	free(pp4);
	free(pp5);
	free(pp6);
	free(pp7);
	free(pp8);
	free(pp9);
	
	exit(EXIT_SUCCESS);

}



