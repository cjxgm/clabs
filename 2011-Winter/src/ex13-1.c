/** ex13-1.c
 * 
 **
 * eXerigumo Clanjor
 **
 * Under GPLv3 and KING Principle
 */

#include <stdio.h>

int main(int argc, char *argv[])
{
	char num[5] = {'0'};
	for (num[0] = '1'; num[0]<'5'; num[0]++)
		for (num[1] = '1'; num[1]<'5'; num[1]++)
			for (num[2] = '1'; num[2]<'5'; num[2]++)
				for (num[3] = '1'; num[3]<'5'; num[3]++)
					printf("%s%c", num, (num[3]=='4' ? '\n' : '\t'));

	return 0;
}

