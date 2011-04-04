/** type.h
 * Clanjor Utilities Library: Type
 **
 * This code is under GPLv3, NO WARRANTY!!!
 * Copyleft (C) eXerigumo Clanjor, 2010-2011.
 **
 * Coded by eXerigumo Clanjor.
 */

#ifndef __CJU_TYPE__
#define __CJU_TYPE__

/************ COMMON ************/
// Use lower-case on purpose, because VIM can highlight them.
#define true	1
#define false	0

typedef unsigned char byte;
// Without postfix "_t" on purpose, because VIM can highlight them.
typedef char bool;

/************ cju_str ************/
#define CJU_STR_BLOCK_SIZE		32

typedef struct {
	char *data;		// The string.
	int used;		// How many chars?
} cju_str_t;

#endif

