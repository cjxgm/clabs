#include <string.h>
#include "renderer.h"
#include "renderer_stdout.h"

renderer_t renderers[] = {
	{ "stdout", renderer_stdout_plain,
				renderer_stdout_key,
				renderer_stdout_escape},
};
int renderers_len = 0;

static int rid = 0; // Renderer ID

void renderer_init(void)
{
	renderers_len = sizeof(renderers) / sizeof(renderers[0]);
}

int select_renderer(const char *renderer)
{
	for (rid=0; rid<renderers_len; rid++)
		if (strcmp(renderer, renderers[rid].name) == 0)
			return 0;
	return 1;
}

void render_plain(const char *str)
{
	renderers[rid].render_plain(str);
}

void render_key(const char *str)
{
	renderers[rid].render_key(str);
}

void render_escape(const char *str)
{
	renderers[rid].render_escape(str);
}

