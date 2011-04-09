
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processor.h"
#include "renderer.h"

void show_help(void);
char *exename = "";

int main(int argc, char *argv[])
{
	char *renderer = "stdout";

	exename = *argv;
	argv++; argc--;

	renderer_init();

	while (argc){
		if (strcmp(*argv, "--help") == 0
				|| strcmp(*argv, "-h") == 0) {
			show_help();
			exit(0);
		}
		else if (strcmp(*argv, "--renderer") == 0
				|| strcmp(*argv, "-r") == 0) {
			argv++; argc--;
			if (argc == 0) {
				fprintf(stderr, "Why don't you specify a renderer?\n");
				exit(1);
			}
			if (strcmp(*argv, "list") == 0) {
				printf("Renderers:\n");
				int i;
				for (i=0; i<renderers_len; i++)
					printf("\t%s\n", renderers[i].name);
				exit(0);
			}
			renderer = *argv;
			argv++; argc--;
		}
		else {
			fprintf(stderr, "I don't know what '%s' means.\n", *argv);
			exit(1);
		}
	}

	process(renderer);

	return 0;
}

void show_help(void)
{
	printf("Highlight the Regular Expression.\n");
	printf("It will read regexp from stdin and "
			"hightlight it by specified renderer.\n");
	printf("\n");
	printf("USAGE:\n");
	printf("\t%s  [--help|-h] [--renderer|-r <renderer>|list]\n", exename);
	printf("\t--help, -h\n");
	printf("\t\tShow this help.\n");
	printf("\t--renderer, -r\n");
	printf("\t\tSpecify a renderer or list the renderers by '-r list'.\n");
}

