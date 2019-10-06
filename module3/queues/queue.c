/* queue.c --- 
 * 
 * 
 * Author: Selim Hassairi
 * Created: Fri Oct  4 13:51:47 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <queue.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int data;
	struct Node *next;
} node_t;


struct queue_t {
	node_t *front;
	node_t *back;
	int size;
};

queue_t* qopen(void){
	queue_t* q;
	if(!(q = (queue_t*)malloc(sizeof(queue_t)))){
		printf("[Error malloc failed allocating q\n]");
		return NULL;
	}	
	q->front=NULL;
	q->back=NULL;
	q->size = 0;
	return q;
}



