#define	__MODULE__	"LIB_RBTREE"
#define	__IDENT__	"X.00-03"
#define	__REV__		"0.03.0"

#ifdef	__GNUC__
	#ident			__IDENT__
#endif

/* We don't interesting in the some warnings */
#pragma GCC diagnostic ignored  "-Wparentheses"
#pragma GCC diagnostic ignored	"-Wdiscarded-qualifiers"
#pragma GCC diagnostic ignored	"-Wmissing-braces"
//#pragma GCC diagnostic ignored	"-Wfdollars-in-identifiers"

/*
**++
**
**  FACILITY:  RB Tree API, an example/test of using RB Tree API
**
**  DESCRIPTION: A simple example for demonstration of using RB Tree API in application code.
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

#include	"lib_rbtree.h"





/*************************************
 *  maintain Red-Black tree balance  *
 *  after inserting node x           *
 *************************************/

static void	__rbtree_insertFixup	(
				RB_TREE *	tree,
				RB_NODE	*	x
					)
{
	/* check Red-Black properties */
	while ( (x != tree->root) && (x->parent->color == RBTREE$K_RED) )
		{
		/* we have a violation */
		if ( x->parent == x->parent->parent->left )
			{
			RB_NODE *y = x->parent->parent->right;

			if (y->color == RBTREE$K_RED)
				{ /* uncle is RED */
				x->parent->color = RBTREE$K_BLACK;
				y->color = RBTREE$K_BLACK;
				x->parent->parent->color = RBTREE$K_RED;
				x = x->parent->parent;
				}
			else	{ /* uncle is BLACK */
				if (x == x->parent->right)
					{
					/* make x a left child */
					x = x->parent;

					__rbtree_rotateLeft(tree, x);
					}

				/* recolor and rotate */
				x->parent->color = RBTREE$K_BLACK;
				x->parent->parent->color = RBTREE$K_RED;

				__rbtree_rotateRight(tree, x->parent->parent);
				}
			}
		else	{
			/* mirror image of above code */
			RB_NODE *y = x->parent->parent->left;

			if (y->color == RBTREE$K_RED)
				{ /* uncle is RED */
				x->parent->color = RBTREE$K_BLACK;
				y->color = RBTREE$K_BLACK;
				x->parent->parent->color = RBTREE$K_RED;
				x = x->parent->parent;
				}
			else	{ /* uncle is BLACK */
				if (x == x->parent->left)
					{
					x = x->parent;
					__rbtree_rotateRight(tree, x);
					}

				x->parent->color = RBTREE$K_BLACK;
				x->parent->parent->color = RBTREE$K_RED;

				__rbtree_rotateLeft(tree, x->parent->parent);
				}
			}
		}

	tree->root->color = RBTREE$K_BLACK;
}

int	lib$rbtree_insert	(
			RB_TREE	*	tree,
			void		*key,
			RB_NODE	*	node_to_insert,
			RB_NODE	**	node_is_found
				)
{
RB_NODE *current, *parent;
int	res;

	/* find where node belongs */
	current = tree->root;
	parent = 0;

	while ( current != &tree->nl_node )
		{
		if ( !(res = tree->keycmp(tree, key, current->pdata)) )
			return	*node_is_found = current, 1;	/* Return existen node, SS$_NORMAL */

		parent = current;
		current = (0 > res) ? current->left : current->right;
		}

	node_to_insert->parent = parent;
	node_to_insert->left = &tree->nl_node;
	node_to_insert->right = &tree->nl_node;
	node_to_insert->color = RBTREE$K_RED;

	/* insert node in tree */
	if ( parent )
		{
		if ( 0 > tree->keycmp(tree, key, parent->pdata) )
			parent->left = node_to_insert;
		else	parent->right = node_to_insert;
		}
	else	tree->root = node_to_insert;

	__rbtree_insertFixup(tree, node_to_insert);

	tree->node_nr++;

	return	1;
}



/*************************************
 *  maintain Red-Black tree balance  *
 *  after deleting node x            *
 *************************************/

