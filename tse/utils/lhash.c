/* 
 * hash.c -- implements a generic hash table as an indexed set of queues.
Author: Musab Shakeel (12:50)
 *
 */
#include <stdint.h>
#include <queue.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <hash.h>


typedef struct lhashtable {
	hashtable_t *htp;
	pthread_mutex_t mutex;
} lhashtable_t;


lhashtable_t* lhopen(uint32_t hsize) {
	lhashtable_t *lhtp;
	if (!(lhtp = (lhashtable_t*) malloc(sizeof(lhashtable_t)))) {
		return NULL;
	}
	lhtp->htp = hopen(hsize);
	pthread_mutex_init(&lhtp->mutex, NULL);
	return lhtp;
}

int32_t lhput(lhashtable_t *lhtp, void *ep, const char *key, int keylen) {
	pthread_mutex_lock(&lhtp->mutex);
	hput(lhtp->htp, ep, key, keylen);
	pthread_mutex_unlock(&lhtp->mutex);
	return 0;
}

void lhapply(lhashtable_t *lhtp, void (*fn)(void* ep)) {
	pthread_mutex_lock(&lhtp->mutex);
	happly(lhtp->htp, fn);
	pthread_mutex_unlock(&lhtp->mutex);
}

void lhclose(lhashtable_t *lhtp) {
	hclose(lhtp->htp);
	pthread_mutex_destroy(&lhtp->mutex);
	free(lhtp);
}

void *lhsearch(lhashtable_t *lhtp,
							bool (*searchfn)(void* elementp, const void* searchkeyp),
							const char *key,int32_t keylen){

	pthread_mutex_lock(&lhtp->mutex);
	void *tmp = hsearch(lhtp->htp, searchfn, key, keylen);
	pthread_mutex_unlock(&lhtp->mutex);
	return tmp;
}

void *lhadd(lhashtable_t *lhtp,
							bool (*searchfn)(void* elementp, const void* searchkeyp),
							const char *key,int32_t keylen) {
	pthread_mutex_lock(&lhtp->mutex);
	void *output;
	output = hsearch(lhtp->htp, searchfn, key, keylen);
	if (output==NULL) { /* If not in hashtable already */
		hput(lhtp->htp, (void *) key, key, keylen);
	}
	pthread_mutex_unlock(&lhtp->mutex);
	return output; 
	
}

void *lhremove(lhashtable_t *lhtp,
							bool (*searchfn)(void* elementp, const void* searchkeyp),
							const char *key,
							int32_t keylen) {
	pthread_mutex_lock(&lhtp->mutex);
	void *tmp = hremove(lhtp->htp, searchfn, key, keylen);
	pthread_mutex_unlock(&lhtp->mutex);
	return tmp;
}















