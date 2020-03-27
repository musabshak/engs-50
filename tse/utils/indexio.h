#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <webpage.h>
#include <hash.h>
#include <queue.h>

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
int32_t indexsave(hashtable_t *word_htable, char *file);

/* Loads a saved index (text file) into a hash table of words. Returns
 * a pointer to the newly malloced hashtable (this will need to be freed
 * by the user later
 */
hashtable_t* indexload(char *file);
