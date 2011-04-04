/** cju_str.h
 * Clanjor Utilities Library: String (Header)
 **
 * This code is under GPLv3, NO WARRANTY!!!
 * Copyleft (C) eXerigumo Clanjor, 2010-2011.
 **
 * Coded by eXerigumo Clanjor.
 */

#ifndef __CJU_STR__
#define __CJU_STR__

#include "cju_type.h"

inline cju_str_t *cju_str_new(void);
cju_str_t *cju_str_new_with_cstr(const char *cstr);
inline void cju_str_free(cju_str_t *str);

bool cju_str_append(cju_str_t *str, char ch);
bool cju_str_cat_str(cju_str_t *str, const cju_str_t *src);
bool cju_str_cat_cstr(cju_str_t *str, const char *src);

inline char *cju_str_get_cstr(cju_str_t *str);

#endif

