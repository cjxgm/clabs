
#include <lua.h>
#include <lauxlib.h>


/************************************************************
 *
 * functino declarations
 *
 */

static int l_init(lua_State * L);




/************************************************************
 *
 * global variables
 *
 */

static const struct luaL_Reg lib[] = {
	{"init", &l_init},
	{NULL, NULL}
};




/************************************************************
 *
 * main entry
 *
 */

int luaopen_libwm(lua_State * L)
{
	luaL_register(L, "libwm", lib);
	return 1;
}




/************************************************************
 *
 * library functions
 *
 */

static int l_init(lua_State * L)
{
	lua_pushstring(L, "Hello, World!");
	return 1;
}

