/* indexer.c ---
 *
 *
 * Author:  Selim Hassari
 * Created: Wed Oct 23 15:11:12 2019 (-0400)
 * Version: 1
 *
 * Description: Indexer
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pageio.h>
#include <webpage.h>
#include <hash.h>
#include <queue.h>
#include <stdbool.h>
#include <indexio.h>
#include <dirent.h>
#include <sys/stat.h>


/*STRUCTS*/
/* Type of element in indexer that holds all the documents that present it*/
typedef struct word {
	char *normalized_word;
	queue_t *webpages_qp;
} word_t;

typedef struct webpagenode {
	int word_count;
	int id;
} webpagenode_t;

/*HELPER FUNCTIONS*/
/* Normalize a given string by setting everything to lower case 
 * All strings lower than 3 are discarded
 *  */
char *NormalizeWord(char *str) {
	int len = strlen(str);
	// If lower than 3 character long, then discard
	if (len<3) {
		return NULL;
	}
	// Loop through characters
	for (int i = 0; i < len; i++){
		// Check if the character is a not letter 
		if (isalpha(str[i])==0) {
			return NULL;
		}
		// Convert to lowercase
		str[i] = (char) tolower(str[i]);
	}
	return str;
}

bool search_word(void *elementp, const void *searchkeyp){
	word_t *word_obj = (word_t *) elementp;
	if (strcmp(word_obj->normalized_word,(char *) searchkeyp)==0) {
		return true;
	}
	else {
		return false;
	}
}

void print_webpagenode(void *elementp) {
	webpagenode_t *pagenode = (webpagenode_t *) elementp;
	printf("(id: %d; word_count: %d) ", pagenode->id, pagenode->word_count);
}

void print_word(void *elementp) {
	word_t *word = (word_t *) elementp;
	printf("Word: %s\n", word->normalized_word);
	qapply(word->webpages_qp, print_webpagenode);
	printf("\n\n");
}

void print_string(void *elementp) {
	printf("%s\n", (char *) elementp);
}

static int total_count = 0;

void sumwords_queue(void *elementp) {
	webpagenode_t *pagenode = (webpagenode_t *) elementp;
	total_count += pagenode->word_count;
}

void sumwords(void *elementp) {
	word_t *word = (word_t *) elementp;
	qapply(word->webpages_qp, sumwords_queue);
}

void delete_webpagenode(void *elementp) {
	webpagenode_t *pagenode = (webpagenode_t *) elementp;
	free(pagenode);
}

void delete_word(void *elementp) {
	word_t *word = (word_t *) elementp;
	free(word->normalized_word);
	qapply(word->webpages_qp, delete_webpagenode);
	qclose(word->webpages_qp);
	free(word);
}

bool search_webpagenode(void *elementp, const void *keyp) {
	webpagenode_t *nodep = (webpagenode_t *) elementp;
	int *key = (int *) keyp;
	if (nodep->id == *key) {
		return true;
	}
	else {
		return false;
	}
}

bool isDirectoryExists(const char *path)
{
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return true;

    return false;
}

int max_pageid (char *dirname) {
	int max_id = 0;

	DIR *d;
	struct dirent *dir;
	d = opendir(dirname);

	if (d) {
		while((dir = readdir(d)) != NULL) {
			if (atoi(dir->d_name)>max_id) {
				max_id = (atoi(dir->d_name));
			}
		}
		closedir(d);
	}

	return max_id;
}

webpagenode_t* make_webpagenode(int page_id) {
	webpagenode_t *new_node = (webpagenode_t *) malloc(sizeof(webpagenode_t));
	new_node->word_count = 1;
	new_node->id = page_id;

	return new_node;
}

word_t* make_word(char *norm_htmlword) {
	word_t *norm_htmlword_obj = (word_t *) malloc(sizeof(word_t));
	norm_htmlword_obj->normalized_word = norm_htmlword;
	norm_htmlword_obj->webpages_qp = qopen();

	return norm_htmlword_obj;
}

/*MAIN CODE*/
int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("usage: indexer <page_dir> <indexnm>\n");
		exit(EXIT_FAILURE);
	}

	if (!isDirectoryExists(argv[1])) {
		printf("Given directory does not exist; exiting program now.\n");
		exit(EXIT_FAILURE);
	}

	int end_page_id = max_pageid(argv[1]);
	hashtable_t * word_htable = hopen(30);

	for (int page_id=1; page_id<=end_page_id; page_id++) {

		webpage_t *loaded_page = pageload(page_id, "../pages");

		int pos = 0;
		char *htmlword;

		/* Loops over all words in one webpage */
		while ((pos = webpage_getNextWord(loaded_page, pos, &htmlword)) > 0) {
			char *norm_htmlword = NormalizeWord(htmlword);
			if (norm_htmlword!=NULL) {
				word_t *existing_word = (word_t *) hsearch(word_htable,
														   search_word,
														   norm_htmlword,
														   strlen(norm_htmlword));
				if (existing_word!=NULL) { /* Word exists in hash table */
					webpagenode_t *existing_node = (webpagenode_t *) qsearch(existing_word->webpages_qp,
																			 search_webpagenode,
																			 &page_id);
					/* Check if node for loaded_page exists in webpages_qp for the current word */
					if (existing_node!=NULL) {
						existing_node->word_count += 1;
					}
					/* Create webpagenode for loaded_page if one doesn't exist */
					else {
						webpagenode_t *new_node = make_webpagenode(page_id);
						qput(existing_word->webpages_qp, new_node);
					}
					free(norm_htmlword); /* Free norm_htmlword if a malloc-ed version of it already exists */
				}
				/* Word does not exist in hash table; Create word, open queue, put node for loaded_page into queue */
				else {
					word_t *norm_htmlword_obj = make_word(norm_htmlword);
					webpagenode_t *new_node = make_webpagenode(page_id);
					qput(norm_htmlword_obj->webpages_qp, new_node);
					hput(word_htable, norm_htmlword_obj, norm_htmlword, strlen(norm_htmlword));
				}
			}
			else {
				free(htmlword); /* Free htmlword if it never makes it into the hashtable */
			}

		} /* End of while loop */
		webpage_delete(loaded_page);
	} /* End of for loop */

	// indexsave(word_htable, "indexnm");
	// hashtable_t *word_htable_loaded = indexload("indexnm");
	// indexsave(word_htable_loaded, "indexnm1");
	// happly(word_htable_loaded, delete_word);
	// hclose(word_htable_loaded);

	indexsave(word_htable, argv[2]);

	//happly(word_htable_loaded, print_word);
	happly(word_htable, sumwords);
	printf("TOTAL COUNT: %d\n", total_count);
	happly(word_htable, delete_word);
	hclose(word_htable);
	
	return 0;
}











