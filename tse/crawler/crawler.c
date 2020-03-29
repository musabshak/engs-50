/* crawler.c -- Web Crawler
 *
 * Author: Musab Shakeel & Selim Hassari
 * Created: Wed Oct 16 15:11:12 2019 (-0400)
 *
 * Description:
 * Fetches a webpage from a given seed_url, extracts any embedded URLs, 
 * retrieves each of those pages iteratively, storing associated files in a 
 * designated directory as a numbered file. Search is limited to a given depth
 *
 * Usage:
 * 		crawler <seedurl> <pagedir> <maxdepth> 
 * 
 * • seedurl is the starting url where the crawl begins
 * • pagedir is where the saved pages are stored
 * • maxdepth is the maximum depth that the crawler may crawl; it should be 
 * 	 nonnegative
 * 
 */

#include <stdio.h>    /* printf(), scanf(), gets() etc */
#include <stdlib.h>   /* EXIT_FAILURE, atoi(), free(), malloc() etc */
#include <queue.h>
#include <hash.h>
#include <pageio.h>   /* pagesave(), pageload() */
#include <webpage.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>  /* For stat: (checking if directory exists */


/* HELPER FUNCTIONS */

/* Given a webpage, print webpage URL */
void print_webpage(void *page) {
	printf("queue_url: %s\n", webpage_getURL( (webpage_t *) page) );
}

 /* Given a URL string (retrieved from hashtable), print it */
void print_urlstring(void *elementp) {
	printf("hashed_url: %s\n", (char *) elementp);
}

/* Use with hsearch(). Returns true if key searched for ("searchkeyp")
 * matches the given hashtable object attribute ("elementp"). Since the 
 * decided attribute is URL i.e. a string, strcmp is used
 */
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
 * otherwise returns 0
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
 *
 * Variables:
 * 		seen_urls_hp: Hashtable of URLs seen so far
 * 		webpages_qp: Queue of all webpages at a depth <= max depth
 * 		pagep: The page obtained from webpages_qp that is currently being
 * 			    iterated over in the main() funtion
 * 		new_depth: Depth of pagep + 1
 */
void expand(hashtable_t *seen_urls_hp, queue_t *webpages_qp, 
 			webpage_t *pagep, int new_depth) {
	int pos = 0;
	char *url_result;
	
	/* Iterate over URLs found in the current webpage: pagep. webpage_getNextURL()
	 * mallocs url_result that we need to free. This is done either at the end of
	 * this function (for the URLs that don't make it into the hashtable) or
	 * at the end of the main() function (for the URLs that do make it)
	 */
	while ( (pos = webpage_getNextURL(pagep, pos, &url_result) ) > 0) {
		//printf("Found %s URL: %s\n", 
		//IsInternalURL(url_result) ? "Internal" : "External", url_result);

		/* If URL not seen before, put in hashtable and queue */
		bool seen = false;
		if (hsearch(seen_urls_hp, search_for_url, url_result, strlen(url_result))
		 	!= NULL) {
			seen = true;
		}
		if (IsInternalURL(url_result) && !seen) {
			hput(seen_urls_hp, url_result, url_result, strlen(url_result));
			/* Create new webpage and put into queue. webpage_new() mallocs 
			 * memory for the page and its URL; this malloc is freed at the 
			 * end of the while loop in the main() function, using webpage_delete()
			 */
			qput(webpages_qp, webpage_new(url_result, new_depth, NULL));
		 }
		else { /* Free the URL (external or seen) that doesn't make it to table */
			free(url_result);
		}
	}
}
					
int main(int argc, char *argv[]) {
	char usage[50] = "usage: crawler <seedurl> <pagedir> <maxdepth>";

	/* Check correct number of arguments */
	if (argc != 4) {
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
		printf("maxdepth should be > 0\n");
		exit(EXIT_FAILURE);
	}

	char *seed_url = argv[1];
	char *pagedir = argv[2];
	int max_depth = atoi(argv[3]);

	hashtable_t *seen_urls_hp = hopen(10); /* Hashtable of seen URLs */
	queue_t *webpages_qp = qopen();        /* Queue of all webpages at */
										   /* depth <= max_depth       */

	/* Make webpage object from specified seed_url at depth 0; put webpage in
	 * queue; put seed_url into hashtable of seen URLs
	 */
	webpage_t *first_page = webpage_new(seed_url, 0, NULL);
	qput(webpages_qp, first_page);
	hput(seen_urls_hp, seed_url, seed_url, strlen(seed_url));

	int id = 1;
	webpage_t *pagep = (webpage_t *) qget(webpages_qp);

	/* Loop over queue of webpages, starting from first_page, until the queue
	 * is empty. At the end of each iteration of the loop, invoke webpage_delete()
	 * on the webpage so as to free the space malloced for the webpage by 
	 * webpage_new() inside the expand() function.

	 * When maxdepth = 0 the crawler retrieves and saves the page at seed_url. 
	 * When maxdepth = 1 the crawler retrieves and saves the page at seed_url, 
	 * then crawls it, then retrieves and saves the pages linked in it
	 */
	while (pagep != NULL) {
		if (!webpage_fetch(pagep)) {          /* Check that pagep URL is valid */
			printf("UNABLE TO FETCH PAGE\n"); /* and able to be fetched        */
		}
		else {
			if (pagesave(pagep, id, pagedir) != 0) {   /* Save pagep to pagedir */
				exit(EXIT_FAILURE);
			}
			else {
				printf("Saving page #%d\n", id);
			}
			int current_depth = webpage_getDepth(pagep);
			//printf("%d\n", current_depth);
			if (current_depth < max_depth) {
				expand(seen_urls_hp, webpages_qp, pagep, current_depth+1);
			}
			//qapply(webpages_qp, print_webpage);
			//printf("------\n");
			id += 1;
		}
		webpage_delete(pagep);
		pagep = qget(webpages_qp);  /* Get next webpage from queue */
	}
	
	//printf("REACHED THE END OF WHILE LOOP\n");

	printf("All the %d hashed URLs: \n", id-1);
	happly(seen_urls_hp, print_urlstring);

	/* Remove seed_url from hashtable because it was manually added and was not 
	 * malloced whereas the rest of the URLs were put in by the expand function 
	 * and were malloced for by webpage_getNextURL(). We remove the seed_url because 
	 * we will call free() on the hashtable later to free the malloced URLs
	 */
	hremove(seen_urls_hp, search_for_url, seed_url, strlen(seed_url));

	happly(seen_urls_hp, free);
	qclose(webpages_qp);
	hclose(seen_urls_hp);
	
	exit(EXIT_SUCCESS);
}
