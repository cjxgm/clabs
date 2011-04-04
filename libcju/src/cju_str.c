/** cju_str.c
 * Clanjor Utilities Library: String
 **
 * This code is under GPLv3, NO WARRANTY!!!
 * Copyleft (C) eXerigumo Clanjor, 2010-2011.
 **
 * Coded by eXerigumo Clanjor.
 */

#include <stdlib.h>
#include "cju_str.h"

cju_str_t *cju_str_new(void)
{
	return calloc(sizeof(cju_str_t), 1);
	/*
	cju_str_t *str;

	if (str = malloc(sizeof(cju_str_t)) == NULL)
		return NULL;
	
	str->data = NULL;
	str->used = -1;
	return str;
	*/
}

cju_str_t *cju_str_new_with_cstr(const char *cstr)
{
	cju_str_t *str;

	if ((str = cju_str_new()) == NULL)
		return NULL;

	cju_str_cat_cstr(str, cstr);
	return str;
}

void cju_str_free(cju_str_t *str)
{
	free(str->data);
	free(str);
}

bool cju_str_append(cju_str_t *str, char ch)
{
	// Allocate memory
	if (str->used == 0
		|| (str->used + 1) % CJU_STR_BLOCK_SIZE == 0) {
		//          ^^^^^ +1 is for the 'end of string' (0 of course).
		str->data = realloc(str->data,
					str->used + 1 +	CJU_STR_BLOCK_SIZE);
		if (str->data == NULL)
			return false;
	}

	// Append <ch> followed by 0, you know why.
	str->data[str->used++] = ch;
	str->data[str->used] = 0;
	return true;
}

// TODO: Improve performance.
bool cju_str_cat_cstr(cju_str_t *str, const char *src)
{
	while (*src)
		if (!cju_str_append(str, *src++))
			return false;
	return true;
}

// NEVER free the cstring returned!!!
char *cju_str_get_cstr(cju_str_t *str)
{
	return str->data;
}

