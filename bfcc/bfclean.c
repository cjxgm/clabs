/* bfclean - BrainFuck CLEANer */

#include <stdio.h>

char in(char ch, char range[])
{
	while (*range)
		if (ch == *range++)
			return 1;
	return 0;
}

int main(void)
{
	int ch;
	char bf[] = "+-<>,.[]";

	while ((ch = getchar()) != EOF)
		if (in(ch, bf))
			putchar(ch);
	putchar('\n');

	return 0;
}

