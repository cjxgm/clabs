%{
	#include "tree.h"
	#include "y.tab.h"
	void yyerror(char *);		// in cjscript.y
%}

ident			[a-z]
integer			[0-9]+
symbol			[+\-={}()]
st_sep			;
blank			[ \t\n]

%%

"print"			return KPRINT;
"while"			return KWHILE;

{ident}			{
					yylval.s32 = *yytext - 'a';
					return IDENT;
				}

{integer}		{
					yylval.s32 = atoi(yytext);
					return INTEGER;
				}

{symbol}		return *yytext;

{st_sep}		return ST_SEP;

{blank}			;

.				yyerror("invalid character");

%%

int yywrap(void)
{
	return 1;
}

