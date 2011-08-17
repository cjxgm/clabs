/* bfcc - BrainFuck to C Compiler */

#include <stdio.h>

int main(void)
{
	int ch;
	int loop = 0;
	int loops[256] = { 0 };
	char lp = 0;

	puts(
		"#include <stdio.h>\n"
		"\n"
		"static unsigned char mem[256] = { 0 };\n"
		"static unsigned char mp = 0;\n"
		"\n"
		"int main(void)\n"
		"{"
	);

	while ((ch = getchar()) != EOF) {
		switch (ch) {
			case '+':
				puts("\tmem[mp]++;");
				break;
			case '-':
				puts("\tmem[mp]--;");
				break;
			case '>':
				puts("\tmp++;");
				break;
			case '<':
				puts("\tmp--;");
				break;
			case ',':
				puts("\tmem[mp] = getchar();");
				break;
			case '.':
				puts("\tputchar(mem[mp]);");
				break;
			case '[':
				loops[lp] = loop++;
				printf("\tif (!mem[mp]) goto label_%d_end;\n", loops[lp]);
				printf("label_%d_start:\n", loops[lp]);
				lp++;
				break;
			case ']':
				lp--;
				printf("\tif (mem[mp]) goto label_%d_start;\n", loops[lp]);
				printf("label_%d_end:\n", loops[lp]);
				break;
		}
	}

	puts(
		"\n\treturn 0;\n"
		"}"
	);

	return 0;
}

