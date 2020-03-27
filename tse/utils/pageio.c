/* pageio.c -- Defines functions to save/load webpages 
 *			   to/from a local directory
 * 
 * Author: Musab Shakeel
 * Created: Wed Oct 23 16:01:16 2019 (-0400)
 * 
 */

#include <webpage.h>
#include <string.h>

/* Saves a given webpage to a local directory in normalized format
 * Arguments
 * 		pagep: pointer to webpage that needs to be saved
 *		id: integer id that will be the filename
 *		dirnm: string representing the destination directory
 * Returns
 * 		0 if page file saved without errors
 * 		1 if error occurred during saving page file
 * Format
 * 		"""
 * 		https://thayer.github.io/engs50/
 * 		0
 * 		2774
 * 		Example of the html text. 
 * 		This is the last line
 * 		"""
 */
int32_t pagesave(webpage_t *pagep, int id, char *dirnm) {
	FILE *fp;
	/* Formatting the path name */
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

/* Loads a saved webpage from local directory
 * Arguments
 * 		id: integer that is the filename of saved page
 *		dirnm: local directory where page is saved
 */
webpage_t *pageload(int id, char *dirnm) {
	FILE *fp;
	/* Formatting the path name */
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

	/* Read html content character by character using fgetc() */
	int c; /* "character" */
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

	fclose(fp);
	return new_page;
}

