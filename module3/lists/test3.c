/* test3.c ---
 * 
 * 
 * Author: Musab Shakeel
 * Created: Sat Sep 28 19:07:47 2019 (-0400)
 * Version: 
 * 
 * Description: get() from an empty list
 * 
 */

#include <list.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <car_utilities.h>

int main(void) {
	
	car_t * result1 = lget();

	lapply(printcar);
	
	if (result1 != NULL) {
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
