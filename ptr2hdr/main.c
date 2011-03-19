#include <stdio.h>

typedef unsigned long ptr;

int main()
{
	float pi = 3.1415926;
	printf("%f\n", pi);

	float *pp = &pi;
	int p = (ptr)pp;
	printf("%d\n", p);

	int abc;
	scanf("%d", &abc);
	float *a = (float*)abc;
	printf("%f\n", *a);
}
