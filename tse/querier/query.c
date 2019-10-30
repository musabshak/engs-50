/* query.c --- 
 * 
 * 
 * Author: Musab Shakeel
 * Created: Sat Oct 26 00:23:49 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <hash.h>
#include <queue.h>
#include <indexio.h>

typedef struct word {
	char *normalized_word;
	queue_t *webpages_qp;
} word_t;

typedef struct webpagenode {
	int word_count;
	int id;
} webpagenode_t;

char* str_tolower(char *str) {
    int len = strlen(str);

    for (int i=0; i<len; i++) {
        /* Check that character is alphabet */
        if (!isalpha(str[i])) {
            return NULL;
        }
        str[i] = (char) tolower(str[i]);
    }

    return str;
}

int get_query(char *normalized_input) {
    char input[1000];
    printf("> ");
    fgets(input, 1000, stdin);

    /* Replace '\n' at end of input with '\0' */
    if (strlen(input) != 0) {
        input[strlen(input)-1] = '\0';
    }

    int invalid = 0;
    char *word = strtok(input, " ");
    while (word != NULL) {
        char *normalized_word = str_tolower(word);
        /* If word has non-alphabets, break loop */
        if (normalized_word == NULL) {
            invalid = 1;
            break;
        }
        /* Ignore words with len<3, and "and", "or" */
        else if (strlen(normalized_word) < 3 ||
            strcmp(normalized_word, "and") == 0 ||
            strcmp(normalized_word, "or") == 0) {
                word = strtok(0, " "); /* Ignore "and" and "or" */
            }
        /* Everything else is a valid query word */
        else {
            strcat(normalized_input, normalized_word);
            strcat(normalized_input, " ");
            word = strtok(0, " ");
        }
    }

    return invalid;
}

int str_to_array(char **input_array, char *str) {
    char *word = strtok(str, " ");
    int num_words = 0;
    while (word != NULL) {
        input_array[num_words] = word;
        word = strtok(0, " ");
        num_words += 1;
    }
    return num_words;
}

bool search_word(void *elementp, const void *searchkeyp) {
	word_t *word_obj = (word_t *) elementp;
	if (strcmp(word_obj->normalized_word,(char *) searchkeyp)==0) {
		return true;
	}
	else {
		return false;
	}
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

int main(int argc, char *argv[]) {

    hashtable_t *word_htable = indexload("../indexer/indexnm");

    while (!feof(stdin)) {
        char normalized_input[1000] = "\0";
        int invalid = get_query(normalized_input);        

        if (feof(stdin)) { /* If input is EOF */
            printf("\n"); 
        }
        else if (invalid) { /* If input is invalid */
            printf("INVALID\n");
        }
        else if (normalized_input[0] == 0) { /* If no input, do nothing */
            ; 
        }
        else { /* Else print input */
            //printf("%s\n", normalized_input);
            char *input_array[100];
            int num_words = str_to_array(input_array, 
                                           normalized_input);

            char query_result[200];
            int pos = 0;
            int min_pagecount = 99999;
            for (int i=0; i<num_words; i++) {
                char *query_word = input_array[i];
                word_t *resulting_word = (word_t *) hsearch(word_htable, 
                                                            search_word, 
                                                            query_word,
                                                            strlen(query_word));
                         
                int page_count;
                if (resulting_word != NULL) {
                    int page_id = 1;
                    queue_t *webpages_qp = resulting_word->webpages_qp;
                    webpagenode_t *pagenode = (webpagenode_t *) qsearch(webpages_qp, 
                                                                        search_webpagenode,
                                                                        &page_id);
                    page_count = pagenode->word_count;
                }
                else {
                    page_count = 0;
                }

                if (page_count < min_pagecount) {
                    min_pagecount = page_count;
                }
                pos += sprintf(&query_result[pos], "%s:%d ", query_word, page_count);
            }  
            pos += sprintf(&query_result[pos], "-- %d", min_pagecount);

            printf("%s\n", query_result);   

        }
    }

    return 0;
}