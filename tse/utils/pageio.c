/* pageio.c --- 
 * 
 * 
 * Author: Musab Shakeel
 * Created: Wed Oct 23 16:01:16 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <webpage.h>
#include <string.h>

int32_t pagesave(webpage_t *pagep, int id, char *dirnm) {
	FILE *fp;
	char full_path[100];
	sprintf(full_path, "%s/%d", dirnm, id);

	fp = fopen(full_path, "w");
	if (fp==NULL) {
		printf("Error: file unable to be created!\n");
		return 1;
	}

	fprintf(fp,
			"%s\n%d\n%d\n%s",
			webpage_getURL(pagep),
			webpage_getDepth(pagep),
			webpage_getHTMLlen(pagep),
			webpage_getHTML(pagep)
			);

	fclose(fp);
	return 0;
}

webpage_t *pageload(int id, char *dirnm) {
	/* Formatting the path to open the file	 */
	FILE *fp;
	char full_path[100];
	sprintf(full_path, "%s/%d", dirnm, id);

	/* Opening the File */
	fp = fopen(full_path, "r");
	if (fp==NULL) {
		printf("Error: file unable to be read!\n");
		return NULL;
	}

	/* Scanning the file and storing read values */
	char url[300];
	int depth;
	int html_len;
	fscanf(fp,"%s\n%d\n%d\n", url, &depth, &html_len);
	char *html = (char *) malloc(200000000);

	int c;
	int pos = 0;
	while(1) {
		c = fgetc(fp);
		if (feof(fp) ) {
			break;
		}
		pos += sprintf(&html[pos], "%c", c);

	}
	html[pos] = '\0';

	/* Creating new webpage out of scanned variables */
	webpage_t *new_page = webpage_new(url, depth, html);
	//printf("Finished PAGEIO\n");

	fclose(fp);
	return new_page;
}

