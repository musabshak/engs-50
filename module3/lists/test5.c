/* test5.c ---
 * 
 * 
 * Author: Musab Shakeel
 * Created: Sat Sep 28 19:07:47 2019 (-0400)
 * Version: 
 * 
 * Description: apply() a function to an empty list
 * 
 */

#include <list.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <car_utilities.h>

int main(void) {

	lapply(printcar);

	exit(EXIT_SUCCESS);
}
