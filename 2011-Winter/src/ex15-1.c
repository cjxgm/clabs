/** ex15-1.c
 * 
 **
 * eXerigumo Clanjor
 **
 * Under GPLv3 and KING Principle
 */

#include <stdio.h>

#define n 8
#define NORMAL "\033[0m"
#define NORMAL_BOLD "\033[01m"
#define RED_BOLD "\033[01;31m"
#define YELLOW_BOLD "\033[01;32m"

int main(int argc, char *argv[])
{
	int ns[n], rs[n], i, jl, jr;

	// Read all nums
	printf("Please input %s%d%s positive numbers: ",
			NORMAL_BOLD, n, NORMAL);
	for (i=0; i<n; i++){
		scanf("%d", &ns[i]);
		// Same numbers go wrong
		if (i && ns[i] == ns[0]){
			printf("%sInvalid number %s%d%s\n", 
					RED_BOLD, YELLOW_BOLD, ns[i], NORMAL);
			// Restart from the second number
			i = 0;
		}
	}

	// Work
	for (i=1, jl=0, jr=n-1; i<n; i++){
		if (ns[i] < ns[0]){
			rs[jl] = ns[i];
			jl++;
		}
		else if (ns[i] > ns[0]){
			rs[jr] = ns[i];
			jr--;
		}
		else printf("FUCK!\n");
	}
	if (jl != jr) printf("FUCK %d, %d\n", jl, jr);
	rs[jl] = ns[0];

	// Output
	for (i=0; i<n; i++) printf("%d%c", rs[i], (i==n-1?'\n':' '));

	return 0;
}

