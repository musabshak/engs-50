/* indexio.c -- Contains the indexsave() and indexload() functions that 
 * save and load an index to a file
 * 
 * Author: Musab Shakeel
 * 
 * Description:
 * 
 *  
 */

#include <stdint.h>
#include <queue.h>
#include <hash.h>
#include <stdio.h>
#include <pageio.h>
#include <webpage.h>

/* GLOBAL VARIABLE */
static FILE *fp; 	/* File pointer for destination file
				  	 * where index is to be saved 
				  	 */

typedef struct word {
	char *normalized_word;
	queue_t *webpages_qp;
} word_t;

typedef struct webpagenode {
	int word_count;
	int id;
} webpagenode_t;

static void print_webpagenode(void *elementp) {
	webpagenode_t *pagenode = (webpagenode_t *) elementp;
	fprintf(fp, "%d %d ", pagenode->id, pagenode->word_count);
}

static void print_word(void *elementp) {
	word_t *word = (word_t *) elementp;
	fprintf(fp, "%s ", word->normalized_word);
	qapply(word->webpages_qp, print_webpagenode);
	fprintf(fp, "\n");
}

/* Takes an index in the form of a hash table of words and saves 
 * it to a file locally in the following format:
 * 
 * The index file shall contain one line for each word in the index. 
 * Each line has the format: 
 * 			<word> <docID1> <count1> <docID2> <count2> ….<docIDN> <countN> 
 * where <word> is a string of lowercase letters, <docIDi> is a positive 
 * integer designating a document, <counti> is a positive integer designating
 * the number of occurrences of <word> in <docIDi>; each entry should be 
 * placed on the line separated by a space
 */
int32_t indexsave(hashtable_t *word_htable, char *file) {
	char *file_name = file;
	fp = fopen(file_name, "w");
	if (fp==NULL) {
		printf("Error: file unable to be created!\n");
		return 1;
	}
	happly(word_htable, print_word);
	fclose(fp);
	return 0;
}

/* Loads a saved index (text file) into a hash table of words. Returns
 * a pointer to the newly malloced hashtable (this will need to be freed
 * by the user later
 */
hashtable_t* indexload(char *file) {
	char *file_name = file;
	FILE *fp1;
	fp1 = fopen(file_name, "r");

	if (fp1==NULL) {
		printf("Error: file unable to be read!\n");
		return NULL;
	}

	hashtable_t *word_htable = hopen(30);

	/* Loops over indexnm file; each iteration is one line of the
	 * indexnm file (and thus one word)
	 */
	while (1) {
		char line[1000];
		fgets(line, 1000, fp1);

		if (feof(fp1)) { /* Break if reached end of file */
			break;
		}

		char *read_word;
		read_word = strtok(line, " "); /* First " " delimited token of the */
									   /* line is the word				   */

		/* Malloc word struct and word text; add pointer to word text to word
		 * struct attribute 
		 */
		word_t *new_word = (word_t *) malloc(sizeof(word_t));
		new_word->normalized_word = (char *) malloc(strlen(read_word)+1);
		strcpy(new_word->normalized_word, read_word);

		/* Open webpages queue for word struct; put word struct in index */
		new_word->webpages_qp = qopen();
		hput(word_htable, new_word, read_word, strlen(read_word));

		/* Loops over contents of each line starting after the first token;
		 * each iteration is a space-delimited token which alternates b/w the
		 * document ID (first_tok) and the associated word count (second_tok)
		 */
		while(1) {
			char *first_tok = strtok(0, " ");
			if (atoi(first_tok) == 0) { /* End of Line */
				break; 
			}
			// if (first_tok==NULL) { /* End of Line */
			// 	break; 
			// }
			char *second_tok = strtok(0, " ");
			
			int page_id_int = atoi(first_tok);
			int word_count_int = atoi(second_tok);

			webpagenode_t *pagenode = (webpagenode_t *) malloc(
														sizeof(webpagenode_t) 
														);
			pagenode->id = page_id_int;
			pagenode->word_count = word_count_int;
			qput(new_word->webpages_qp, pagenode);
		}

	}
	fclose(fp1);
	return word_htable;
}






















