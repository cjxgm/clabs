
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "tree.h"

static void print_tree(tree_t * tree, int depth);
static void do_save_tree(FILE * fp, tree_t * tree);
static tree_t * do_load_tree(FILE * fp);

void save_tree(const char * filename, tree_t * tree)
{
	print_tree(tree, 0);

	FILE * fp = fopen(filename, "w");
	if (!fp) return;

	do_save_tree(fp, tree);

	fclose(fp);
	// free_tree(tree);
}

tree_t * load_tree(const char * filename)
{
	FILE * fp = fopen(filename, "r");
	if (!fp) return NULL;

	tree_t * tree = do_load_tree(fp);

	fclose(fp);
	return tree;
}

void free_tree(tree_t * tree)
{
	if (!tree) return;

	switch ((int)tree->type) {
		case TREE_OP:
			{
				tree_op_t * t = (tree_op_t *) tree;

				int i;
				for (i=0; i<t->op_cnt; i++)
					free_tree(t->ops[i]);
			} break;
	}

	free_tree(tree->next);
	free(tree);
}

tree_t * new_op_tree(int opr, int op_cnt, /*ops*/...)
{
	tree_op_t * t;
	va_list sp;
	int i;

	t = malloc(sizeof(tree_op_t) + op_cnt * sizeof(tree_t *));
	t->tree.type = TREE_OP;
	t->opr       = opr;
	t->op_cnt    = op_cnt;

	va_start(sp, op_cnt);
	for (i=0; i<op_cnt; i++)
		t->ops[i] = va_arg(sp, tree_t *);
	va_end(sp);

	return (tree_t *) t;
}

tree_t * new_cnst_tree(int value)
{
	tree_cnst_t * t;

	t = malloc(sizeof(tree_cnst_t));
	t->tree.type = TREE_CNST;
	t->value     = value;

	return (tree_t *) t;
}

tree_t * new_ident_tree(int id)
{
	tree_ident_t * t;

	t = malloc(sizeof(tree_ident_t));
	t->tree.type = TREE_IDENT;
	t->id        = id;

	return (tree_t *) t;
}

void print_tree(tree_t * tree, int depth)
{
	for (; tree; tree = tree->next) {
		int i = depth * 4;
		while (i--) putchar(' ');

		switch ((int)tree->type) {
			case TREE_OP:
				{
					tree_op_t * t = (tree_op_t *) tree;
					printf("OP('%c'|%d)\n", t->opr, t->opr);

					for (i=0; i<t->op_cnt; i++)
						print_tree(t->ops[i], depth+1);
				} break;
			case TREE_CNST:
				{
					tree_cnst_t * t = (tree_cnst_t *) tree;
					printf("CNST(%d)\n", t->value);
				} break;
			case TREE_IDENT:
				{
					tree_ident_t * t = (tree_ident_t *) tree;
					printf("IDENT(%c)\n", t->id + 'a');
				} break;
		}
	}
}


#define WRITE(DATA)		fwrite(&(DATA), sizeof(typeof(DATA)), 1, fp)
void do_save_tree(FILE * fp, tree_t * tree)
{
	int i;

	for (; tree; tree = tree->next) {
		switch ((int)tree->type) {
			case TREE_OP:
				{
					tree_op_t * t = (tree_op_t *) tree;

					WRITE(tree->type);
					WRITE(t->opr);
					WRITE(t->op_cnt);

					for (i=0; i<t->op_cnt; i++)
						do_save_tree(fp, t->ops[i]);
				} break;
			case TREE_CNST:
				{
					tree_cnst_t * t = (tree_cnst_t *) tree;

					WRITE(tree->type);
					WRITE(t->value);
				} break;
			case TREE_IDENT:
				{
					tree_ident_t * t = (tree_ident_t *) tree;

					WRITE(tree->type);
					WRITE(t->id);
				} break;
		}
	}

	i = 0;
	WRITE(i);
}
#undef WRITE

#define READ(DATA)		fread(&(DATA), sizeof(typeof(DATA)), 1, fp)
tree_t * do_load_tree(FILE * fp)
{
	int i;
	int type;
	tree_t * tree = NULL;

	if (READ(type) == EOF) return NULL;

	switch (type) {
		case 0:
			return NULL;
		case TREE_OP:
			{
				int opr;
				int op_cnt;

				READ(opr);
				READ(op_cnt);
				tree_op_t * t = malloc(sizeof(tree_op_t)
						+ op_cnt * sizeof(tree_t *));

				t->tree.type = type;
				t->opr       = opr;
				t->op_cnt    = op_cnt;

				for (i=0; i<op_cnt; i++)
					t->ops[i] = do_load_tree(fp);

				tree = (tree_t *) t;
			} break;
		case TREE_CNST:
			{
				tree_cnst_t * t = malloc(sizeof(tree_cnst_t));

				t->tree.type = type;
				READ(t->value);

				tree = (tree_t *) t;
			} break;
		case TREE_IDENT:
			{
				tree_ident_t * t = malloc(sizeof(tree_ident_t));

				t->tree.type = type;
				READ(t->id);

				tree = (tree_t *) t;
			} break;
	}

	tree->next = do_load_tree(fp);
	return tree;
}
#undef READ

