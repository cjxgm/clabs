%{
#include <stdio.h>
double mem[26] = { 0 };

#define COLOR_OUTPUT	"\033[01;33m"
#define COLOR_NORMAL	"\033[0m"
#define COLOR_ERROR		"\033[01;31m"
#define COLOR_ERR_MSG	"\033[00;32m"
%}

%union
{
	double	num;
	char	ch;
}

// value
%token <ch>		VAR
%token <num>	NUM

// keyword
%token KRESET
%token KMEM
%token KIS
%token KAND
%token KOR
%token KNOT
%token KQUIT

// symbol (part)
%token SGT
%token SLT
%token SGE
%token SLE
%token SNE

%type <num> expr
%type <num> term
%type <num> factor
%type <num> st_is
%type <num> comp_expr
%type <num> bexpr

%%

calc		: calc line {}
			| line {}
;
line		: stmt '\n' {}
			| '\n' {}
;
stmt		: st_assign {}
			| st_mem {}
			| st_is
				{	printf(COLOR_OUTPUT "%s"
						   COLOR_NORMAL "\n",
						   ($1 ? "true" : "false"));	}
			| expr
				{	printf(COLOR_OUTPUT "%f"
						   COLOR_NORMAL "\n", $1);	}
			| st_system {}
;
st_assign	: VAR '=' expr
				{	mem[$1] = $3;
					printf(COLOR_OUTPUT "%c = %f"
						   COLOR_NORMAL "\n", $1 + 'a', $3);	}
;
st_mem		: KMEM KRESET
				{	int i;
					for (i=0; i<26; i++)
						mem[i] = 0;	}
			| KMEM
				{	int i;
					for (i=0; i<26; i++)
						printf(COLOR_OUTPUT "%c = %f"
							   COLOR_NORMAL "\n",
							   i + 'a', mem[i]);	}
			| KMEM VAR VAR
				{	int i;
					for (i=$2; i<=$3; i++)
						printf(COLOR_OUTPUT "%c = %f"
							   COLOR_NORMAL "\n",
							   i + 'a', mem[i]);	}
;
st_is		: KIS comp_expr
				{	$$ = $2;	}
;
comp_expr	: '(' comp_expr ')'
				{	$$ = $2;	}
			| KNOT comp_expr
				{	$$ = !$2;	}
			| comp_expr KAND bexpr
				{	$$ = $1 && $3;	}
			| comp_expr KOR bexpr
				{	$$ = $1 || $3;	}
			| bexpr
;
bexpr		: '(' bexpr ')'
				{	$$ = $2;	}
			| expr '=' expr
				{	$$ = ($1 == $3);	}
			| expr SNE expr
				{	$$ = ($1 != $3);	}
			| expr SGT expr
				{	$$ = ($1 > $3);	}
			| expr SLT expr
				{	$$ = ($1 < $3);	}
			| expr SGE expr
				{	$$ = ($1 >= $3);	}
			| expr SLE expr
				{	$$ = ($1 <= $3);	}
;
expr		: expr '+' term
				{	$$ = $1 + $3;	}
			| expr '-' term
				{	$$ = $1 - $3;	}
			| term
;
term		: term '*' factor
				{	$$ = $1 * $3;	}
			| term factor
				{	$$ = $1 * $2;	}
			| term '/' factor
				{	$$ = $1 / $3;	}
			| factor
;
factor		: NUM
			| VAR
				{	$$ = mem[$1];	}
			| '(' expr ')'
				{	$$ = $2;	}
			| '-' factor
				{	$$ = -$2;	}
;
st_system	: KQUIT
				{	return -1;	}

%%
#include "calc.lex.c"

int yyerror(const char * err)
{
	fprintf(stderr, COLOR_ERROR "error: "
			COLOR_ERR_MSG "%s"
			COLOR_NORMAL "\n", err);
	return 0;
}

int main(void)
{
	while (yyparse() != -1) {}
	return 0;
}

