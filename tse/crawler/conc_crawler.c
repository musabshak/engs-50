/* conc_crawler.c -- Concurrent Web Crawler
 *
 * Author: Musab Shakeel & Selim Hassari
 *
 * Description:
 * Same functionality as crawler.c but is a multi-threaded implementation 
 * using pthreads. User can set number of threads to use
 * 
 * The crawler has the following usage: 
 * 		usage: crawler <seedurl> <pagedir> <maxdepth> <numthreads>
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <lqueue.h>
#include <lhash.h>
#include <pageio.h>
#include <webpage.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h> 

/* GLOBAL VARIABLES */
lhashtable_t *seen_urls_hp;
lqueue_t *webpages_qp;
int id;
int num_t_empty;
char *pagedir;
int max_depth;
int num_threads;

/* Declare mutexes */
pthread_mutex_t id_mutex; 			/* For page id */
pthread_mutex_t num_t_empty_mutex;  /* For number of currently done threads */
void print_webpage(void *page) {
	printf("queue_url: %s\n", webpage_getURL( (webpage_t *) page) );
}

/* HELPER FUNCTIONS */

void print_urlstring(void *elementp) {
	printf("hashed_url: %s\n", (char *) elementp);
}

bool search_for_url(void *elementp, const void *searchkeyp) {	
	if(strcmp( (char *) elementp, (char *) searchkeyp ) == 0) {
		return true;
	}
	else {
		return false;
	}
}

/* Function to check whether a directory exists or not.
 * It returns 1 if given path is an existing directory, 
 * otherwise returns 0.
 * https://codeforwin.org/2018/03/c-program-check-file-or-directory-exists-not.html
 */
bool isDirectoryExists(const char *path) {
    struct stat stats;
    stat(path, &stats);

    /* Check for file existence */
    if (S_ISDIR(stats.st_mode))
        return true;
    return false;
}

/* expand() gets webpages associated with URLs in the current page ("pagep") and
 * puts them in the webpage queue ("webpages_qp"). These webpages will have a 
 * depth one greater than the depth of the current page ("new_depth"). expand() 
 * also adds the seen URLs to a hashtable ("seen_urls_hp") which it uses to 
 * ensure that the same URL is not crawled more than once
 */
void expand(webpage_t *pagep, int depth) {
	int pos = 0;
	char *url_result;
	
	while ( (pos = webpage_getNextURL(pagep, pos, &url_result) ) > 0) {
		/* If URL is internal and not seen, put in hashtable and queue */
		if (IsInternalURL(url_result) &&
			lhadd( seen_urls_hp, search_for_url, url_result, strlen(url_result)
				 ) == NULL 
			) 
		{
			lqput(webpages_qp, webpage_new(url_result, depth, NULL));
		}
		else { /* Free the URL (external or seen) that doesn't make it to table */
			free(url_result);
		}
	}
}

/* Function that is passed to the threads. Close threads when *all* threads
 * indicate that they are currently done (i.e. the webpage queue that they 
 * have access to is empty). This is kept track of by the global variable: 
 * num_t_empty.This tracks that number of threads currently inactive. 
 * When num_t_empty == num_threads, close all threads.
 * 
 * Logic for incrementing num_t_empty:
 * 		[thread status last itr. && thread status current itr.: action]
 * 		done && !done: num_t_empty --
 * 		done && done: nothing
 * 		!done && !done: nothing
 * 		!done && done: num_t_empty ++
 * 
 * Mutexes are locked while 
 * 		- saving pages and incrementing page id
 * 		- incrementing/decrementing num_t_empty
 * 
 */
void *thread_fxn(void *argsp) {
	bool was_done = false; /* Thread saw empty queue in last iteration */
	while (num_t_empty != num_threads) {
		webpage_t *pagep = lqget(webpages_qp);
		if ( pagep != NULL ) {	
			if (was_done) { /* done && !done */
				pthread_mutex_lock(&num_t_empty_mutex);
				num_t_empty --;
				pthread_mutex_unlock(&num_t_empty_mutex);
				was_done = false;
			}
			if (!webpage_fetch(pagep)) {
				printf("UNABLE TO FETCH PAGE\n");
			}
			else {
				pthread_mutex_lock(&id_mutex);
				printf("Saving page #%d\n", id);
				if (pagesave(pagep, id, pagedir) != 0) {
					exit(EXIT_FAILURE);
				}
				id ++;
				pthread_mutex_unlock(&id_mutex);
				int current_depth = webpage_getDepth(pagep);
				if (current_depth < max_depth) {
					expand(pagep, current_depth+1);
				}
			}
			webpage_delete(pagep);
		}
		else {
			if (!was_done) { /* !done && done */
				pthread_mutex_lock(&num_t_empty_mutex);
				num_t_empty ++;
				pthread_mutex_unlock(&num_t_empty_mutex);
				was_done = true;
			}
		}
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	char usage[100] = "usage: crawler <seedurl> <pagedir> <maxdepth> <numthreads>";

	/* Check correct number of arguments */
	if (argc != 5) {
		printf("%s\n", usage);
		exit(EXIT_FAILURE);
	}

	/* Check that directory exists */	
	if (!isDirectoryExists(argv[2])) {
		printf("The specified directory (%s) does not exist\n", argv[2]);
		exit(EXIT_FAILURE);
  	}

	/* Check that maximum depth is non-negative */
	if (atoi(argv[3]) < 0) {
		printf("maxdepth should be >= 0\n");
		exit(EXIT_FAILURE);
	}

	/* Check that num threads is >= 1 */
	if (atoi(argv[3]) < 1) {
		printf("maxdepth should be >= 1\n");
		exit(EXIT_FAILURE);
	}

	char *seed_url = argv[1];
	pagedir = argv[2];
	max_depth = atoi(argv[3]);
	num_threads = atoi(argv[4]); /* Number of threads to use */

	seen_urls_hp = lhopen(10); /* Hashtable of seen URLs */
	webpages_qp = lqopen();	   /* Queue of all webpages at */
							   /* depth <= max_depth       */

	/* Make webpage object from specified seed_url at depth 0; put webpage in
	 * queue; put seed_url into hashtable of seen URLs
	 */
	webpage_t *first_page = webpage_new(seed_url, 0, NULL);
	lqput(webpages_qp, first_page);
	lhput(seen_urls_hp, seed_url, seed_url, strlen(seed_url));
	id = 1;

	/* Initialize mutexes */
	pthread_mutex_init(&id_mutex, NULL);
	pthread_mutex_init(&num_t_empty_mutex, NULL);

	/* Create threads and store in an array */
	pthread_t thread_array[num_threads];
	for (int i=0; i<num_threads; i++) {
		printf("Creating thread: %d\n", i+1);
		pthread_create(&thread_array[i], NULL, thread_fxn, NULL);
	}
	printf("Finished creating all threads\n");	
	for (int i=0; i<num_threads; i++) {
		pthread_join(thread_array[i], NULL);
	}
	pthread_mutex_destroy(&id_mutex);
	pthread_mutex_destroy(&num_t_empty_mutex);
	
	printf("All the %d hashed URLs: \n", id-1);
	lhapply(seen_urls_hp, print_urlstring);

	/* Remove seed_url from hashtable because it was manually added and was not 
	 * malloced whereas the rest of the URLs were
	 */
	lhremove(seen_urls_hp, search_for_url, seed_url, strlen(seed_url));
	
	lhapply(seen_urls_hp, free);
	lqclose(webpages_qp);
	lhclose(seen_urls_hp);
	
	exit(EXIT_SUCCESS);
}
