#ifndef	LIB_RBTREE_H_INCLUDED
#define	LIB_RBTREE_H_INCLUDED	1


#ifdef	__GNUC__
	#pragma GCC diagnostic ignored  "-Wparentheses"
	#pragma	GCC diagnostic ignored	"-Wdate-time"
	#pragma	GCC diagnostic ignored	"-Wunused-variable"
#endif


#ifdef _WIN32
	#pragma once
	#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

/*
**++
**
**  FACILITY:  A portable Red-Black Tree API
**
**  DESCRIPTION: This module contains data structures and constant definition is supposed to be used
**	in the application programd and partialy in the LIB_RBTREE.C
**
**  AUTHORS: Ruslan R. (The BadAss SysMan) Laishev
**
**  CREATION DATE:  27-AUG-2020
**
**  MODIFICATION HISTORY:
**
**
**--
*/

#ifdef __cplusplus
extern "C" {
#endif


#pragma	pack	(push, 8)				/* Set aligment to double-word (8 octets) boundary */


#define	RBTREE$K_RED	1
#define	RBTREE$K_BLACK	(!RBTREE$K_RED)


							/* Red-Black tree related definitions */
typedef struct __rb_tree_node__ {
    struct __rb_tree_node__	*left,			/* left child */
				*right,			/* right child */
				*parent;		/* parent */
		int		color;			/* node color (BLACK, RED) */

	union	{
		u_int64_t	data;			/* Key itself or address of the key's area */
		void		*pdata;
		};

} RB_NODE;


typedef struct __rb_tree__
{
		RB_NODE		*root,
				nl_node;		/* NULL/NIL node */

		u_int64_t	node_nr;		/* A number of nodes in the tree */

	union	{
		u_int64_t	user;			/* An user data itself or address to user data area */
		void		*puser;
		};

		int	(* keycmp) (struct __rb_tree__ *tree, void *key1, void *pdata2);
} RB_TREE;

#pragma	pack	(pop)

/*
 *   DESCRIPTION:
 *	Internaly used routine to performs a single rotaion of the node to left
 *
 *   INPUTS:
 *	tree:	an address of the tree
 *	x:	an address of the node to be rotated to left
 *
 *   RETURNS:
 *	NONE
 */
static inline void	__rbtree_rotateLeft	(
				RB_TREE		*tree,
				RB_NODE		*x
					)
{
RB_NODE	*y = x->right;

	/* establish x->right link */
	if ( (x->right = y->left) != &tree->nl_node )
		y->left->parent = x;

	/* establish y->parent link */
	if (y != &tree->nl_node)
		y->parent = x->parent;

	if (x->parent)
		{
		if (x == x->parent->left)
			x->parent->left = y;
		else	x->parent->right = y;
		}
	else	tree->root = y;

	/* link x and y */
	if ( (y->left = x) != &tree->nl_node)
		x->parent = y;
}




/*
 *   DESCRIPTION:
 *	Internaly used routine to performs a single rotaion of the node to right
 *
 *   INPUTS:
 *	tree:	an address of the tree
 *	x:	an address of the node to be rotated to right
 *
 *   RETURNS:
 *	NONE
 */

static inline void	__rbtree_rotateRight	(
				RB_TREE		*tree,
				RB_NODE	*x
					)

{
RB_NODE	*y = x->left;

	/* establish x->left link */
	if ( (x->left = y->right) != &tree->nl_node)
		y->right->parent = x;

	/* establish y->parent link */
	if (y != &tree->nl_node)
		y->parent = x->parent;

	if (x->parent)
		{
		if (x == x->parent->right)
			x->parent->right = y;
		else	x->parent->left = y;
		}
	else	tree->root = y;

	/* link x and y */
	if ( (y->right = x) != &tree->nl_node)
		x->parent = y;
}


/* Declare prototypes for the RBTree API routines */

int	lib$rbtree_init		(RB_TREE *tree, void *keycmp);
int	lib$rbtree_insert	(RB_TREE *tree, void *key, RB_NODE *node_to_insert, RB_NODE **node_is_found);
int	lib$rbtree_lookup	(RB_TREE *tree, void *key, RB_NODE **node_is_found);
int	lib$rbtree_delete	(RB_TREE *tree, RB_NODE *node_to_kill, RB_NODE **node_to_free);


/*
 *   DESCRIPTION:
 *	Return a smallest node on the tree
 *
 *   INPUTS:
 *	tree:	an address of the tree
 *
 *   RETURNS:
 *	address of smalet node
 *	NULL:	tree is empty
 *
 */
static	inline	RB_NODE	*lib$rbtree_head (
			RB_TREE *	tree
				)
{
RB_NODE		*currentNode = tree->root;

	if ( currentNode != &tree->nl_node )
		{
		/* Find the smallest node */
		while ( currentNode->left != &tree->nl_node )
			currentNode = currentNode->left;

		return currentNode;
		}

	return NULL;
}

/*
 *   DESCRIPTION:
 *	Return a next node (relatively to given) on the tree
 *
 *   INPUTS:
 *	tree:	an address of the tree
 *	node:	a current node in the tree
 *
 *   RETURNS:
 *	address of next node
 *	NULL:	no more nodes
 *
 */

static inline	RB_NODE	*lib$rbtree_next	(
			RB_TREE	*	tree,
			RB_NODE	*	node
				)
{
RB_NODE	*currentNode;

	if ( node->right != &tree->nl_node )
		{
		/* look for right child's smallest node */
		currentNode = node->right;

		while ( currentNode->left != &tree->nl_node )
			currentNode = currentNode->left;

		return	currentNode;
		}
	else if ( node->parent != &tree->nl_node )
		{
		currentNode = node;

		/* the node has no right child, look back */
		while ( currentNode != tree->root )
			if ( currentNode->parent->right != currentNode )
				return currentNode->parent;
			else	currentNode = currentNode->parent;
		}

	return	NULL;
}


/*
 *   DESCRIPTION:
 *	Return a greatest node on the tree
 *
 *   INPUTS:
 *	tree:	an address of the tree
 *
 *   RETURNS:
 *	address of greatest node
 *	NULL:	tree is empty
 *
 */

static inline	RB_NODE	*lib$rbtree_tail	(
			RB_TREE *	tree
				)
{
RB_NODE	*currentNode = tree->root;

	if ( currentNode != &tree->nl_node )
		{
		/* find the greatest node */
		while ( currentNode->right != &tree->nl_node )
			currentNode = currentNode->right;

		return	currentNode;
		}

	return	NULL;
}

/*
 *   DESCRIPTION:
 *	Return a previous node (relatively to given) on the tree
 *
 *   INPUTS:
 *	tree:	an address of the tree
 *	node:	a current node in the tree
 *
 *   RETURNS:
 *	address of previous node
 *	NULL:	no more nodes
 *
 */
static inline	RB_NODE	*lib$rbtree_prev	(
			RB_TREE	*	tree,
			RB_NODE	*	node
				)
{
RB_NODE	*currentNode;

	if ( node->left != &tree->nl_node )
		{
		currentNode = node->left;

		/* look for left child's greatest node */
		while ( currentNode->right != &tree->nl_node )
			currentNode = currentNode->right;

		return currentNode;
		}
	else if ( node->parent != &tree->nl_node )
		{
		currentNode = node;

		/* the node has no left child, look back */
		while ( currentNode != tree->root )
			if ( currentNode->parent->left != currentNode )
				return currentNode->parent;
			else	currentNode = currentNode->parent;
		}

	return	NULL;
}




#ifdef __cplusplus
	}
#endif

#endif
