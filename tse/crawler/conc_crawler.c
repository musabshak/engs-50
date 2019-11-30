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
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
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

lhashtable_t *seen_urls_hp;
lqueue_t *webpages_qp;
int id;
int num_t_empty;
char *pagedir;
int max_depth;
int num_threads;

pthread_mutex_t id_mutex;
pthread_mutex_t num_t_empty_mutex;




void expand(webpage_t *pagep, int depth) {
	int pos = 0;
	char *url_result;
	
	// webpage_getNextURL() mallocs url_result that user needs to free
	while ( (pos = webpage_getNextURL(pagep, pos, &url_result) ) > 0) {
		// printf("Found %s URL: %s\n",
		// 		    IsInternalURL(url_result) ? "Internal" : "External",
		// 		    url_result);

		// If url not seen and not internal, put in hashtable and queue

	
		// bool seen = false;
		// if (lhsearch(seen_urls_hp, search_for_url, url_result, strlen(url_result)) != NULL) {
		// 	seen = true;
		// }
		
		// if (IsInternalURL(url_result) && !seen) {
		// 	lhput(seen_urls_hp, url_result, url_result, strlen(url_result));
		// 	lqput(webpages_qp, webpage_new(url_result, depth, NULL));
		//  }

		if (IsInternalURL(url_result) && lhadd(seen_urls_hp, search_for_url, url_result, strlen(url_result)) == NULL ) {
			lqput(webpages_qp, webpage_new(url_result, depth, NULL));
		}

		else { // Free the urls (external or seen) that don't make it into the hashtable
			free(url_result);
		}
	}
}

void *thread_fxn(void *argsp) {

	bool was_done = false;
	while (num_t_empty!=num_threads) {
		webpage_t *pagep = lqget(webpages_qp);
		if ( pagep != NULL ) {	
			if (was_done) {
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
				printf("Printing %d\n", id);
				
				if (pagesave(pagep, id, pagedir) != 0) {
					exit(EXIT_FAILURE);
				}
				//pthread_mutex_lock(&id_mutex);
				id ++;
				pthread_mutex_unlock(&id_mutex);
				
				int current_depth = webpage_getDepth(pagep);
				//printf("%d\n", current_depth);
				if (current_depth < max_depth) {
					expand(pagep, current_depth+1);
				}
			
				//lqapply(webpages_qp, print_webpage);
				//printf("------\n");
	
			}
			webpage_delete(pagep);
		}
		else {
			//printf("Selim's BLACK MAGIC\n");
			// int a = 5;
			// a *= 10000;
			if (!was_done) {
				pthread_mutex_lock(&num_t_empty_mutex);
				num_t_empty ++;
				pthread_mutex_unlock(&num_t_empty_mutex);
				was_done = true;
			}
		}
		//webpage_delete(pagep);

	}
	return NULL;
}

int main(int argc, char *argv[]) {
	char usage[50] = "usage: crawler <seedurl> <pagedir> <maxdepth>";

	if (argc != 4) {
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
	pagedir = argv[2];
	max_depth = atoi(argv[3]);

	seen_urls_hp = lhopen(10);
	webpages_qp = lqopen();

	webpage_t *first_page = webpage_new(seed_url, 0, NULL);
	lqput(webpages_qp, first_page);
	lhput(seen_urls_hp, seed_url, seed_url, strlen(seed_url));
	id = 1;
	num_threads = 10;


	pthread_mutex_init(&id_mutex, NULL);
	pthread_mutex_init(&num_t_empty_mutex, NULL);
	pthread_t thread_array[num_threads];

	for (int i=0; i<num_threads; i++) {
		printf("Creating thread: %d\n", i);
		pthread_create(&thread_array[i], NULL, thread_fxn, NULL);
	}
	
	for (int i=0; i<num_threads; i++) {
		pthread_join(thread_array[i], NULL);
	}

	printf("Finished all threads\n");	
	printf("REACHED THE END OF WHILE LOOP\n");

	pthread_mutex_destroy(&id_mutex);
	pthread_mutex_destroy(&num_t_empty_mutex);
	
	lhapply(seen_urls_hp, print_urlstring);

	lhremove(seen_urls_hp, search_for_url, seed_url, strlen(seed_url));
	//char *result2 = lhremove(seen_urls_hp, search_for_url, seed_url, strlen(seed_url));
	//printf("RESULT: %s\n", result2);
	
	lhapply(seen_urls_hp, free_the_urls);
	//happly(seen_urls_hp, print_urlstring);
	
	lqclose(webpages_qp);
	lhclose(seen_urls_hp);
	
	exit(EXIT_SUCCESS);
}
