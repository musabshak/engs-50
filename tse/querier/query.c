/* query.c
 * 
 * 
 * Author: Musab Shakeel
 * Created: Sat Oct 26 00:23:49 2019 (-0400)
 * 
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <hash.h>
#include <queue.h>
#include <indexio.h>
#include <stdbool.h>

				/*
				 *
				 * STRUCTS & STRUCT FUNCTIONS
				 *
				 */

/*
 * word
 */

/*
 * word
 * This is the struct that is stored in the hashtable "index". Each word struct
 * has a normalized text string (normalized_word) and a queue of webpages
 * (webpages_qp) associated with it.
 */
typedef struct word {
	char *normalized_word;
	queue_t *webpages_qp;
} word_t;

bool search_word(void *elementp, const void *searchkeyp) {
	word_t *word_obj = (word_t *) elementp;
	if (strcmp(word_obj->normalized_word,(char *) searchkeyp)==0) {
		return true;
	}
	else {
		return false;
	}
}

/*
 * Function header for function defined later
 */
void delete_webpagenode(void *elementp);

void delete_word(void *elementp) {
	word_t *word = (word_t *) elementp;
	free(word->normalized_word);
	qapply(word->webpages_qp, delete_webpagenode);
	qclose(word->webpages_qp);
	free(word);
}

/*
 * Function header for function defined later
 */
void print_webpagenode(void *elementp);

void print_word(void *elementp) {
	word_t *word = (word_t *) elementp;
	printf("Word: %s\n", word->normalized_word);
	qapply(word->webpages_qp, print_webpagenode);
	printf("\n\n");
}


/*
 * webpagenode
 */

/*
 * webpagenode
 * Each word in the index has a queue of webpages (webpages_qp) associated
 * with it; webpagenode is the struct stored inside that queue
 */
typedef struct webpagenode {
	int word_count;
	int id;
} webpagenode_t;

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

void print_webpagenode(void *elementp) {
	webpagenode_t *pagenode = (webpagenode_t *) elementp;
	printf("(id: %d; word_count: %d) ", pagenode->id, pagenode->word_count);
}

void delete_webpagenode(void *elementp) {
	webpagenode_t *pagenode = (webpagenode_t *) elementp;
	free(pagenode);
}

void print_webpage_qp(void *elementp) {
    queue_t *webpages_qp = (queue_t *) elementp;
    qapply(webpages_qp, print_webpagenode);
    printf("\n");
}


/*
 * document
 */

/*
 * document
 * This struct stores the id, url, and rank of a given webpage.
 */
typedef struct document {
    int id;
    char *url;
    int rank;
} document_t;

/*
 * Given a page_id and rank, make a malloc-ed document struct.
 * Load the page from the page directory based off the page_id, get the url for
 * the page (first line), malloc space for the url, and store the url in the document struct.
 */
document_t* make_document(int page_id) {
    document_t *doc = (document_t *) malloc(sizeof(document_t));
    doc->id = page_id;
    doc->rank = 0;

    /** Load page file to get the url **/
    FILE *fp;
	char full_path[100];
	sprintf(full_path, "%s/%d", "../pages", page_id);

	/* Opening the file */
	fp = fopen(full_path, "r");
	if (fp==NULL) {
		printf("Error: file unable to be read!\n");
		return NULL;
	}

	/* Scanning the file and storing read values */
	char url[1000];
	fscanf(fp,"%s\n", url);    
    doc->url = malloc(strlen(url)+1);
    strcpy(doc->url, url);

    fclose(fp);
    return doc;
}

void print_document(void *elementp) {
    document_t *doc = (document_t *) elementp;
    printf("rank:%d:doc:%d:url:%s\n", doc->rank, doc->id, doc->url);
}

bool search_document(void *elementp, const void *keyp) {
    document_t *doc = (document_t *) elementp; 
    int *key = (int *) keyp;
    if (doc->id == *key) {
        return true;
    }
    else {
        return false;
    }

}

