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
#include <webpage.h>
#include <stdbool.h>
#include <string.h>


void print_webpage(void *page) {
	printf("URL: %s\n", webpage_getURL((webpage_t *) page));
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

int main() {
	char seed_url[100] = "https://thayer.github.io/engs50/";

	// Create new webpage
	webpage_t *first_page = webpage_new(seed_url, 0, NULL);

	// Fetch html content associated with webpage url and store in webpage attribute: html
	if (!webpage_fetch(first_page)) {
		exit(EXIT_FAILURE);
	}

	/*
	Scan fetched html content from seed page; create and store in a queue a webpage 
  for each internal URL found
	*/
	queue_t *webpages_qp = qopen();
	int pos = 0;
	char *url_result;

	// Hashtable of seen URLs
	hashtable_t *seen_urls_hp = hopen(7);
	
	// webpage_getNextURL() mallocs url_result that user needs to free
	while ( (pos = webpage_getNextURL(first_page, pos, &url_result) ) > 0) {
		printf("Found %s URL: %s\n",
				    IsInternalURL(url_result) ? "Internal" : "External",
				    url_result);

		// If url not seen, put in hashtable and queue
		bool seen = false;
		if (hsearch(seen_urls_hp, search_for_url, url_result, sizeof(url_result)) != NULL) {
			seen = true;
		}
		
		if (IsInternalURL(url_result) && !seen ) {
			hput(seen_urls_hp, url_result, url_result, sizeof(url_result));
			qput(webpages_qp, webpage_new(url_result, 1, NULL));
		 }
		else {
			char *external_link = url_result;
			free(external_link);
		}
	}
	
	// Print hashtable of seen urls
	//happly(seen_urls_hp, print_urlstring);

	// Free the URLs
	happly(seen_urls_hp, free_the_urls);
	
	// Print queue of webpages
	qapply(webpages_qp, print_webpage);

	// Free seed webpage
	webpage_delete(first_page);
	
	// Free all the new webpages created and close webpages_qp
	qapply(webpages_qp, webpage_delete);
	qclose(webpages_qp);

	// Close seen_urls_hp
	hclose(seen_urls_hp);

	exit(EXIT_SUCCESS);
}
