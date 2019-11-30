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

#include <lhash.h>
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
	
	lhashtable_t *htp1;
	int h_size = 10;
	htp1 = lhopen(h_size);

	person_t *people[h_size*10]; 
	for (int i=0; i<h_size*10; i++) {
		char index[h_size*10];
		sprintf(index, "person%d", i);
		people[i] = make_person(index, i, i*50); 
		lhput(htp1, people[i], people[i]->name, sizeof(people[i]->name));
	}

	person_t *people_no[h_size*10]; 
	for (int i=h_size*10; i<h_size*20; i++) {
		char index[h_size*10];
		sprintf(index, "person%d", i);
		people_no[i-(h_size*10)] = make_person(index, i, i*50); 
	}

	lhapply(htp1, print_queue_element);
	printf("-------------\n");

	for (int i=0; i<h_size*10; i++) {
		person_t *tmp = (person_t *) lhremove(htp1, search_name, people[i]->name, sizeof(people[i]->name));
		int check = strcmp(tmp->name, people[i]->name);
		if (check != 0) {
			printf("Error in remove\n");
		}
	}
	
	for (int i=0; i<h_size*10; i++) {
		person_t *tmp = (person_t *) lhsearch(htp1, search_name, people_no[i]->name, sizeof(people_no[i]->name));
		void *ans = lhremove(htp1, search_name, people_no[i]->name, sizeof(people_no[i]->name));
		if (ans != NULL) {
			printf("Error\n");
			}
	}
	
	for (int i=0; i<h_size*10; i++) {
		lhremove(htp1, search_name, people[i]->name, sizeof(people[i]->name));
	}

	for (int i=0; i<h_size*10; i++) {
		person_t *tmp = (person_t *) lhsearch(htp1, search_name, people_no[i]->name, sizeof(people_no[i]->name));
		void *ans = lhremove(htp1, search_name, people_no[i]->name, sizeof(people_no[i]->name));
		if (ans != NULL) {
			printf("Error\n");
			}
	}
	
	
	lhapply(htp1, print_queue_element);
	printf("-------------\n");
	
	lhclose(htp1);

	for (int i=0; i<h_size*10; i++) {
		free(people[i]);
		free(people_no[i]);
	};
	
	exit(EXIT_SUCCESS);

	
	
}



