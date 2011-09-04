/** link.h
 * cjfc: link (header)
 **
 * Under GPLv2, ABSOLUTELY NO WARRANTY!
 **
 * by eXerigumo Clanjor
 */

#ifndef __CJFC_LINK__
#define __CJFC_LINK__

typedef struct link
{
	struct link * prev;
	struct link * next;
}
link_t;

#define LINK_NEW(TYPE)			link_new(sizeof(TYPE))
#define LINK_NEW_ROOT()			link_new(0)

#define LINK_DELETE(LNK)		link_delete(LNK)
#define LINK_CLEAR(LNK)			link_clear(LNK)
#define LINK_FREE(LNK)			link_free(LNK)

#define LINK_DATA(LNK, TYPE) \
			((TYPE *)(((void *)(LNK)) + sizeof(link_t)))

#define LINK_APPEND(LNK, SRC)	link_append(LNK, SRC)
#define LINK_PREPEND(LNK, SRC)	LINK_APPEND((LNK)->prev, SRC)
#define LINK_SWAP(A, B)			link_swap(A, B)

inline link_t * link_new(size_t size);

inline void link_delete(link_t * lnk);
inline void link_clear(link_t * lnk);
inline void link_free(link_t * lnk);

inline void link_append(link_t * lnk, link_t * src);
inline void link_swap(link_t * a, link_t * b);

#endif

