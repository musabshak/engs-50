#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <webpage.h>


/* Saves a given webpage to a local directory in normalized format
 * Arguments
 * 		pagep: pointer to webpage that needs to be saved
 *		id: integer id that will be the filename
 *		dirnm: string representing the destination directory
 * Returns
 * 		0 if page file saved without errors
 * 		1 if error occurred during saving page file
 * Format
 * 		"""
 * 		https://thayer.github.io/engs50/
 * 		0
 * 		2774
 * 		Example of the html text. 
 * 		This is the last line
 * 		"""
 */
int32_t pagesave(webpage_t *pagep, int id, char *dirnm);

/* 
 * pageload -- loads the numbered filename <id> in direcory <dirnm>
 * into a new webpage
 *
 * returns: non-NULL for success; NULL otherwise
 */
webpage_t *pageload(int id, char *dirnm);
