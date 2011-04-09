#include <stdio.h>
#include "renderer_stdout.h"

void renderer_stdout_plain(const char *str)
{
	printf("%s", str);
}

void renderer_stdout_key(const char *str)
{
	printf("\033[01;34m%s\033[0m", str);
}

void renderer_stdout_escape(const char *str)
{
	printf("\033[01;31m%s\033[0m", str);
}

