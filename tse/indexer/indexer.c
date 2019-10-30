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


/*STRUCTS*/
/* Type of element in indexer that holds all the documents that present it*/
typedef struct word {
	char *normalized_word; // word searched for
	queue_t *in_documents; // list of webpages IDs that have the word and their count
} word_t;

/* Type of element in word objects representing a webpage ID the count of the word */
typedef struct document {
	int ID; // ID of the webpage
	int frequency; // number of occurence
} document_t;


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

/*SEARCH FUNCTIONS*/
bool search_word(void* elementp, const void* searchkeyp){
	word_t *word_obj = (word_t *) elementp;
	//printf("%s VS %s; ", word_obj->normalized_word, (char*) searchkeyp);
	if (strcmp(word_obj->normalized_word,(char *) searchkeyp)==0) {
		//printf("true\n");
		return true;
	}
	else {
		//printf("false\n");
		return false;
	}
}


/* Search for the ID in the documents queue */
bool search_ID(void* elementp, const void* searchkeyp) {
	document_t *doc = (document_t *) elementp;
	int *t_ID = (int*) searchkeyp;
	if (doc->ID == *t_ID) {
		return true;
	}
	else {
		return false;
	}
}

/*APPLY FUNCTIONS*/
/* Print the ids of the documents */
void printDocs(void* elementp) {
	document_t *temp = (document_t *) elementp;
	printf("%d : %d\n", temp->ID, temp->frequency);
}

/* Print the words */
void printWords(void* elementp) {
	word_t *temp = (word_t *) elementp;
	printf("___ %s ___\n", temp->normalized_word);
	qapply(temp->in_documents,printDocs);
}

/* Get the count of the words in the entire index */
static int total_count=0;
void counter_helper(void* elementp) {
	document_t *tmp = (document_t *) elementp;
	total_count += tmp->frequency;
}

void counter(void* elementp) {
	word_t *temp = (word_t *) elementp;
	qapply(temp->in_documents,counter_helper);
}

void delete_document(void* elementp) {
	document_t *tmp = (document_t *) elementp;
	free(tmp);
}

void delete_word(void* elementp) {
	word_t *tmp = (word_t *) elementp;
	free(tmp->normalized_word);
	qapply(tmp->in_documents,delete_document);
	qclose(tmp->in_documents);
	free(tmp);
}

/*MAIN CODE*/
int main(int argc, char *argv[]) {
	// Load a webpage
	int page_ID = atoi(argv[1]);
	printf("id = %d\n",page_ID);
	
	
	int current_page_ID = 1;
	char *htmlword;


	hashtable_t *indexer_htable = hopen(30);
	// current_page_ID = page_ID;  ////////////////////////////////
	while (current_page_ID<=page_ID) {
		webpage_t *loaded_page = pageload(current_page_ID,"../pages/");
		int cur_pos = 0;

		// Loop through all words and add them to indexer or increment through their document frequency variable
		while ( (cur_pos = webpage_getNextWord(loaded_page, cur_pos, &htmlword)) > 0) {
			char *norm_htmlword = NormalizeWord(htmlword);
			// If valid word, update indexer accordingly 
			if (norm_htmlword!=NULL) {
				// Search if word object already present 
				word_t *corresp_word = (word_t *) hsearch(indexer_htable,search_word,norm_htmlword,strlen(norm_htmlword));
				// If word exists, check if corresponding document exists 
				if (corresp_word!=NULL){
					// If page ID is in the documents list, increment the frequency of that word in that document
					document_t *corresp_doc = (document_t *) qsearch(corresp_word->in_documents,search_ID, (void *) &current_page_ID);
					if (corresp_doc!=NULL) {
						corresp_doc->frequency+=1;
					}

					// Else create a new document for that word entry
					else {
						// Add new document and set frequency to 1
						document_t *doc = (document_t *) malloc(sizeof(document_t));
						doc->ID = current_page_ID;
						doc->frequency = 1;
						qput(corresp_word->in_documents,doc);
					}
					// Free the htmlword if it has been mallocced but not added to indexer because already existed
					free(norm_htmlword);
				}

				/* If word doesn't exist, add new word to hash and add 
				* document with corresponding ID and count 1 */
				else {
					// Create new word obj
					word_t *new_word = (word_t *) malloc(sizeof(word_t));
					new_word->in_documents = qopen();
					// Update name
					new_word->normalized_word = norm_htmlword; 
					// Add new document and update
					document_t *doc = (document_t *) malloc(sizeof(document_t));
					doc->ID = current_page_ID;
					doc->frequency = 1;
					qput(new_word->in_documents,doc);
					// Add word obj to indexer 
					hput(indexer_htable,new_word,norm_htmlword,strlen(norm_htmlword));
				}

			}
			else {
				free(htmlword);
			}
		}
		webpage_delete(loaded_page);
		printf("|||||||||||| done with page %d\n",current_page_ID);
		current_page_ID++;
	}
	happly(indexer_htable,counter);
	// happly(indexer_htable,printWords);

	printf("TOTAL_COUNT: %d\n", total_count);


	happly(indexer_htable, delete_word);
	hclose(indexer_htable);
	


	return 0;
}
