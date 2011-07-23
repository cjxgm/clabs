/** link.h
 * Link data structure for coders (header)
 **
 * Under GPLv3
 * ABSOLUTELY NO WARRANTY!
 **
 * by eXerigumo Clanjor
 */

#ifndef __LINK_H__
#define __LINK_H__

#include <stdlib.h>

#ifndef NEW
#define NEW(TYPE)		malloc(sizeof(TYPE))
#endif

typedef struct _link_head
{
	struct _link_head * prev;
	struct _link_head * next;
}
LINK_HEAD;

LINK_HEAD * link_new(void);
void link_free(LINK_HEAD * lnk);
#define link_append(lnk, src)	link_append_do(lnk, (LINK_HEAD *)(src));
void link_append_do(LINK_HEAD * lnk, LINK_HEAD * src);

#endif

