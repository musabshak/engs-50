/* crawler.c --- 
 * 
 * 
 * Author: Musab Shakeel & Selim Hassari
 * Created: Wed Oct 16 15:11:12 2019 (-0400)
 * Version: 1
 * 
 * Description: Web Crawler 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <lqueue.h>
#include <lhash.h>
#include <pageio.h>
#include <webpage.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h> // For stat; checking if directory exists

void print_webpage(void *page) {
	printf("queue_url: %s\n", webpage_getURL( (webpage_t *) page) );
}

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

void free_the_urls(void *elementp) {
	free(elementp);
}

/*
 * Function to check whether a directory exists or not.
 * It returns 1 if given path is directory and exists
 * otherwise returns 0.
 * https://codeforwin.org/2018/03/c-program-check-file-or-directory-exists-not.html
 */
bool isDirectoryExists(const char *path)
{
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return true;

    return false;
}

static lhashtable_t *seen_urls_hp;
static lqueue_t *webpages_qp;
static int ID;
static pthread_mutex_t ID_mutex;
static int NUM_T_EMPTY;
static pthread_mutex_t NUM_T_EMPTY_mutex;

/*
 * seen_urls_hp: Hashtable of URLs seen so far
 * webpages_qp: Queue of all webpages at a depth <= max depth
 * pagep: The page obtained from webpages_qp that is currently being iterated over in the main()
 * 		  function
 * depth: Depth of pagep
 */
void expand(lhashtable_t *seen_urls_hp, lqueue_t *webpages_qp, webpage_t *pagep, int depth) {
	int pos = 0;
	char *url_result;
	/*
	 * This loop iterates over URLs found in the current webpage: pagep
	 */
	/* webpage_getNextURL() mallocs url_result that user needs to free */
	while ( (pos = webpage_getNextURL(pagep, pos, &url_result) ) > 0) {
		printf("Found %s URL: %s\n",
				    IsInternalURL(url_result) ? "Internal" : "External",
				    url_result);
		/* Check if URL has been seen */
		bool seen = false;
		if (lhsearch(seen_urls_hp, search_for_url, url_result, strlen(url_result)) != NULL) {
			seen = true;
		}
		/* If URL not seen and is internal, add to hashtable and queue */
		if (IsInternalURL(url_result) && !seen) {
			lhput(seen_urls_hp, url_result, url_result, strlen(url_result));
			lqput(webpages_qp, webpage_new(url_result, depth, NULL));
		 }
		else { // Free the urls (external or seen) that don't make it into the hashtable
			free(url_result);
		}
	}
}

typedef struct thread_args {
	char pagedir[100];
	int max_depth;
	int num_threads;
} thread_args_t;

thread_args_t *make_thread_args(char *pagedir, int max_depth, int num_threads) {
	thread_args_t *args = (thread_args_t *) malloc(sizeof(thread_args_t));

	strcpy(args->pagedir, pagedir);
	args->max_depth = max_depth;
	args->num_threads = num_threads;

	return args;
}

void *thread_fxn(void *argsp) {


	thread_args_t *args = (thread_args_t *) argsp;
	char *pagedir = args->pagedir;
	int max_depth = args->max_depth;
	int num_threads = args->num_threads;

	bool was_done = false;

//	webpage_t *pagep = (webpage_t *) lqget(webpages_qp);
//	while (NUM_T_EMPTY != 2) {
//		if (pagep == NULL) {
//			pthread_mutex_lock(&NUM_T_EMPTY_mutex);
//			NUM_T_EMPTY += 1;
//			pthread_mutex_unlock(&NUM_T_EMPTY_mutex);
//			pagep = lqget(webpages_qp);
//			was_done = true;
//			continue;
//		}
//
//		if (was_done) {
//			pthread_mutex_lock(&NUM_T_EMPTY_mutex);
//			NUM_T_EMPTY -= 1;
//			pthread_mutex_unlock(&NUM_T_EMPTY_mutex);
//		}
//
//		if (!webpage_fetch(pagep)) {
//			printf("UNABLE TO FETCH PAGE\n");
//		}
//		else {
//			/* Save page to directory */
//			if (pagesave(pagep, ID, pagedir) != 0) {
//				exit(EXIT_FAILURE);
//			}
//			int current_depth = webpage_getDepth(pagep);
//			printf("%d\n", current_depth);
//			if (current_depth < max_depth) {
//				/* expand() gets webpages associated with URLs in the current page and puts
//				 * them in the queue. These webpages will have a depth one greater than the depth
//				 * of the current page. expand() also adds the seen URLs to a hashtable which it uses
//				 * to ensure that the same URL is not crawled more than once
//				 */
//				expand(seen_urls_hp, webpages_qp, pagep, current_depth+1);
//			}
//			lqapply(webpages_qp, print_webpage);
//			printf("------\n");
//			pthread_mutex_lock(&ID_mutex);
//			ID += 1;
//			pthread_mutex_unlock(&ID_mutex);
//		}
//		webpage_delete(pagep);
//		pagep = lqget(webpages_qp);
//	}

	webpage_t *pagep;
	while (NUM_T_EMPTY != 2) {
		//printf("NUM_T_EMPTY: %d\n", NUM_T_EMPTY);
		if ( (pagep = lqget(webpages_qp)) != NULL) {
			if (was_done) {
				pthread_mutex_lock(&NUM_T_EMPTY_mutex);
				NUM_T_EMPTY -= 1;
				pthread_mutex_unlock(&NUM_T_EMPTY_mutex);
				was_done = false;
			}
			if (!webpage_fetch(pagep)) {
				printf("UNABLE TO FETCH PAGE\n");
			}
			else {
				/* Save page to directory */
				if (pagesave(pagep, ID, pagedir) != 0) {
					printf("PAGE UNABLE TO BE SAVED\n");
					exit(EXIT_FAILURE);
				}
				int current_depth = webpage_getDepth(pagep);
				printf("%d\n", current_depth);
				if (current_depth < max_depth) {
					/* expand() gets webpages associated with URLs in the current page and puts
					 * them in the queue. These webpages will have a depth one greater than the depth
					 * of the current page. expand() also adds the seen URLs to a hashtable which it uses
					 * to ensure that the same URL is not crawled more than once
					 */
					expand(seen_urls_hp, webpages_qp, pagep, current_depth+1);
				}
				//lqapply(webpages_qp, print_webpage);
				printf("------\n");
				pthread_mutex_lock(&ID_mutex);
				ID += 1;
				pthread_mutex_unlock(&ID_mutex);
			}
			webpage_delete(pagep);
		}

		else {
			if (!was_done) {
				pthread_mutex_lock(&NUM_T_EMPTY_mutex);
				NUM_T_EMPTY += 1;
				pthread_mutex_unlock(&NUM_T_EMPTY_mutex);
				was_done = true;
			}
		}
	}

	return NULL;
}


