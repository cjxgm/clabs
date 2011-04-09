#include <stdio.h>
#include <stdlib.h>
#include "processor.h"
#include "renderer.h"

void process(const char *renderer)
{
	if (select_renderer(renderer)){
		fprintf(stderr, "No such a renderer called %s.", renderer);
		exit(1);
	}

	int ch;
	char str[4] = {0};
	while ((ch = getchar()) != EOF) {
		switch (ch) {
			case '.':
			case '*':
			case '[':
			case ']':
				str[0] = ch;
				str[1] = 0;
				render_key(str);
				break;
			case '\\':
				if ((ch = getchar()) == EOF) {
					fprintf(stderr, "Unexpected EOF.\n");
					exit(1);
				}
				str[0] = '\\';
				str[1] = ch;
				str[2] = 0;
				switch (ch) {
					case '<':
					case '>':
					case '(':
					case ')':
					case '{':
					case '}':
					case '?':
					case '|':
						render_key(str);
						break;
					default:
						render_escape(str);
				}
				break;
			default:
				str[0] = ch;
				str[1] = 0;
				render_plain(str);
				break;
		}
	}
}
