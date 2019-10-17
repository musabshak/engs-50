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

bool searchURL(void* elementp, const void* searchkeyp){
	char *t_url;
	strcpy(t_url,(char*) elementp);
	char *c_url;
	strcpy(c_url,(char*) searchkeyp);
	printf("COMPARING %s to %s \n",t_url,c_url);
	if (strcmp(t_url,c_url)==0) {
		return true;
	}
	return false;
}

int main() {
	char seed_url[300] = "https://thayer.github.io/engs50/";

	webpage_t *page = webpage_new(seed_url,0,NULL);
	if (! webpage_fetch(page)) {
		exit(EXIT_FAILURE);
	}

	int pos = 0;
	char *res;

	queue_t *webpages = qopen(); // to help the breadth first exploration of the website
	hashtable_t *hash = hopen(10); //for visited webpages
	
	while ( (pos=webpage_getNextURL(page,pos,&res)) > 0) {
		//char urltype[30];
		//urltype = IsInternalURL(res) ? "Internal" : "External";
		printf("Found url of type  %s  : ' %s'\n", IsInternalURL(res) ? "Internal" : "External", res);
		// Check if in Internal
		if (IsInternalURL(res)) {
			// Check if it has been visited (in the hash)
			if (hsearch(hash,searchURL,res,sizeof(res))==NULL){ 
					webpage_t *tmp_page = webpage_new(res, 1, NULL);
					// Add to visited pages
					hput(hash,res,res,sizeof(res));
					// Add to the breadth first queue
					qput(webpages, tmp_page);
				}
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