int main(int argc, char *argv[]) {
	char usage[400] = "usage: crawler <seedurl> <pagedir> <maxdepth> <num_threads>";
	if (argc != 5) {
		printf("%s\n", usage);
		exit(EXIT_FAILURE);
	}
	if (!isDirectoryExists(argv[2])) {
		printf("The specified directory (%s) does not exist\n", argv[2]);
		exit(EXIT_FAILURE);
	}
	if (atoi(argv[3]) < 0) {
		printf("maxdepth should be > 0\n");
		exit(EXIT_FAILURE);
	}
	char *seed_url = argv[1];
	char *pagedir = argv[2];
	int max_depth = atoi(argv[3]);
	int num_threads = atoi(argv[4]);

	seen_urls_hp = lhopen(10);
	webpages_qp = lqopen();
	webpage_t *first_page = webpage_new(seed_url, 0, NULL);
	lqput(webpages_qp, first_page);
	lhput(seen_urls_hp, seed_url, seed_url, strlen(seed_url));


	pthread_t tid1, tid2, tid3, tid4;

	ID = 1;
	NUM_T_EMPTY = 0;
	pthread_mutex_init(&ID_mutex, NULL);
	pthread_mutex_init(&NUM_T_EMPTY_mutex, NULL);

	thread_args_t *args = make_thread_args(pagedir, max_depth, num_threads);


//	for (int i=0; i<num_threads; i++)
//
//	for (int i=0; i<num_threads; i++) {
//		if(pthread_create(&tid1, NULL, thread_fxn, args)!=0) {
//			exit(EXIT_FAILURE);
//		}
//	}

	if(pthread_create(&tid1, NULL, thread_fxn, args)!=0)  {
			exit(EXIT_FAILURE);
		}
	if(pthread_create(&tid2, NULL, thread_fxn, args)!=0)  {
		exit(EXIT_FAILURE);
	}
//	if(pthread_create(&tid3, NULL, thread_fxn, args)!=0)  {
//				exit(EXIT_FAILURE);
//	}

	if(pthread_join(tid1, NULL)!=0){
	        exit(EXIT_FAILURE);
	    }
	if(pthread_join(tid2, NULL)!=0){
		exit(EXIT_FAILURE);
	}
//	if(pthread_join(tid3, NULL)!=0){
//			exit(EXIT_FAILURE);
//	}


	printf("All threads finished\n");
	pthread_mutex_destroy(&ID_mutex);
	pthread_mutex_destroy(&NUM_T_EMPTY_mutex);
	free(args);
	

	printf("REACHED THE END OF WHILE LOOP\n");
	
	lhapply(seen_urls_hp, print_urlstring);

	char *result2 = (char *) lhremove(seen_urls_hp, search_for_url, seed_url, strlen(seed_url));
	printf("RESULT: %s\n", result2);
	
	lhapply(seen_urls_hp, free_the_urls);
	//happly(seen_urls_hp, print_urlstring);
	
	lqclose(webpages_qp);
	lhclose(seen_urls_hp);
	
	exit(EXIT_SUCCESS);
}
