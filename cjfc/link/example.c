/** example.c
 * cjfc: link (example)
 **
 * Under GPLv2, ABSOLUTELY NO WARRANTY!
 **
 * by eXerigumo Clanjor
 */

#include <stdio.h>
#include <stdlib.h>
#include "link.h"

int main(void)
{
	link_t * lnk;
	link_t * p;

	lnk = LINK_NEW_ROOT();

	// input data
	printf("Please input data to be sorted:\n");
	while (1) {
		p = LINK_NEW(double);
		if (scanf("%lg", LINK_DATA(p, double)) == EOF) {
			free(p);
			break;
		}
		LINK_PREPEND(lnk, p);
	}

	// selection sort (asc)
	p = lnk;
	while ((p = p->next) != lnk) {
		link_t * t = p;
		while ((t = t->next) != lnk)
			if (*LINK_DATA(t, double) < *LINK_DATA(p, double)) {
				LINK_SWAP(p, t);
				t = p;
			}
	}

	// output result
	p = lnk;
	while ((p = p->next) != lnk)
		printf("%lg\n", *LINK_DATA(p, double));

	return 0;
}