void delete_doc(void *elementp) {
    document_t *doc = (document_t *) elementp;
    free(doc->url);
    free(doc);
}



				/*
				 *
				 * HELPER FUNCTIONS
				 *
				 */

/*
 * Convert string to lower case. Return NULL if any non-alphabet
 * character present in string.
 */
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

/*
 * Convert a string into an array of words
 *
 * Args
 * 		char *str: String to be converted
 * 		char **input_array: Empty array of words into which the separated words
 * 		will be stored
 *
 * Returns
 * 		num_words: Number of words stored in input_array
 */
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

/*
 * Print an array of words
 */
void print_word_array(char **word_array, int arr_len) {
    for (int i=0; i<arr_len; i++) {
        printf("%s ", word_array[i]);
    }
    printf("\n");
}

/*
 * Print an array of integers. Assumes that the integer -50 is used
 * as the sentinel character to mark the end of the array
 */
void print_int_array(int *int_array) {
    int i = 0;
    while (int_array[i] != -50) {
        printf("%d ", int_array[i]);
        i += 1;
    }
    printf("\n");
}

int is_and(char *word) {
    int ans = (strcmp(word, "and") == 0) ? 1 : 0;
    return ans;
}

int is_or(char *word) {
    int ans = (strcmp(word, "or") == 0) ? 1 : 0;
    return ans;
}

void strarr_to_str(char **input_array, int arr_size, char *dest_str) {
    for (int i=0; i<arr_size; i++) {
        strcat(dest_str, input_array[i]);
        strcat(dest_str, " ");
    }
    dest_str[strlen(dest_str)-1] = '\0';
}

				/*
				 *
				 * OTHER_FUNCTIONS
				 *
				 */

/*
 * Get a query from user.
 * Only normalization done in this function is:
 * - queries with any non-alphabet characters are marked as invalid
 * - words with length<3 that are not "or" are removed from query
 *
 * Args
 * 		char *input_str: Empty string into which the normalized input
 * 		will be stored
 * Returns
 * 		1 if query is invalid (i.e., has non alphabet characters)
 *  	0 if query is valid
 */
int get_query(char **input_array) {
    char input_str[1000] = "\0";
    printf("> ");
    fgets(input_str, 1000, stdin);

    /* Replace '\n' at end of input with '\0' */
    if (strlen(input_str) != 0) {
        input_str[strlen(input_str)-1] = '\0';
    }
    char *word = strtok(input_str, " ");
    int num_tokens = 0;
    while (word != NULL) {
        input_array[num_tokens] = (char *) malloc(strlen(word)+1);
        strcpy(input_array[num_tokens], word);
        word = strtok(0, " ");
        num_tokens += 1;
    }
    return num_tokens;
}

int normalize_query(char **input_array, int num_tokens, 
                    char **normalized_input_array, int *num_valid_tokens) {

    if (num_tokens == 0) {
        return 0;
    }
    int valid = 1;

    char *first_token = input_array[0];
    char *last_token = input_array[num_tokens-1];
    //printf("first: %s; last: %s\n", first_token, last_token);

    if (is_and(first_token) || is_or(first_token)  ||
        is_and(last_token)  || is_or(last_token)) {
            valid = 0;
            return valid;
        }
    // printf("so far so good\n");
    int lastseen_and_or = 0;
    for (int i=0; i<num_tokens; i++) {
        char *token = str_tolower(input_array[i]);
        /* If word has non-alphabets, break loop */
        if (token == NULL) {
            //printf("what\n");
            valid = 0;
            return valid;
        }
        /* Ignore words with len<3 that are not "or" */
        if (strlen(token) < 3 && 
            strcmp(token, "or") != 0) {
            continue;
        }

        if ((is_and(token) || is_or(token)) && lastseen_and_or) {
            valid = 0;
            return valid;
        }
        else if ((is_and(token) || is_or(token)) && !lastseen_and_or) {
            lastseen_and_or = 1;
        }
        else if (! (is_and(token) || is_or(token))) { 
            lastseen_and_or = 0;
        }
        normalized_input_array[*num_valid_tokens] = token;
        *num_valid_tokens += 1;

    }
    return valid;
 }