static inline void	__rbtree_deleteFixup	(
				RB_TREE	*	tree,
				RB_NODE *	x
				)
{
	while (x != tree->root && x->color == RBTREE$K_BLACK)
		{
		if (x == x->parent->left)
			{
			RB_NODE *w = x->parent->right;

			if (w->color == RBTREE$K_RED)
				{
				w->color = RBTREE$K_BLACK;
				x->parent->color = RBTREE$K_RED;

				__rbtree_rotateLeft (tree, x->parent);

				w = x->parent->right;
				}

			if ( (w->left->color == RBTREE$K_BLACK) && (w->right->color == RBTREE$K_BLACK))
				{
				w->color = RBTREE$K_RED;
				x = x->parent;
				}
			else	{
				if ( w->right->color == RBTREE$K_BLACK )
					{
					w->left->color = RBTREE$K_BLACK;
					w->color = RBTREE$K_RED;

					__rbtree_rotateRight (tree, w);

					w = x->parent->right;
					}

				w->color = x->parent->color;
				x->parent->color = RBTREE$K_BLACK;
				w->right->color = RBTREE$K_BLACK;

				__rbtree_rotateLeft (tree, x->parent);

				x = tree->root;
				}
			}
		else	{
			RB_NODE *w = x->parent->left;

			if ( w->color == RBTREE$K_RED )
				{
				w->color = RBTREE$K_BLACK;
				x->parent->color = RBTREE$K_RED;

				__rbtree_rotateRight (tree, x->parent);

				w = x->parent->left;
				}
			if ( (w->right->color == RBTREE$K_BLACK) && (w->left->color == RBTREE$K_BLACK) )
				{
				w->color = RBTREE$K_RED;
				x = x->parent;
				}
			else	{
				if ( w->left->color == RBTREE$K_BLACK )
					{
					w->right->color = RBTREE$K_BLACK;
					w->color = RBTREE$K_RED;

					__rbtree_rotateLeft (tree, w);

					w = x->parent->left;
					}

				w->color = x->parent->color;
				x->parent->color = RBTREE$K_BLACK;
				w->left->color = RBTREE$K_BLACK;

				__rbtree_rotateRight (tree, x->parent);

				x = tree->root;
				}
			}
		}

	x->color = RBTREE$K_BLACK;
}


/*
 *   DESCRIPTION:  Initialize field of the tree's context area by initial values;
 *	supposed to be called before any other calls

 *
 *   INPUTS:
 *	tree:	an address of the tree
 *	keycmp:	an address of the key compare routine
 *
 *   OUTPUTS:
 *	NONE
 *
 *  RETURNS:
 *	0:	ERROR
 *	1:	SUCCESS
 */

int	lib$rbtree_init		(
			RB_TREE		*tree,
			void		*keycmp
				)
{
	tree->nl_node.left = tree->nl_node.right = tree->root = &tree->nl_node;
	tree->nl_node.parent = 0;
	tree->nl_node.data = 0;
	tree->keycmp = keycmp;
	tree->nl_node.color = RBTREE$K_BLACK;

	return	1;	/* SS$_NORMAL */
}





/*
 *   DESCRIPTION:  Remove a given node from the tree, by advised that you can can peromfs deallocation
 *	of memory for 'node_to_free' NOT FOR 'node_to_kill'!
 *
 *   INPUTS:
 *	tree:	an address of the tree
 *	node_to_kill: an address of the node to be 'removed' from the tree
 *
 *   OUTPUT:s
 *	node_to_free: an address of the node's area is can be deallocated
 *
 *  RETURNS:
 *	0:	Record-Not-Found
 *	1:	SUCCESS, node_to_free contains an address to be deallocated memory area
 */
int	lib$rbtree_delete	(
				RB_TREE	*	tree,
				RB_NODE *	node_to_kill,
				RB_NODE **	node_to_free
				)
{
RB_NODE	*x = NULL, *y = NULL;

	*node_to_free = NULL;

	if ( !node_to_kill || (node_to_kill == &tree->nl_node) )
		return	0;	/* RNF */


	if ( (node_to_kill->left == &tree->nl_node) || (node_to_kill->right == &tree->nl_node) )
		{
		/* y has a NIL node as a child */
		y = node_to_kill;
		}
	else	{
		/* find tree successor with a NIL node as a child */
		y = node_to_kill->right;

		while (y->left != &tree->nl_node)
			y = y->left;
		}

	/* x is y's only child */
	if ( y->left != &tree->nl_node )
		x = y->left;
	else	x = y->right;

	/* remove y from the parent chain */
	x->parent = y->parent;

	if ( (x->parent = y->parent) )
		{
		if (y == y->parent->left)
			y->parent->left = x;
		else	y->parent->right = x;
		}
	else	tree->root = x;

	if ( y != node_to_kill )
		{
		//node_to_kill->data = y->data;

		node_to_kill->data  = node_to_kill->data ^ y->data;
		y->data = node_to_kill->data ^ y->data;
		node_to_kill->data  = node_to_kill->data ^ y->data;

		}

	if ( y->color == RBTREE$K_BLACK )
		__rbtree_deleteFixup (tree, x);

	tree->node_nr--;		/* Adjust nodes counter */

	*node_to_free = y;		/* Return an address of node to be freed */

	return	1;			/* SS$_NORMAL */
}


/*
 *   DESCRIPTION:  Lookup a node in three with given key
 *
 *   INPUTS:
 *	tree:	an address of the tree
 *	key:	an adddress of the key's area
 *
 *   OUTPUT:s
 *	retnode: an address of the pointer to be accept found node
 *
 *  RETURNS:
 *	0:	Record-Not-Found
 *	1:	SUCCESS, retnode contains an address of found node
 */

int	lib$rbtree_lookup	(
			RB_TREE	*	tree,
				void	*key,
			RB_NODE	**	retnode
				)
{
RB_NODE *current = tree->root;
int	res;

	while ( current != &tree->nl_node )
		{
		if ( !(res = tree->keycmp(tree, key, current->pdata)) )
			return	*retnode =  current, 1;	/* Return found node, SS$_NORMAL */

		current = 0 > res ? current->left : current->right;
		}

	return	0;
}
