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

char *NormalizeWords(char *str) {
	int len = strlen(str);
	/* If lower than 3 character long, then discard*/
	if (len<=3) {
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

int main(int argc, char *argv[]) {
	/* Load a webpage */
	webpage_t *loaded_page = pageload(1,"../pages/");
	// char word[] = "D";
	// printf("%s\n", word);
	// if (NormalizeWords(word)!=NULL){
	// 	printf("%s\n", word);
	// }
	char **htmlword;
	int cur_pos = 0;
	while (webpage_getNextWord(loaded_page,cur_pos,htmlword)<webpage_getHTMLlen(page)){
		printf("%s\n", *htmlword);
	}


	return 0;
}
