
#ifndef LINK_H
#define LINK_H

typedef struct Link
{
	struct Link * prev, * next;
}
Link;

void link_init(Link * lnk);
void link_addtail(Link * lnk, void * data);
void link_addhead(Link * lnk, void * data);
void link_remove(void * data);
void link_delete(void * data);

#define TRAVERSE(TYPE, V, LNK) \
	TYPE * V = (TYPE *)LNK; \
	while ((V = V->next) != (TYPE *)LNK)

#endif

