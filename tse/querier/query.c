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

/**
STRUCTS
**/

typedef struct word {
	char *normalized_word;
	queue_t *webpages_qp;
} word_t;

typedef struct webpagenode {
	int word_count;
	int id;
} webpagenode_t;

typedef struct document {
    int id; 
    char *url; 
    int rank;
} document_t;


/**
STRUCT FUNCTIONS
**/

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

/**
HELPER FUNCTIONS
**/

/** 
Convert string to lowercase. Return NULL if any non-alphabet
character in string.
**/
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

/** 
Convert a string into an array of words

Args
    char *str: String to be converted
    char **input_array: Empty array of words into which the separated words will
                        be stored
Returns
    num_words: Number of words stored in input_array
**/
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

/**
OTHER FUNCTIONS
**/

/**
Get a query from user and normalize it.
Removes from query: 
- words with length < 3
- "and", "or"

Args
    char *normalized_input: Empty string into which the normalized input
    will be stored
Returns 
    1 if query is invalid (i.e., has non alphabet characters)
    0 if query is valid
**/
int get_query(char *normalized_input) {
    char input[1000] = "\0";
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





/* Rank one webpage for a given query */
/**
Args:
    char **input_array: Array of normalized words inputted by the user
    int num_words: Number of words in input_array
    int page_id: Particular page id
    char *query_result: Empty string into which the response to the query 
                  will be stored
**/
int rank_page(hashtable_t * word_htable, char **input_array, int num_words, 
              int page_id, char *query_result) {
    //int pos = 0;
    int min_wordcount = 99999; /* Rank: minimum of all word counts for a given page */ 
    for (int i=0; i<num_words; i++) {
        char *query_word = input_array[i];
        /* Does word exist in index? */
        word_t *resulting_word = (word_t *) hsearch(word_htable, 
                                                    search_word, 
                                                    query_word,
                                                    strlen(query_word)); 
        int word_count = 0; /* Number of occurrences of word in page */
        if (resulting_word != NULL) {
            /* If word exists in index, does it exist in given webpage? */     
            queue_t *webpages_qp = resulting_word->webpages_qp;
            webpagenode_t *pagenode = (webpagenode_t *) qsearch(webpages_qp, 
                                                                search_webpagenode,
                                                                &page_id);                                       
            if (pagenode != NULL) { /* If it does, word_count is set accordingly */
                    word_count = pagenode->word_count;
            }
        }
        /* Is the word_count for this particular word lower than overall word count? */
        if (word_count < min_wordcount) {
            min_wordcount = word_count;
        }
        
        //pos += sprintf(&query_result[pos], "rank:%d:doc:%d:url%s", word_count, page_id, page_url);
    }  
    //pos += sprintf(&query_result[pos], "-- %d; PAGE:%d", min_wordcount, page_id);

    return min_wordcount;
}

void print_document(void *elementp) {
    document_t *doc = (document_t *) elementp; 
    printf("rank:%d:doc:%d:url:%s\n", doc->rank, doc->id, doc->url);
}

void delete_doc(void *elementp) {
    document_t *doc = (document_t *) elementp; 
    free(doc->url);
    free(doc);
}

/**
Given a page_id and rank, make a malloc-ed document struct.
Load the page from the page directory based off the page_id, get the url for 
the page (first line), malloc space for the url, and store the url in the document struct.
**/
document_t* make_document(int page_id, int rank) {
    document_t *doc = (document_t *) malloc(sizeof(document_t));
    doc->id = page_id;
    doc->rank = rank;

    /* Load page file to get the url */
    FILE *fp;
	char full_path[100];
	sprintf(full_path, "%s/%d", "../pages", page_id);

	/* Opening the File */
	fp = fopen(full_path, "r");
	if (fp==NULL) {
		printf("Error: file unable to be read!\n");
		return NULL;
	}

	/* Scanning the file and storing read values */
	char url[100];
	fscanf(fp,"%s\n", url);    
    doc->url = malloc(strlen(url)+1);
    strcpy(doc->url, url);

    fclose(fp);
    return doc;
}

int main(int argc, char *argv[]) {

    hashtable_t *word_htable = indexload("../indexer/indexnm");

    /* While user has not entered EOF */
    while (!feof(stdin)) {
        char normalized_input[1000] = "\0";
        //normalized_input[0] = 0;
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
        else { /* Valid Input */
            printf("%s\n", normalized_input);
            /* Convert normalized_input into array of words */
            char *input_array[100]; 
            int num_words = str_to_array(input_array, 
                                           normalized_input);

            queue_t *queue_docs = qopen();
            /* For a given query, find rank of each webpage */
            for (int page_id=1; page_id<=20; page_id ++) {
                char query_result[200]; /* What will be printed in response to a query */
                int rank = rank_page(word_htable, input_array, num_words, 
                            page_id, query_result);
                if (rank != 0) { /* If document contains all words in query */
                    document_t *doc = make_document(page_id, rank);
                    qput(queue_docs, doc);
                    ;
                }
                //printf("%s\n", query_result);   
            };
            qapply(queue_docs, print_document);
            qapply(queue_docs, delete_doc);
            qclose(queue_docs);
        }
    }

    happly(word_htable, delete_word);
    hclose(word_htable);

    return 0;
}