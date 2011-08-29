%{
	#include <stdio.h>
	#include "tree.h"
	#include "vm.h"

	int yylex(void);
	void yyerror(char * err);
%}

%union
{
	int s32;
	tree_t * tree;
}

%token ST_SEP	/* statement seperator */
%token KPRINT
%token KWHILE
%token <s32> IDENT
%token <s32> INTEGER
%left '+' '-'

%type <tree> program stmt_list stmt stmt_block expr value

%%

cjscript: program { save_tree("test.tree", $1); exec_tree($1); };

program: stmt_list ;

stmt_list:
	  stmt stmt_list		{
		  						if ($1) {
									$1->next = $2;
									$$ = $1;
								}
								else $$ = $2;
							}
	|						{ $$ = NULL; }
	;

stmt:
	  ST_SEP				{ $$ = NULL; }
	| KPRINT '(' expr ')' ST_SEP
							{ $$ = new_op_tree(KPRINT, 1, $3); }
	| KWHILE '(' expr ')' stmt_block
							{ $$ = new_op_tree(KWHILE, 2, $3, $5); }
	| IDENT '=' expr ST_SEP	{ $$ = new_op_tree('=', 2,
										new_ident_tree($1), $3); }
	;

stmt_block:
	  stmt					{ $$ = $1; }
	| '{' stmt_list '}'		{ $$ = $2; }
	;

expr:
	  value
	| expr '+' expr			{ $$ = new_op_tree('+', 2, $1, $3); }
	| expr '-' expr			{ $$ = new_op_tree('-', 2, $1, $3); }
	;

value:
	  IDENT					{ $$ = new_ident_tree($1); }
	| INTEGER				{ $$ = new_cnst_tree($1); }
	;


%%

#include <stdio.h>

void yyerror(char * err)
{
	printf("\e[1;31m" "%s" "\e[m" "\n", err);
}