int find_min_wc(hashtable_t *index, char *and_str, int page_id) {
    int min_wordcount = 99999;
    char *word = strtok(and_str, " ");
    while (word != NULL) {
        if (is_and(word)) {
            word = strtok(0, " ");
            continue;
        }
        //printf("word: %s\n", word);
        int word_count = 0;
        word_t *resulting_word = (word_t *) hsearch(index, 
                                                    search_word, 
                                                    word,
                                                    strlen(word));
        //printf("word found in index\n");
        queue_t *webpages_qp = resulting_word->webpages_qp;
        webpagenode_t *pagenode = (webpagenode_t *) qsearch(webpages_qp, 
                                                                search_webpagenode, 
                                                                &page_id);
        //qapply(webpages_qp, print_webpagenode);
                // printf("pagenode found for page: %d\n", page_id);
        word_count += pagenode->word_count;
        if (word_count < min_wordcount) {
            min_wordcount = word_count;
        }
        word = strtok(0, " ");
    }
    return min_wordcount;
}

void intersect_webpages_qp(queue_t *all_webpages_qp, queue_t *intersection) {
    
    queue_t *first_qp = (queue_t *) qget(all_webpages_qp);

    if (first_qp == NULL) {
        return ;
    }
    webpagenode_t *first_qp_pagenode = (webpagenode_t *) qget(first_qp);
    while (first_qp_pagenode != NULL) {
        int first_qp_pagenode_id = first_qp_pagenode->id;
        bool pagenode_in_intersection = true;
        queue_t *ith_qp = (queue_t *) qget(all_webpages_qp); 
        queue_t * removed_queues_qp = qopen();
        while(ith_qp != NULL) {
            if (qsearch(ith_qp, search_webpagenode, 
                        &first_qp_pagenode_id) == NULL) {
                pagenode_in_intersection = false;
                qput(removed_queues_qp, ith_qp);
                break;
            }
            qput(removed_queues_qp, ith_qp);
            ith_qp = qget(all_webpages_qp);
        }
        if (pagenode_in_intersection) {
            qput(intersection, first_qp_pagenode);
        }
        qconcat(all_webpages_qp, removed_queues_qp);
        first_qp_pagenode = qget(first_qp);
    }
    qput(all_webpages_qp, first_qp);

}

void cpy_webpages_qp(queue_t *dest_qp, queue_t *src_qp) {
    webpagenode_t *pagenode = qget(src_qp);
    queue_t * tmp = qopen(); // MALLOC (freed by concat)
    while (pagenode != NULL) {
        //webpagenode_t *nodecpy = (webpagenode_t *) malloc(sizeof(webpagenode_t));
        //memcpy(nodecpy, pagenode, sizeof(webpagenode_t));
        qput(dest_qp, pagenode);
        qput(tmp, pagenode);
        pagenode = qget(src_qp);
    }
    qconcat(src_qp, tmp);
}

void free_webpage_qp(void *elementp) {
    queue_t * webpage_qp = (queue_t *) elementp;
    qclose(webpage_qp);
}

