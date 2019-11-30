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
#include <queue.h>
#include <hash.h>
#include <pageio.h>
#include <webpage.h>
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

/*
 * seen_urls_hp: Hashtable of URLs seen so far
 * webpages_qp: Queue of all webpages at a depth <= max depth
 * pagep: The page obtained from webpages_qp that is currently being iterated over in the main()
 * 		  function
 * depth: Depth of pagep
 */
void expand(hashtable_t *seen_urls_hp, queue_t *webpages_qp, webpage_t *pagep, int depth) {
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

		// If url not seen, put in hashtable and queue
		bool seen = false;
		if (hsearch(seen_urls_hp, search_for_url, url_result, strlen(url_result)) != NULL) {
			seen = true;
		}
		
		if (IsInternalURL(url_result) && !seen) {
			hput(seen_urls_hp, url_result, url_result, strlen(url_result));
			qput(webpages_qp, webpage_new(url_result, depth, NULL));
		 }
		else { // Free the urls (external or seen) that don't make it into the hashtable
			free(url_result);
		}
	}
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
	char *pagedir = argv[2];
	int max_depth = atoi(argv[3]);

	hashtable_t *seen_urls_hp = hopen(10);
	queue_t *webpages_qp = qopen();

	webpage_t *first_page = webpage_new(seed_url, 0, NULL);
	qput(webpages_qp, first_page);
	hput(seen_urls_hp, seed_url, seed_url, strlen(seed_url));

	// char *result1 = (char *) hsearch(seen_urls_hp, search_for_url, seed_url, strlen(seed_url));
	// printf("RESULT: %s\n", result1);

	int id = 1;
	webpage_t *pagep = (webpage_t *) qget(webpages_qp);
	while (pagep != NULL) {
		
		if (!webpage_fetch(pagep)) {
			printf("UNABLE TO FETCH PAGE\n");
		}
		else {
			if (pagesave(pagep, id, pagedir) != 0) {
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
		
			qapply(webpages_qp, print_webpage);
			printf("------\n");
	 
			id += 1;
		}
		webpage_delete(pagep);
		pagep = qget(webpages_qp);
	}
	
	printf("REACHED THE END OF WHILE LOOP\n");
	
	happly(seen_urls_hp, print_urlstring);

	char *result2 = (char *) hremove(seen_urls_hp, search_for_url, seed_url, strlen(seed_url));
	// printf("RESULT: %s\n", result2);
	
	happly(seen_urls_hp, free_the_urls);
	//happly(seen_urls_hp, print_urlstring);
	
	qclose(webpages_qp);
	hclose(seen_urls_hp);
	
	exit(EXIT_SUCCESS);
}
