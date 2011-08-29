
#ifndef __TREE__
#define __TREE__

typedef enum
{ TREE_OP = 1, TREE_CNST, TREE_IDENT }
tree_e;

typedef struct tree
{
	tree_e        type;
	struct tree * next;
}
tree_t;

typedef struct tree_op
{
	tree_t   tree;
	int      opr;
	int      op_cnt;
	tree_t * ops[];
}
tree_op_t;

typedef struct tree_cnst
{
	tree_t tree;
	int    value;
}
tree_cnst_t;

typedef struct tree_ident
{
	tree_t tree;
	int    id;
}
tree_ident_t;

void     save_tree(const char * filename, tree_t * tree);
tree_t * load_tree(const char * filename);
void     free_tree(tree_t * tree);

tree_t * new_op_tree(int opr, int op_cnt, /*ops*/...);
tree_t * new_cnst_tree(int value);
tree_t * new_ident_tree(int id);

#endif

