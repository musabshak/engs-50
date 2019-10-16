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
	int h_size = 10;
	htp1 = hopen(h_size);

	person_t *people[h_size*10]; 
	for (int i=0; i<h_size*10; i++) {
		char index[h_size*10];
		sprintf(index, "person%d", i);
		people[i] = make_person(index, i, i*50); 
		hput(htp1, people[i], people[i]->name, sizeof(people[i]->name));
	}

	person_t *people_no[h_size*10]; 
	for (int i=h_size*10; i<h_size*20; i++) {
		char index[h_size*10];
		sprintf(index, "person%d", i);
		people_no[i-(h_size*10)] = make_person(index, i, i*50); 
	}

	happly(htp1, print_queue_element);
	printf("-------------\n");

	for (int i=0; i<h_size*10; i++) {
		person_t *tmp = (person_t *) hremove(htp1, search_name, people[i]->name, sizeof(people[i]->name));
		int check = strcmp(tmp->name, people[i]->name);
		if (check != 0) {
			printf("Error in remove\n");
		}
	}
	
	for (int i=0; i<h_size*10; i++) {
		person_t *tmp = (person_t *) hsearch(htp1, search_name, people_no[i]->name, sizeof(people_no[i]->name));
		void *ans = hremove(htp1, search_name, people_no[i]->name, sizeof(people_no[i]->name));
		if (ans != NULL) {
			printf("Error\n");
			}
	}
	
	for (int i=0; i<h_size*10; i++) {
		hremove(htp1, search_name, people[i]->name, sizeof(people[i]->name));
	}

	for (int i=0; i<h_size*10; i++) {
		person_t *tmp = (person_t *) hsearch(htp1, search_name, people_no[i]->name, sizeof(people_no[i]->name));
		void *ans = hremove(htp1, search_name, people_no[i]->name, sizeof(people_no[i]->name));
		if (ans != NULL) {
			printf("Error\n");
			}
	}
	
	
	happly(htp1, print_queue_element);
	printf("-------------\n");
	
	hclose(htp1);

	for (int i=0; i<h_size*10; i++) {
		free(people[i]);
		free(people_no[i]);
	};
	
	exit(EXIT_SUCCESS);

	
	
}



