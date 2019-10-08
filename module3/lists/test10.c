/* test10.c ---
 * 
 * 
 * Author: Musab Shakeel
 * Created: Sat Sep 28 19:07:47 2019 (-0400)
 * Version: 
 * 
 * Description: remove() from a non-empty list in middle 
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
	car_t *car3p = make_car("ENGS62", 12000, 2100);
	lput(car1p);
	lput(car2p);
	lput(car3p);
  car_t *result1 = lremove("ENGS50");
	
	lapply(printcar);

	if (result1 == NULL) {
		exit(EXIT_FAILURE);
	}
	free(car1p);
	free(car2p);
	free(car3p);
	exit(EXIT_SUCCESS);
}
