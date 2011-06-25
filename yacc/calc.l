%option noyywrap

VAR			[a-zA-Z]
NUM			[0-9]+
SYMBOL		[+\-*/\n=()]
BLANK		[ \t]
IDENT		"reset"|"mem"|"is"|"and"|"or"|"not"|"quit"

%{
typedef struct _key_entry
{
	const char * name;
	int id;
}
KEY_ENTRY;

KEY_ENTRY keytab[] = {
	{ "reset",		KRESET		},
	{ "mem",		KMEM		},
	{ "is",			KIS			},
	{ "and",		KAND		},
	{ "or",			KOR			},
	{ "not",		KNOT		},
	{ "quit",		KQUIT		},
};

int key_to_id(const char * name)
{
	int i;
	for (i=0; i<sizeof(keytab)/sizeof(keytab[0]); i++)
		if (strcmp(keytab[i].name, name) == 0)
			return keytab[i].id;
	return 0;
}
%}

%%

{VAR}			{	yylval.ch = tolower(yytext[0]) - 'a';
					return VAR;	}
{NUM}			{	yylval.num = atof(yytext);
					return NUM;	}
{SYMBOL}		return yytext[0];
{IDENT}			return key_to_id(yytext);
">"				return SGT;
"<"				return SLT;
">="			return SGE;
"<="			return SLE;
"!="			return SNE;

{BLANK}			;
.				yyerror("unknown character");

