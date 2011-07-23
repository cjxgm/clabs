/** main.c
 * Link data structure for coders (usage)
 **
 * Under GPLv3
 * ABSOLUTELY NO WARRANTY!
 **
 * by eXerigumo Clanjor
 */

// Get ready!
#include <stdio.h>
#include "link.h"			// You need this of course.

// The LINK_HEAD provided has no data field, so you need this.
typedef struct _int_link
{
	LINK_HEAD	head;		// This must be the first field.
	int			data;		// Put anything you want here.
}
INT_LINK;

// Let's begin!
int main(void)
{
	LINK_HEAD * root = link_new();	// Initialize an root node
	int i;

	for (i=0; i<4; i++) {
		// Use your own INT_LINK defined above.
		//.---v---.
		INT_LINK * il = NEW(INT_LINK);
		//              `-----v-----'
		//	NEW(T) == malloc(sizeof(T)) (see link.h)

		il->data = i;
		link_append(root, il);
		// `-------v--------'
		// Just do it like this.
		// You don't need to do a type-casting
		// because the macro helps you. (see link.h)
	}

	// Traversing is as easily as you see below.
	LINK_HEAD * p = root;
	while ((p = p->next) != root)
		printf("%d\n", ((INT_LINK *)p)->data);
		//				`-----v------'
		// FIXME: You have to do a type-casting here.

	// The end!
	return 0;
}

