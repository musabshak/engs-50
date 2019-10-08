/* test2.c ---
 * 
 * 
 * Author: Musab Shakeel
 * Created: Sat Sep 28 19:07:47 2019 (-0400)
 * Version: 
 * 
 * Description: put() to a non-empty list
 * 
 */

#include <list.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <car_utilities.h>

int main(void) {

	car_t *car1p = make_car("554HEY", 5000, 1998);
	car_t *car2p = make_car("ENGS50", 10000, 2000);
	
	int32_t result1 = lput(car1p);
	int32_t result2 = lput(car2p);

	lapply(printcar);
	
	if (result1 != 0 || result2 != 0) {
		exit(EXIT_FAILURE);
	}

	free(car1p);
	free(car2p);
	exit(EXIT_SUCCESS);
}
