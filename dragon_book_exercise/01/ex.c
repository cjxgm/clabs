/* A simple example of mid-fix to postfix converter */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int lookahead;

void expr(void);
void term(void);
void match(int t);
void error(void);

int main(void)
{
	lookahead = getchar();
	expr();
	putchar('\n');
	return 0;
}

void expr(void)
{
	term();
	for (;;){
		if (lookahead == '+'){
			match('+');
			term();
			putchar('+');
		}
		else if (lookahead == '-'){
			match('-');
			term();
			putchar('-');
		}
		else break;
	}
}

void term(void)
{
	if (isdigit(lookahead)){
		putchar(lookahead);
		match(lookahead);
	}
	else error();
}

void match(int t)
{
	if (lookahead == t)
		lookahead = getchar();
	else error();
}

void error(void)
{
	printf("Syntax error!\n");
	exit(1);
}

