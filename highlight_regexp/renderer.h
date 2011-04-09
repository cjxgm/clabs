
#ifndef __RENDERER__
#define __RENDERER__

typedef struct {
	char *name;
	void (*render_plain)(const char *);
	void (*render_key)(const char *);
	void (*render_escape)(const char *);
} renderer_t;

extern renderer_t renderers[];
extern int renderers_len;

void renderer_init(void);
int select_renderer(const char *renderer); // Return 1 when failed.
void render_plain(const char *str);
void render_key(const char *str);
void render_escape(const char *str);

#endif
