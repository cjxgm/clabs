/** link.c
 * cjfc: link
 **
 * Under GPLv2, ABSOLUTELY NO WARRANTY!
 **
 * by eXerigumo Clanjor
 */

#include <stdlib.h>
#include "link.h"

link_t * link_new(size_t size)
{
	link_t * lnk = malloc(sizeof(link_t) + size);
	return lnk->next = lnk->prev = lnk;
}

void link_delete(link_t * lnk)
{
	lnk->prev->next = lnk->next;
	lnk->next->prev = lnk->prev;
	free(lnk);
}

void link_clear(link_t * lnk)
{
	link_t * p;

	while ((p = lnk->next) != lnk) {
		lnk->next = p->next;
		free(p);
	}

	lnk->prev = lnk;
}

void link_free(link_t * lnk)
{
	link_clear(lnk);
	free(lnk);
}

void link_append(link_t * lnk, link_t * src)
{
	src->prev = lnk;
	src->next = lnk->next;
	lnk->next = src;
	src->next->prev = src;
}

void link_swap(link_t * a, link_t * b)
{
	a->prev->next = b;
	a->next->prev = b;
	b->prev->next = a;
	b->next->prev = a;

	link_t t = *a;
	*a = *b;
	*b = t;
}

