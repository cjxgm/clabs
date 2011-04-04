
#include <stdio.h>
#include <string.h>
#include "cju.h"

int main(void)
{
	char buf[81] = "Hello, World!\n";
	cju_str_t *str = cju_str_new_with_cstr(buf);

	// Some hints
	printf("Clanjor Utilities Library Test Drive\n");
	printf("Type something, \"end\" for end.\n\n");

	// Input
	for (;;) {
		printf("> ");
		scanf("%80s", buf);
		if (strcmp(buf, "end") == 0)
			break;
		cju_str_cat_cstr(str, buf);
		cju_str_append(str, '\n');
	}

	// Output
	printf("\nWow, you've typed:\n");
	printf("%s", cju_str_get_cstr(str));
	printf("\nThx for testing.\n");

	cju_str_free(str);

	return 0;
}
