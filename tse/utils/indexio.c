/* indexio.c --- 
 * 
 * 
 * Author: Musab Shakeel
 * Created: Fri Oct 25 18:17:54 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdint.h>
#include <queue.h>
#include <hash.h>
#include <stdio.h>
#include <pageio.h>
#include <webpage.h>

static FILE *fp;

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

hashtable_t* indexload(char *file) {
	char *file_name = file;
	FILE *fp1;
	fp1 = fopen(file_name, "r");

	if (fp1==NULL) {
		printf("Error: file unable to be read!\n");
		return NULL;
	}

	hashtable_t *word_htable = hopen(30);

	/* Loops over indexnm file: each iteration is one line of the indexnm file */
	while (1) {
		char line[1000];
		fgets(line, 1000, fp1);

		if (feof(fp1)) { /* Break if reached end of file */
			break;
		}

		char *read_word;
		read_word = strtok(line, " ");
		word_t *new_word = (word_t *) malloc(sizeof(word_t));
		new_word->normalized_word = (char *) malloc(strlen(read_word)+1);
		strcpy(new_word->normalized_word, read_word);
		new_word->webpages_qp = qopen();
		hput(word_htable, new_word, read_word, strlen(read_word));

		/* Loops over contents of each line starting after the first token: each iteration is a space-delimited token */
		while(1) {
			char *page_id = strtok(0, " ");
			//printf("hi\n");
			// if (atoi(page_id) == 0) { /* End of Line */
			// 	break; 
			// }
			if (page_id==NULL) { /* End of Line */
				break; 
			}
			char *word_count = strtok(0, " ");

			int page_id_int = atoi(page_id);
			int word_count_int = atoi(word_count);

			webpagenode_t *pagenode = (webpagenode_t *) malloc(sizeof(webpagenode_t));
		
			pagenode->id = page_id_int;
			pagenode->word_count = word_count_int;
			qput(new_word->webpages_qp, pagenode);
			
		}

	}
	fclose(fp1);
	
	return word_htable;

}






















