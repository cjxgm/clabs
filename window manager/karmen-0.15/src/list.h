#if !defined(LIST_H)
#define LIST_H

/*
 * Doubly-linked list macros
 */

/*
 * Copyright 2006 Johan Veenhuizen
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Doubly-linked list type
 *
 *	Both lists and nodes of lists are of type LIST.
 *
 * Doubly-linked list macros
 *
 *	LIST_DEFINE(listname)
 *		Define and initialize an empty list at load time.
 *
 *	LIST_INIT([list | node])
 *		Initialize a list or node at run time.
 *
 *	LIST_EMPTY(list)
 *		Test if list is empty.
 *
 *	LIST_HEAD(list)
 *		Return the first element of list, or NULL if empty.
 *
 *	LIST_TAIL(list)
 *		Return the last element of list, or NULL if empty.
 *
 *	LIST_INSERT(list, node)
 *		Insert node somewhere in list (order not important).
 *
 *	LIST_INSERT_HEAD(list, node)
 *		Insert node at the head of list.
 *
 *	LIST_INSERT_TAIL(list, node)
 *		Insert node at the tail of list.
 *
 *	LIST_INSERT_BEFORE(elem, node)
 *		Insert node before elem.
 *
 *	LIST_INSERT_AFTER(elem, node)
 *		Insert node after elem.
 *
 *	LIST_REMOVE(node)
 *		Remove node from its list.
 *
 *	LIST_MEMBER(node)
 *		Test if node is a member of a list.
 *
 *	LIST_ITEM(node, type, memb)
 *		Return the structure of the given type, containing
 *		node as member memb.
 *
 *	LIST_FOREACH(node, list)
 *		Iterate over the nodes of list (for loop).
 */

#include <stdio.h>

/* The exported LIST type. */
typedef struct listnode LIST;

/* The internal list type.  Don't use this in code. */
struct listnode {
	struct listnode *ln_prev, *ln_next;
};

/* Define and initialize an empty list at load time. */
#define LIST_DEFINE(name)	LIST name = { &name, &name }

/* Initialize a list or list node at run time. */
#define LIST_INIT(lp)	((lp)->ln_prev = (lp)->ln_next = (lp))

/* Return the first node of list, or NULL if empty. */
#define LIST_HEAD(list) ((list)->ln_next == (list) ? NULL : (list)->ln_next)

/* Return the last node of list, or NULL if empty. */
#define LIST_TAIL(list) ((list)->ln_prev == (list) ? NULL : (list)->ln_prev)

/* Insert node before elem. */
#define LIST_INSERT_BEFORE(elem, node) do { \
	(node)->ln_next = (elem); \
	(node)->ln_prev = (elem)->ln_prev; \
	(elem)->ln_prev->ln_next = (node); \
	(elem)->ln_prev = (node); \
} while (0)

/* Insert node after elem. */
#define LIST_INSERT_AFTER(elem, node) do { \
	(node)->ln_prev = (elem); \
	(node)->ln_next = (elem)->ln_next; \
	(elem)->ln_next->ln_prev = (node); \
	(elem)->ln_next = (node); \
} while (0)

/* Insert node at the head of list. */
#define LIST_INSERT_HEAD(list, node)	LIST_INSERT_AFTER((list), (node))

/* Insert node at the tail of list. */
#define LIST_INSERT_TAIL(list, node)	LIST_INSERT_BEFORE((list), (node))

/* Use this when order is not important. */
#define LIST_INSERT(list, node)		LIST_INSERT_TAIL((list), (node))

/* Delete node from its list. */
#define LIST_REMOVE(node) do { \
	(node)->ln_prev->ln_next = (node)->ln_next; \
	(node)->ln_next->ln_prev = (node)->ln_prev; \
	LIST_INIT(node); \
} while (0)

/* Test if list if empty. */
#define LIST_EMPTY(list)	((list)->ln_next == (list))

/* Test if node is a member of a list. */
#define LIST_MEMBER(node)	((node)->ln_next != (node))

#define LIST_ITEM(node, type, memb) \
	((type *)((char *)(node) - (unsigned)&(((type *)0)->memb)))

#define LIST_FOREACH(node, list) \
	for ((node) = (list)->ln_next; \
	     (node) != (list); \
	     (node) = (node)->ln_next)

#endif /* defined(LIST_H) */
