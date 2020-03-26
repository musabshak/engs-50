/* queue.c --- 
 * 
 * 
 * Author: Musab Shakeel
 * Created: Sun Oct  6 16:30:26 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct queue {
	struct node *front;
	struct node *back;
} queue_t; 

typedef struct node {
	void *data;
	struct node *next;
} node_t;

node_t* data_to_node(void *elementp) {
	node_t *nodep;

	if (!(nodep = (node_t *) malloc(sizeof(node_t)))) {
		return NULL;
	}

	nodep->data = elementp;

	return nodep;
}

queue_t* qopen(void) {
	queue_t *qp;

	if (!(qp = (queue_t *) malloc(sizeof(queue_t)))) {
		return NULL;
	}
	printf("hello\n");
	return qp;
}

void qclose(queue_t *qp) {
	free(qp);
}

int32_t qput(queue_t *qp, void *elementp) {
	node_t *nodep = data_to_node(elementp);
	
	if (qp->front==NULL && qp->back==NULL) { // empty queue
		qp->front = nodep;
		qp->back = nodep;
	}
	else { // non-empty queue
		qp->back->next = nodep;
		qp->back = nodep;
	}

	return 0;
}

void* qget(queue_t *qp) {
	void *tmp = qp->front->data;
	qp->front = qp->front->next;
	printf("here\n");
	
	return tmp;
}

void qapply(queue_t *qp, void(*fn)(void* elementp)) {
	node_t *np;

	for (np=qp->front; np!=NULL; np=np->next) {
		fn(np->data);
	}
}

void* qsearch(queue_t *qp,
							bool (*searchfn)(void* elementp, const void* keyp),
							const void* skep) {
	node_t *np;

	for (np=qp->front; np!=NULL; np=np->next) {

//TODO: figuring outsearchfn parameters + keys 
		if (searchfn(np->data, "hi")) {
			return np->data;
		}
	}

	return NULL;
}

void* qremove(queue_t *qp,
							bool (*searchfn)(void* elementp, const void* keyp),
							const void* skeyp) {
	node_t *np;
	node_t *nf;

	for (np=qp->front; np!=NULL; np=np->next) {
//TODO: figuring outsearchfn parameters + keys 
		if (searchfn(np->data, "hi")) {
			if (qp->back==np) { // element at end
				nf->next = NULL;
				qp->back = np;
			}
			else if (qp->front==np) { // element at front
				qp->front = np->next;
			}
			else { // element in middle
				nf->next = np->next;
			}
			return np->data;
		}
		nf = np;
	}

	return NULL;
}

void qconcat(queue_t *q1p, queue_t *q2p) {

	q1p->back->next = q2p->front;
	q1p->back = q2p->back;
	qclose(q2p);
}
