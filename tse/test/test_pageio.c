/* test_pageio.c --- 
 * 
 * 
 * Author: Musab Shakeel
 * Created: Wed Oct 23 18:28:16 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <webpage.h>
#include <pageio.h>

int main() {
	webpage_t *loaded_page = pageload(42, "../pages");
	pagesave(loaded_page, 42, "../pages1");

	webpage_t *loaded_page1 = pageload(42, "../pages1");

	printf("%s; %d; %d\n", webpage_getURL(loaded_page),
			webpage_getDepth(loaded_page), webpage_getHTMLlen(loaded_page));

	printf("%s; %d; %d\n", webpage_getURL(loaded_page1),
				webpage_getDepth(loaded_page1), webpage_getHTMLlen(loaded_page1));

	return 0;
}
