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


struct Node {
	int data;
	struct Node *next;
};

struct Queue {
	struct Node *front;
	struct Node *back;
	int size;
};

struct Queue* qopen(void){
struct  Queue q;
q->front= NULL;
q->back=NULL;
q->size = 0;
return q;
}

void main() {

}

