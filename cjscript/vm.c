
#include <stdio.h>
#include "vm.h"
#include "sym.h"
#include "y.tab.h"

int exec_tree(tree_t * tree)
{
	tree_op_t * op;
	int rv;

	while (tree) {
		rv = 0;

		switch ((int)tree->type) {
			case TREE_CNST:
				rv = ((tree_cnst_t *)tree)->value;
				break;
			case TREE_IDENT:
				rv = sym[((tree_ident_t *)tree)->id];
				break;
			case TREE_OP:
				op = (tree_op_t *) tree;
				switch (op->opr) {
					case '+':
						rv = exec_tree(op->ops[0])
							+ exec_tree(op->ops[1]);
						break;
					case '-':
						rv = exec_tree(op->ops[0])
							- exec_tree(op->ops[1]);
						break;
					case '=':
						sym[((tree_ident_t *)op->ops[0])->id] =
							exec_tree(op->ops[1]);
						break;
					case KPRINT:
						printf("%d\n", exec_tree(op->ops[0]));
						break;
					case KWHILE:
						while (exec_tree(op->ops[0]))
							exec_tree(op->ops[1]);
						break;
				}
				break;
		}
		tree = tree->next;
	}

	return rv;
}

