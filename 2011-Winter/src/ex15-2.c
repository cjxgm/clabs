/** ex15-2.c
 * 
 **
 * eXerigumo Clanjor
 **
 * Under GPLv3 and KING Principle
 */

#include <stdio.h>
#include <stdlib.h>

#define NORMAL "\033[0m"
#define NORMAL_BOLD "\033[01m"

int tall = 0;
// prototype
void seperate_natural(int max_deep, int deep);

int main(int argc, char *argv[])
{
	int n;
	printf("Input a natural number: ");
	scanf("%d", &n);

	seperate_natural(n, 0);
	printf("Total: %s%d%s\n", NORMAL_BOLD, tall, NORMAL);

	return 0;
}

void seperate_natural(int max_deep, int deep)
{
	static int *ns = NULL;
	static int total = 0;
	static int begins = 0;

	if (deep == 0 && ns == NULL){
		total = 0;
		ns = malloc(sizeof(int) * max_deep);
	}

	if (total == max_deep){
		printf("%d", total);
		int i;
		for (i=0; i<deep; i++)
			printf(" %c %s%d%s", (i?'+':'='), NORMAL_BOLD, ns[i], NORMAL);
		printf("\n");
		tall++;
		return;
	}
	if (deep == max_deep) return;

	int i;
	for (i=begins; i<max_deep-1; i++){
		ns[deep] = i+1;
		total += i+1;
		begins = i;
		seperate_natural(max_deep, deep+1);
		total -= i+1;
	}
}

