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

/*
int main() {
	char seed_url[300] = "https://thayer.github.io/engs50/";

	webpage_t *page = webpage_new(seed_url,0,NULL);
	if (! webpage_fetch(page)) {
		exit(EXIT_FAILURE);
	}

	int pos = 0;
	char *res;

	queue_t *webpages = qopen();
	
	while ( (pos=webpage_getNextURL(page,pos,&res)) > 0) {
		//char urltype[30];
		//urltype = IsInternalURL(res) ? "Internal" : "External";
		printf("Found url of type  %s  : ' %s'\n", IsInternalURL(res) ? "Internal" : "External", res);


		

		

		
		free(res);
	}
	//free(webpage_getHTML(page));
	//free(webpage_getURL(page));
	//free(page);
	webpage_delete(page);
	exit(EXIT_SUCCESS);
}
*/


void print_webpage(void *page) {
	webpage_t *tmp = (webpage_t *) page;
	printf("URL: %s\n", webpage_getURL(tmp));
}


int main() {
	char seed_url[300] = "https://thayer.github.io/engs50/";

	webpage_t *page = webpage_new(seed_url,0,NULL);
	if (! webpage_fetch(page)) {
		exit(EXIT_FAILURE);
	}

	int pos = 0;
	char *res;

	queue_t *webpages = qopen();
	
	while ( (pos=webpage_getNextURL(page,pos,&res)) > 0) {
		//char urltype[30];
		//urltype = IsInternalURL(res) ? "Internal" : "External";
		printf("Found url of type  %s  : ' %s'\n", IsInternalURL(res) ? "Internal" : "External", res);

		if (IsInternalURL(res)) {
			webpage_t *tmp_page = webpage_new(res, 1, NULL);
			qput(webpages, tmp_page);
		 }
		
		free(res);
	}

	qapply(webpages, print_webpage);
	qapply(webpages, webpage_delete);
	qclose(webpages);
	//free(webpage_getHTML(page));
	//free(webpage_getURL(page));
	//free(page);
	webpage_delete(page);
	exit(EXIT_SUCCESS);
}