void process_and(hashtable_t * index, queue_t *docs_qp, char *and_str) {
    // loop through and_str, skipping "and" words
    // get intersection of pages for each word
    // loop through intersection (one iteration: one page_node, and_str)

    char and_strcpy1[strlen(and_str)+1];
    strcpy(and_strcpy1, and_str);
    queue_t *all_webpage_qs_qp = qopen(); // MALLOC
    int all_in_index = true;

    char *word = strtok(and_strcpy1, " ");
    while (word != NULL) {
        printf("word: %s\n", word);
        if (is_and(word)) {
            word = strtok(0, " ");
            continue;
        }
        word_t *resulting_word = (word_t *) hsearch(index, 
                                                    search_word, 
                                                    word,
                                                    strlen(word));
        if (resulting_word != NULL) {
            printf("ONCE\n");
            printf("word found in index\n");
            queue_t *webpages_qp = qopen(); // MALLOC
            cpy_webpages_qp(webpages_qp, resulting_word->webpages_qp);
            //printf("testing: \n");
            //qapply(webpages_qp, print_webpagenode);
            qput(all_webpage_qs_qp, webpages_qp);
        }  
        else {
            all_in_index = false;
        }
        word = strtok(0, " ");
    }
    qapply(all_webpage_qs_qp, print_webpage_qp);
    queue_t *intersection = qopen(); // MALLOC
    if (all_in_index) {
        intersect_webpages_qp(all_webpage_qs_qp, intersection);
    }


    qapply(all_webpage_qs_qp, free_webpage_qp);
    qclose(all_webpage_qs_qp);


    printf("intersection: ");
    qapply(intersection, print_webpagenode);
    printf("\n");
    webpagenode_t *intersected_page = (webpagenode_t *) qget(intersection);
    while(intersected_page != NULL) {
        char and_strcpy2[strlen(and_str)+1];
        strcpy(and_strcpy2, and_str);

        int intersected_page_id = intersected_page->id;
        document_t *resulting_doc = (document_t *) qsearch(docs_qp, search_document, 
                                                    &intersected_page_id);
        if (resulting_doc != NULL) {
            resulting_doc->rank += find_min_wc(index, and_strcpy2, intersected_page_id);
        }
        else {
            document_t *new_doc = make_document(intersected_page_id); // MALLOC (freed in main)
            new_doc->rank = find_min_wc(index, and_strcpy2, intersected_page_id);
            qput(docs_qp, new_doc);
        }
        intersected_page = qget(intersection);
    }   
    qclose(intersection);

}

void process_query(hashtable_t * index, queue_t *docs_qp, char *input_str) {

    while(1) {
        char *tmp_str = strstr(input_str, " or ");
        if (tmp_str) {
        *tmp_str = 0;
        }
        printf("str_split: %s\n", input_str);
        process_and(index, docs_qp, input_str);
        if (!tmp_str) {
             break;
        }
        input_str = tmp_str + 4;
    }

    printf("end of query\n");
}

int main(int argc, char *argv[]) {

    hashtable_t *index = indexload("../indexer/indexnm_test");
    
    printf("loaded\n");
    //happly(index, print_word);
    
    /* While user has not entered EOF */
    while (!feof(stdin)) {
        char *input_array[100]; 
        char *normalized_input_array[100];
        int num_tokens = get_query(input_array);

        int num_valid_tokens = 0;

        // for (int i=0; i<num_tokens; i++) {
        //     printf("word: %s\n", input_array[i]);
        // }

        int valid = normalize_query(input_array, 
                                      num_tokens, 
                                      normalized_input_array, 
                                      &num_valid_tokens);  

        if (feof(stdin)) { /* If input is EOF */
            printf("\n"); 
        } 
        else if (num_tokens == 0) { /* If no input, do nothing */
            continue;
        }
        else if (!valid) { /* If input is invalid */
            printf("INVALID\n");
        }
        else if (num_valid_tokens != 0) { /* Valid Input */
            //print_word_array(normalized_input_array, num_valid_tokens);     
            char normalized_inputstr[1000] = "";
            strarr_to_str(normalized_input_array, num_valid_tokens,
                          normalized_inputstr);
            queue_t *docs_qp = qopen();

            // char normalized_inputstrcpy[100];
            // strcpy(normalized_inputstrcpy,normalized_inputstr);

            printf("norm_array: ");
            print_word_array(normalized_input_array, num_valid_tokens);
            printf("norm_str: %s\n", normalized_inputstr);
            
            process_query(index, docs_qp, normalized_inputstr);

            qapply(docs_qp, print_document);
            qapply(docs_qp, delete_doc);
            qclose(docs_qp);
        }

        for (int i=0; i<num_tokens; i++) {
            free(input_array[i]);
        }
    }


    happly(index, delete_word);
    hclose(index);

    return 0;
}
