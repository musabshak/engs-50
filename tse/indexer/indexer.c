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

typedef struct word {
	char *normalized_word;
	int count;
} word_t;


char *NormalizeWord(char *str) {
	int len = strlen(str);
	/* If lower than 3 character long, then discard*/
	if (len<3) {
		return NULL;
	}
	/* Loop through characters */
	for (int i = 0; i < len; i++){
		/* Check if the character is a not letter */
		if (isalpha(str[i])==0) {
			return NULL;
		}
		/* Convert to lowercase*/
		str[i] = (char) tolower(str[i]);
	}
	return str;
}

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

void printElement(void* elementp) {
	word_t *temp = (word_t *) elementp;
	printf("%s : %d\n", temp->normalized_word , temp->count );
}

static int total_count=0;
void counter(void* elementp) {
	word_t *temp = (word_t *) elementp;
	total_count+=temp->count;
}

void delete_word(void* elementp) {
	word_t *tmp = (word_t *) elementp;
	free(tmp->normalized_word);
	free(tmp);
}

int main(int argc, char *argv[]) {
	/* Load a webpage */
	webpage_t *loaded_page = pageload(1,"../pages/");

	// char word[] = "D";
	// printf("%s\n", word);
	// if (NormalizeWords(word)!=NULL){
	// 	printf("%s\n", word);
	// }

	char *htmlword;
	int cur_pos = 0;
	/*while (webpage_getNextWord(loaded_page,cur_pos,*htmlword)<webpage_getHTMLlen(loaded_page)){
		printf("%s\n", htmlword);
	}*/

	hashtable_t *word_htable = hopen(30);

	/* Loop through all words and add them to hashmap or increment thri count*/
	while ( (cur_pos = webpage_getNextWord(loaded_page, cur_pos, &htmlword)) > 0) {
		char *norm_htmlword = NormalizeWord(htmlword);
		/* If valid word */
		if (norm_htmlword!=NULL) {
			/* Search if already present */
			word_t *corresp_word = (word_t *) hsearch(word_htable,search_word,norm_htmlword,sizeof(norm_htmlword));
			/* If exists, increment*/
			if (corresp_word!=NULL){
				corresp_word->count+=1;
				free(norm_htmlword);
			}
			/* If doesn't, add new word to hash with count 1*/
			else {
				word_t *new_word = (word_t *) malloc(sizeof(word_t *));
				new_word->normalized_word = norm_htmlword;
				new_word->count=1;
				hput(word_htable,new_word,norm_htmlword,sizeof(norm_htmlword));
			}

		}
		else {
			free(htmlword);
		}
	}

	//happly(word_htable,counter);
	//happly(word_htable,printElement);

	printf("TOTAL_COUNT: %d\n", total_count);


	happly(word_htable, delete_word);
	hclose(word_htable);
	webpage_delete(loaded_page);


	return 0;
}
