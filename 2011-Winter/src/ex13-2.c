/** ex13-2.c
 * 
 **
 * eXerigumo Clanjor
 **
 * Under GPLv3 and KING Principle
 */

#include <stdio.h>

#define MAX 20
#define is_odd(num) (num%2)

#define NORMAL "\033[0m"
#define BLUE "\033[00;34m"
#define BLUE_BOLD "\033[01;34m"
#define print_an(n) printf("%sa%s%d%s", BLUE_BOLD, BLUE, n, NORMAL);

int main(int argc, char *argv[])
{
	int a[MAX], i;

	printf("Please input ");
	print_an(1);
	printf(" ");
	print_an(2);
	printf(": ");
	scanf("%d %d", &a[0], &a[1]);

	for (i=2; i<MAX; i++){
		a[i] = a[i-2] + a[i-1];
		if (is_odd(a[i-2]) && is_odd(a[i-1]));
		else if (is_odd(a[i-2]))
			a[i] += a[i-1];
		else if (is_odd(a[i-1]))
			a[i] += a[i-2];
		else a[i]++;
		print_an(i+1);
		printf(" = %d\n", a[i]);
	}

	return 0;
}

