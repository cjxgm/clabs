/** link.c
 * Link data structure for coders
 **
 * Under GPLv3
 * ABSOLUTELY NO WARRANTY!
 **
 * by eXerigumo Clanjor
 */

#include "link.h"
#include <assert.h>

/**********************************************************************
 * link_new
 **
 * Create a link's root node (Allocate memory and initialize it).
 * Remember that this function is only used for the ROOT node!
 */
LINK_HEAD * link_new(void)
{
	LINK_HEAD * lnk = NEW(LINK_HEAD);
	assert(lnk);

	lnk->next = lnk;
	lnk->prev = lnk;

	return lnk;
}

/**********************************************************************
 * link_free
 **
 * Free all the nodes that linked together.
 * This function is not only used for the root node.
 */
void link_free(LINK_HEAD * lnk)
{
	assert(lnk);

	LINK_HEAD * p = lnk;
	while (p->next != lnk) {
		p = p->next;
		free(p->prev);
	}
}

/**********************************************************************
 * link_append (macro, for typecasting)
 * link_append_do
 **
 * Append node <src> to <lnk>. (That is, <src> is inserted between
 * <lnk> and <lnk->next>).
 * This function is not only used for the root node.
 */
void link_append_do(LINK_HEAD * lnk, LINK_HEAD * src)
{
	assert(lnk);
	assert(src);

	src->prev = lnk;
	src->next = lnk->next;
	lnk->next = src;
	src->next->prev = src;
}

