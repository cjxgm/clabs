
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static void call_lua_func(lua_State * L,
		const char * func, const char * fmt, ...)
{
	va_list va;
	int narg, nres;

	va_start(va, fmt);
	lua_getglobal(L, func);

	for (narg=0; *fmt; narg++, fmt++) {
		luaL_checkstack(L, 1, "too many arguments");

		switch (*fmt) {
			case 'd':	// double
				lua_pushnumber(L, va_arg(va, double));
				break;
			case 'i':	// int
				lua_pushinteger(L, va_arg(va, int));
				break;
			case 's':	// const char *
				lua_pushstring(L, va_arg(va, const char *));
				break;
			case '>':	// arguments done
				fmt++;
				goto args_done;
			default:
				error(L, "invalid option (%c)", *fmt);
		}
	}
	args_done:

	nres = strlen(fmt);
	if (lua_pcall(L, narg, nres, 0))
		error(L, "error calling %s: %s", func, lua_tostring(L, -1));

	nres = -nres;	// index from the first result
	for (; *fmt; fmt++, nres++) {
		switch (*fmt) {
			case 'd':	// double
				if (!lua_isnumber(L, nres))
					error(L, "wrong result type");
				*va_arg(va, double *) = lua_tonumber(L, nres);
				break;
			case 'i':	// int
				if (!lua_isnumber(L, nres))
					error(L, "wrong result type");
				*va_arg(va, int *) = lua_tointeger(L, nres);
				break;
			case 's':	// const char *
				if (!lua_isstring(L, nres))
					error(L, "wrong result type");
				*va_arg(va, const char **) = lua_tostring(L, nres);
				break;
			default:
				error(L, "invalid option (%c)", *fmt);
		}
	}

	va_end(va);
}

static void load_script(lua_State * L, const char * filename)
{
	double t;

	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot load file %s", lua_tostring(L, -1));

	call_lua_func(L, "MAIN", "s>d", "cjxgm", &t);

/*
	lua_getglobal(L, "MAIN");
	lua_pushstring(L, "cjxgm");

	//               .----- 1 argument
	//               |  .-- 1 result
	//               v  v
	if (lua_pcall(L, 1, 1, 0))
		error(L, "error launching MAIN function: %s", lua_tostring(L, -1));

	if (!lua_isnumber(L, -1))
		error(L, "What a fuck! I want a number!!");
	t = lua_tonumber(L, -1);
	lua_pop(L, 1);
*/

	printf("I've got \e[1m%g\e[m.\n", t);
}

static int l_getinfo(lua_State * L)
{
	switch (lua_tointeger(L, 1)) {
		case 1:
			lua_pushstring(L, "cjxgm");
			return 1;
		case 2:
			lua_pushinteger(L, 1995);
			return 1;
		case 3:
			lua_pushstring(L, "cjxgm");
			lua_pushinteger(L, 1995);
			return 2;
		default:
			error(L, "unknown info id %d", lua_tointeger(L, 1));
			return 0;
	}
}

static const luaL_Reg cjfc[] = {
	{"getinfo", &l_getinfo},
	{NULL, NULL}
};

int main(void)
{
	lua_State * L = luaL_newstate();
	luaL_openlibs(L);
	luaL_register(L, "Cjfc", cjfc);

	load_script(L, "demo.lua");

	lua_close(L);

	return 0;
}

