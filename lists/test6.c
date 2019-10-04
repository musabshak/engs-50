/* test6.c ---
 * 
 * 
 * Author: Musab Shakeel
 * Created: Sat Sep 28 19:07:47 2019 (-0400)
 * Version: 
 * 
 * Description: apply() a function to a non-empty list
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
  int32_t result1 = lput(car1p);
	int32_t result2 = lput(car2p);
	int32_t result3 = lput(car3p);
	
	lapply(printcar);

	if (! (result1 == 0 && result2 ==0 && result3 == 0)) {
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
