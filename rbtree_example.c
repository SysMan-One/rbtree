#define	__MODULE__	"RBTEXMPL"
#define	__IDENT__	"X.00-02"
#define	__REV__		"0.02.0"

#ifdef	__GNUC__
	#ident			__IDENT__
#endif

/* We don't interesting in the some warnings */
#pragma GCC diagnostic ignored  "-Wparentheses"
#pragma GCC diagnostic ignored	"-Wdiscarded-qualifiers"
#pragma GCC diagnostic ignored	"-Wmissing-braces"
#pragma GCC diagnostic ignored	"-Wfdollars-in-identifiers"

/*
**++
**
**  FACILITY:  nDPI, a test of using nDPI API
**
**  DESCRIPTION: A simplest example of using nDPI API to performs analyzing of traffic.
**
**  BUILD:
**
**  USAGE:
**
**  AUTHORS: Ruslan R. (The BadAss SysMan) Laishev
**
**  CREATION DATE:  17-FEB-2020
**
**  MODIFICATION HISTORY:
**--
*/

#ifndef	__ARCH__NAME__
#define	__ARCH__NAME__	"VAX"
#endif

#include	<stdlib.h>

#include	"rbtree.h"

/*
* Defines and includes for enable extend trace and logging
*/
#define		__FAC__	"RBTEXMPL"
#define		__TFAC__ __FAC__ ": "
#include	"utility_routines.h"

int	g_trace = 1;


RB_TREE	tree = {0};

static int	cmpkey	(
		RB_TREE	*tree,
		void	*key1,
		void	*key2
			)

{
int	ix, iy;

	ix = * ((int *) key1);
	iy = * ((int *) key2);

	return	(ix - iy);

}

typedef	struct __flow__ {
	union	{
		RB_TREE_NODE	rb_node;
		ENTRY		ent;
		};

	ASC	val;

} FLOW;



const	char items [][8] = {"E", "F", "G", "H", "A", "B", "C", "D"};
const	int	items_nr = $ARRSZ(items);

FLOW	fe[$ARRSZ(items)] = {0};

int	main	(int argc, char* argv[])
{
int	status, i;
FLOW	*pf;
RB_TREE_NODE	*pnode, *tmpnode;

	status = rb_tree_init(&tree, sizeof(int), cmpkey);

	for (i = 0; i < items_nr; i++)
		{
		pf = &fe[i];

		pf->rb_node.key = items[i][0];
		__util$str2asc(items[i], &pf->val);

		status = rb_tree_insert(&tree, &pf->rb_node);

		$IFTRACE(g_trace, "[tree=%d] Inserted node[key=%d, val=%.*s]", tree.nr_node, pf->rb_node.key, $ASC(&pf->val));
		}

	for (pnode = rb_tree_node_head(&tree); pnode; pnode = rb_tree_node_next(&tree, pnode), i)
		pf = (FLOW *) pnode,  $IFTRACE(g_trace, "Retrieved node[key=%d, val=%.*s]", pnode->key, $ASC(&pf->val));

	for (pnode = rb_tree_node_head(&tree); pnode; pnode = rb_tree_node_next(&tree, pnode), i)

		pf = (FLOW *) pnode,  $IFTRACE(g_trace, "Retrieved node[key=%d, val=%.*s]", pnode->key, $ASC(&pf->val));
	for (pnode = rb_tree_node_head(&tree); pnode; pnode = rb_tree_node_next(&tree, pnode), i)
		pf = (FLOW *) pnode,  $IFTRACE(g_trace, "Retrieved node[key=%d, val=%.*s]", pnode->key, $ASC(&pf->val));

	for (pnode = rb_tree_node_head(&tree); pnode; )
		{
		tmpnode = pnode;
		pf = (FLOW *) (tmpnode);

		$IFTRACE(g_trace, "[tree=%d] Remove node[key=%d, val=%.*s] ...", tree.nr_node, pnode->key, $ASC(&pf->val));

		pnode = rb_tree_node_next(&tree, pnode);

		rb_tree_remove(&tree, tmpnode);

		$IFTRACE(g_trace, "[tree=%d] Removed.", tree.nr_node);
		}



	for (pnode = rb_tree_node_head(&tree); pnode; pnode = rb_tree_node_next(&tree, pnode), i)
		pf = (FLOW *) pnode,  $IFTRACE(g_trace, "[tree=%d] Retrieved node[key=%d, val=%.*s]", tree.nr_node, pnode->key, $ASC(&pf->val));



	return	0;
}

