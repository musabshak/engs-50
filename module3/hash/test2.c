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

void print_string(void *elementp) {
	printf("%s; ", (char *) elementp);
}

bool search_string(void *elementp, const void* searchkeyp) {
	return strcmp((char*) elementp, (char *) searchkeyp);
}

int main() {

	hashtable_t *htp1;

 
	htp1 = hopen(3);

	char str1[10] = "hello1";
	char str2[10] = "hello2";
	char str3[10] = "hello3";
 
	hput(htp1, str1, str1, sizeof(str1) );
	hput(htp1, str2, str2, sizeof(str2) );
	hput(htp1, str3, str3, sizeof(str3) );

	
	happly(htp1, print_string);
		
	printf("---\n");


	char *tmp = (char *) hsearch(htp1, search_string, str1, sizeof(str1));
	printf("FOUND: %s\n", tmp);


	hclose(htp1);
		
	exit(EXIT_SUCCESS);

}



