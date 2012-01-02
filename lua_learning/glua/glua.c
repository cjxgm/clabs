
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <GL/glut.h>




static void dofile(lua_State * L, const char * filename);
static void render(lua_State * L);
static void reshape(int w, int h);

static int glua_window_create(lua_State * L);
static int glua_render_cube(lua_State * L);




static float spf = 30;
static const luaL_Reg glua[] = {
	{"window_create", &glua_window_create},
	{"render_cube", &glua_render_cube},
	{NULL, NULL}
};




int main(int argc, char * argv[])
{
	lua_State * L = luaL_newstate();
	luaL_openlibs(L);
	luaL_register(L, "glua", glua);

	chdir("luapi");
	dofile(L, "glua.lua");
	chdir("..");

	if (argc != 2) 
		error(L, "Please specify a file.");
	dofile(L, argv[1]);
	glutMainLoop();

	lua_close(L);
	return 0;
}


static void dofile(lua_State * L, const char * filename)
{
	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot load file %s", lua_tostring(L, -1));
}

static void render(lua_State * L)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lua_getglobal(L, "Window");
	lua_getfield(L, -1, "render");
	if (!lua_isfunction(L, -1))
		error(L, "want render function");

	if (lua_pcall(L, 0, 0, 0))
		error(L, "an error occured: %s", lua_tostring(L, -1));

	glutSwapBuffers();

	glutTimerFunc(spf, (void *)&render, (int)L);
}

static void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 100, w/h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}




static int glua_window_create(lua_State * L)
{
	// args: title, width, height, spf
	//         1      2       3     4

	int argc = 0;
	glutInit(&argc, NULL);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(lua_tointeger(L, 2), lua_tointeger(L, 3));
	glutCreateWindow(lua_tostring(L, 1));

	spf = lua_tonumber(L, 4);
	glutTimerFunc(spf, (void *)&render, (int)L);
	glutReshapeFunc(&reshape);

	return 0;
}

static int glua_render_cube(lua_State * L)
{
	// args: x, y, z, w, h, d
	//       1  2  3  4  5  6
	glPushMatrix();
	glTranslatef(lua_tonumber(L, 1),
				 lua_tonumber(L, 2),
				 lua_tonumber(L, 3));
	glScalef(lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
	glutSolidCube(1);
	glPopMatrix();
	return 0;
}

