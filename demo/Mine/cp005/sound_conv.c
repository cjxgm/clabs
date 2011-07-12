
#include <stdio.h>

int main(void)
{
	int ch;
	int cnt = 0;

	printf("\nstatic unsigned char module[] = {\n");

	while ((ch = getc(stdin)) != EOF) {
		if (cnt == 0)
			printf("\t\"");
		printf("\\x%2.2X", ch);
		cnt++;
		if (cnt == 16) {
			printf("\"\n");
			cnt = 0;
		}
	}

	printf("%s};\n", (cnt ? "\"\n" : ""));

	return 0;

}
