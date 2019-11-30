#include <queue.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct lqueue {
	queue_t * qp;
	pthread_mutex_t mutex;

} lqueue_t;

/* create an empty queue */
lqueue_t* lqopen(void) {
	lqueue_t *lqp;

	if (!(lqp = (lqueue_t *) malloc(sizeof(lqueue_t)))) {
		return NULL;
	}
	lqp->qp = qopen();
	pthread_mutex_init(&lqp->mutex,NULL);
	return lqp;
}

/* deallocate a queue, frees everything in it */
void lqclose(lqueue_t *lqp) {
	qclose(lqp->qp);
	pthread_mutex_destroy(&lqp->mutex);
	free(lqp);
}

/* put element at the end of the queue
 * returns 0 is successful; nonzero otherwise
 */
int32_t lqput(lqueue_t *lqp, void *elementp) {
	pthread_mutex_lock(&lqp->mutex);
	int result = qput(lqp->qp, elementp);
	pthread_mutex_unlock(&lqp->mutex);
	return result;
}

/* get the first first element from queue, removing it from the queue */
void* lqget(lqueue_t *lqp) {
	pthread_mutex_lock(&lqp->mutex);
	void *tmp = qget(lqp->qp);
	pthread_mutex_unlock(&lqp->mutex);
	return tmp;
}

/* apply a function to every element of the queue */
void lqapply(lqueue_t *lqp, void (*fn)(void* elementp)) {
	pthread_mutex_lock(&lqp->mutex);
	qapply(lqp->qp, fn);
	pthread_mutex_unlock(&lqp->mutex);
}

/* search a queue using a supplied boolean function
 * skeyp -- a key to search for
 * searchfn -- a function applied to every element of the queue
 *          -- elementp - a pointer to an element
 *          -- keyp - the key being searched for (i.e. will be
 *             set to skey at each step of the search
 *          -- returns TRUE or FALSE as defined in bool.h
 * returns a pointer to an element, or NULL if not found
 */
void* lqsearch(lqueue_t *lqp,
							bool (*searchfn)(void* elementp,const void* keyp),
							const void* skeyp) {
	pthread_mutex_lock(&lqp->mutex);
	void *tmp = qsearch(lqp->qp, searchfn, skeyp);
	pthread_mutex_unlock(&lqp->mutex);
	return tmp;
}

/* search a queue using a supplied boolean function (as in qsearch),
 * removes the element from the queue and returns a pointer to it or
 * NULL if not found
 */
void* lqremove(lqueue_t *lqp,
							bool (*searchfn)(void* elementp,const void* keyp),
							const void* skeyp) {
	pthread_mutex_lock(&lqp->mutex);
	void *tmp = qremove(lqp->qp, searchfn, skeyp);
	pthread_mutex_unlock(&lqp->mutex);
	return tmp;
}

/* concatenates elements of q2 into q1
 * q2 is deallocated, closed, and unusable upon completion
 */
void lqconcat(lqueue_t *lq1p, lqueue_t *lq2p) {
	pthread_mutex_lock(&lq1p->mutex);
	pthread_mutex_lock(&lq2p->mutex);

	qconcat(lq1p->qp, lq2p->qp);
	free(lq2p);

	pthread_mutex_unlock(&lq1p->mutex);
	pthread_mutex_unlock(&lq2p->mutex);

}
























