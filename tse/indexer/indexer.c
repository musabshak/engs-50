/* indexer.c -- Indexer
 *
 * Author:  Musab Shakeel and Selim Hassari
 * Created: Wed Oct 23 15:11:12 2019 (-0400)
 *
 * Description: 
 * Creates an index from all the crawled webpages that are stored in 
 * a local directory. The index is a hashtable of words. Each word has
 * associated with it a queue of documents in which the word is found.
 * Each element of this queue, along with the document ID, also stores
 * the number of occurrences of the word in that document.
 * 
 * The index file shall contain one line for each word in the index. 
 * Each line has the format: 
 * 			<word> <docID1> <count1> <docID2> <count2> ….<docIDN> <countN> 
 * where <word> is a string of lowercase letters, <docIDi> is a positive 
 * integer designating a document, <counti> is a positive integer designating
 * the number of occurrences of <word> in <docIDi>; each entry should be 
 * placed on the line separated by a space
 * 
 * Usage: 
 * 		indexer <pagedir> <indexnm> 
 * 
 * where pagedir is directory where the saved pages exist and indexnm 
 * is the output file where index will be saved.
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

/* GLOBAL VARIABLE */
static int total_count = 0;

/* STRUCTS */

/* Each element of the hashtable is this word struct

 * normalized_word is the lower case version of the word
 * that appears in the document. Words with len < 3 and
 * any words with non-alphabet characters are not included 
 * in the index
 * 
 * webpages_qp is the queue of documents associated with 
 * each word. Each document is represented as another 
 * struct: webpagenode (below), which stores the document
 * id and the number of times the word appeared in the 
 * document 
 */
typedef struct word {
	char *normalized_word;
	queue_t *webpages_qp;
} word_t;

typedef struct webpagenode {
	int word_count;
	int id;
} webpagenode_t;

/* HELPER FUNCTIONS */

/* Normalize a given string by setting everything to lower case 
 * All strings with len<3 are discarded
 */
char *NormalizeWord(char *str) {
	int len = strlen(str);
	/* If lower than 3 character long, then discard */
	if (len<3) {
		return NULL;
	}
	/* Loop through characters */
	for (int i = 0; i < len; i++){
		/* Check if the character is a not letter */
		if (isalpha(str[i])==0) {
			return NULL;
		}
		/* Convert to lowercase */
		str[i] = (char) tolower(str[i]);
	}
	return str;
}

/* Searches for a word struct in a hashtable. Returns true
 * if found, false otherwise
 */
bool search_word(void *elementp, const void *searchkeyp){
	word_t *word_obj = (word_t *) elementp;
	if (strcmp(word_obj->normalized_word,(char *) searchkeyp)==0) {
		return true;
	}
	else {
		return false;
	}
}

/* Prints the document ID and the associated word count for the document */
void print_webpagenode(void *elementp) {
	webpagenode_t *pagenode = (webpagenode_t *) elementp;
	printf("(id: %d; word_count: %d) ", pagenode->id, pagenode->word_count);
}

/* Prints a word in the hashtable. In the next line, the queue of documents
 * containing that word and the associated word counts are also printed. 
 * 
 * An example of a call to this function:
 * """
 * 		> professor
 * 		> (id: 1; word_count: 4) (id: 4; word_count: 2) 		
 * """
 */
void print_word(void *elementp) {
	word_t *word = (word_t *) elementp;
	printf("Word: %s\n", word->normalized_word);
	qapply(word->webpages_qp, print_webpagenode);
	printf("\n\n");
}

/* Prints a string */
void print_string(void *elementp) {
	printf("%s\n", (char *) elementp);
}

/* For *one* word in the hashtable, finds the sum of the word counts
 * for all the documents the word appears in. i.e. Finds out the 
 * total number of times a particular word appears in all the saved
 * webpage documents
 */
void sumwords_queue(void *elementp) {
	webpagenode_t *pagenode = (webpagenode_t *) elementp;
	total_count += pagenode->word_count;
}

/* Finds the total number of all words in all the documents */
void sumwords(void *elementp) {
	word_t *word = (word_t *) elementp;
	qapply(word->webpages_qp, sumwords_queue);
}

/* Frees the malloced webpagenode struct */
void delete_webpagenode(void *elementp) {
	webpagenode_t *pagenode = (webpagenode_t *) elementp;
	free(pagenode);
}

/* Frees the malloced word struct (and all its attributes) */
void delete_word(void *elementp) {
	word_t *word = (word_t *) elementp;
	free(word->normalized_word);
	qapply(word->webpages_qp, delete_webpagenode);
	qclose(word->webpages_qp);
	free(word);
}

/* Searches for a particular webpagenode. Used with qsearch(). Returns
 * true if specified webpagenode found, false otherwise
 */
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

/* Function to check whether a directory exists or not.
 * It returns 1 if given path is an existing directory, 
 * otherwise returns 0.
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

/* Finds the highest page ID from all the files in a directory */
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

/* Returns a malloced webpagenode struct with a given page ID.
 * The word_count is set by default to 1.
 */
webpagenode_t* make_webpagenode(int page_id) {
	webpagenode_t *new_node = (webpagenode_t *) malloc(sizeof(webpagenode_t));
	new_node->word_count = 1;
	new_node->id = page_id;
	return new_node;
}

/* Returns a malloced word struct with a given normalized word and an 
 * empty initialzed webpagenode queue. The normalized word attribute is 
 * not malloced here.
 */
word_t* make_word(char *norm_htmlword) {
	word_t *norm_htmlword_obj = (word_t *) malloc(sizeof(word_t));
	norm_htmlword_obj->normalized_word = norm_htmlword;
	norm_htmlword_obj->webpages_qp = qopen();
	return norm_htmlword_obj;
}

/* MAIN CODE */
int main(int argc, char *argv[]) {
	/* Check correct number of arguments */
	if (argc != 3) {
		printf("usage: indexer <page_dir> <indexnm>\n");
		exit(EXIT_FAILURE);
	}

	/* Check that the directory exists */
	if (!isDirectoryExists(argv[1])) {
		printf("Given directory does not exist; exiting program now.\n");
		exit(EXIT_FAILURE);
	}

	int end_page_id = max_pageid(argv[1]); /* Used for looping through dir.*/
	hashtable_t * word_htable = hopen(30); /* This is the "index" */

	/* Loop through all the saved documents in directory */
	for (int page_id=1; page_id<=end_page_id; page_id++) {
		int pos = 0;
		char *htmlword;

		webpage_t *loaded_page = pageload(page_id, "../pages");		

		/* Loops over all words in one webpage */
		while ( (pos = webpage_getNextWord(loaded_page, pos, &htmlword) ) > 0) {
			char *norm_htmlword = NormalizeWord(htmlword);
			if (norm_htmlword!=NULL) {
				/* Check if word already exists in hash table */
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

	/* Testing that indexsave and indexload functions from indexio work */
	// indexsave(word_htable, "indexnm");
	// hashtable_t *word_htable_loaded = indexload("indexnm");
	// indexsave(word_htable_loaded, "indexnm1");
	// happly(word_htable_loaded, delete_word);
	// hclose(word_htable_loaded);

	/* Save the index to the specified file */
	indexsave(word_htable, argv[2]);

	//happly(word_htable_loaded, print_word);
	happly(word_htable, sumwords);
	printf("TOTAL COUNT: %d\n", total_count);
	happly(word_htable, delete_word);
	hclose(word_htable);
	
	return 0;
}











