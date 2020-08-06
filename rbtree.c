#define	__MODULE__	"RBTREE"
#define	__IDENT__	"X.00-02ECO1"
#define	__REV__		"0.02.1"

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
**  DESCRIPTION: A portable Red-Black Tree API
**
**  USAGE: Add rbtree.c and rbtree.h into u project.
**
**  AUTHORS: Ruslan R. (The BadAss SysMan) Laishev
**
**  CREATION DATE:  17-FEB-2020
**
**  MODIFICATION HISTORY:
**
**	18-MAR-2020	RRL	Source level optimization in the rb_tree_search() - removed a multiple calling of cmpkey() routine.
**
**	11-JUN-2020	RRL	Fixe compilation warning and other minor bugs.
**
**--
*/

#ifndef	__ARCH__NAME__
#define	__ARCH__NAME__	"VAX"
#endif

#include	<stdlib.h>	/* for NULL */
#include	<string.h>
#include	"rbtree.h"


				/* Magic */
static	const char __starlet__ [] = {"StarLet"};
static const unsigned *__starlet_ul__ = (unsigned *) &__starlet__;



inline static void DoRotateL	(
			RB_NODE	*tree,
		RB_TREE_NODE	*node
		)
{
RB_TREE_NODE	*node0 = node->parent, *node1 = node, *node2 = node->left;

	/* rotate left child to right */
	node1->left = node2->right;
	node1->left->parent = node1;
	node2->right = node1;
	node2->right->parent = node2;

	if ( node0->left == node1 )
		{
		node0->left = node2;
		node0->left->parent = node0;
		return;
		}

	if ( node0->right == node1 )
		{
		node0->right = node2;
		node0->right->parent = node0;
		return;
		}

	tree->rootnode = node2;
	tree->rootnode->parent = &tree->nl_node;
}


inline static void DoRotateR	(
			RB_NODE	*tree,
		RB_TREE_NODE	*node
				)
{
RB_TREE_NODE	*node0 = node->parent, *node1 = node, *node2 = node->right;

	/* rotate right child to left */
	node1->right = node2->left;
	node1->right->parent = node1;
	node2->left = node1;
	node2->left->parent = node2;

	if ( node0->left == node1 )
		{
		node0->left = node2;
		node0->left->parent = node0;
		return;
		}

	if ( node0->right == node1 )
		{
		node0->right = node2;
		node0->right->parent = node0;
		return;
		}

	tree->rootnode = node2;
	tree->rootnode->parent = &tree->nl_node;
}


inline	static RB_TREE_NODE *DoGetUncle(
		RB_TREE_NODE	*node
			)
{
	/* get uncle of current node */
	if (node->parent == node->parent->parent->left )
		return node->parent->parent->right;

	return node->parent->parent->left;
}


inline static void DoInsertNode	(
			RB_NODE	*tree,
		RB_TREE_NODE	*root,
		RB_TREE_NODE	*node
				)
{
	/* normal binary search tree's insertion method */
	if(tree->keycmp(tree, &node->key, &root->key) < 0)
		{
		if ( root->left != &tree->nl_node )
			DoInsertNode(tree, root->left, node);
		else	{
			root->left = node;
			root->left->parent = root;
			}
		}
	else	{
		if ( root->right != &tree->nl_node )
			DoInsertNode(tree, root->right, node);
		else	{
			root->right = node;
			root->right->parent = root;
			}
		}
}

inline static void DoInsertMaintain(
			RB_NODE	*tree,
		RB_TREE_NODE	*node
				)
{
	// case 1: the root
	if ( node == tree->rootnode )
		{
		node->is_black = 1;
		return;
		}

	// case 2: parent is black
	if ( node->parent->is_black )
		return;

	// case 3: both parent and uncle are red
	if ( !node->parent->is_black &&  !DoGetUncle(node)->is_black )
		{
		node->parent->is_black = 1;
		DoGetUncle(node)->is_black = 1;
		node->parent->parent->is_black = 0;

		DoInsertMaintain(tree, node->parent->parent);
		return;
		}

	// case 4: parent is red and uncle is black
	if ( (node == node->parent->right) && (node->parent == node->parent->parent->left) )
		{
		DoRotateR(tree, node->parent);

		DoInsertMaintain(tree, node->left);
		return;
		}
	else if ( (node == node->parent->left) && (node->parent == node->parent->parent->right) )
		{
		DoRotateL(tree, node->parent);

		DoInsertMaintain(tree, node->right);
		return;
		}
	else if ( (node == node->parent->left) && (node->parent == node->parent->parent->left) )
		{
		node->parent->is_black = 1;
		node->parent->parent->is_black = 0;

		DoRotateL(tree, node->parent->parent);
		return;
		}
	else if ( (node == node->parent->right) && (node->parent == node->parent->parent->right) )
		{
		node->parent->is_black = 1;
		node->parent->parent->is_black = 0;

		DoRotateR(tree, node->parent->parent);
		return;
		}
}

inline static void DoDeleteMaintain(
			RB_NODE	*tree,
		RB_TREE_NODE	*node
				)
{
RB_TREE_NODE	*parent = node->parent, *brother;

	if ( node == parent->right )
		brother = parent->left;
	else	brother = parent->right;

	/* case 1: node is red */
	if ( !node->is_black )
		{
		node->is_black = 1;
		return;
		}

	/* case 2: node is the root */
	if ( node == tree->rootnode )
		{
		node->is_black = 1;
		return;
		}

	/* case 3: brother is red */
	if ( !brother->is_black)
		{
		if ( brother == parent->left )
			DoRotateL(tree, parent);
		else	DoRotateR(tree, parent);

		DoDeleteMaintain(tree, node);
		return;
		}

	/* case 4: brother is black */
	if ( brother->is_black )
		{
		/* case 4.1: brother's children are all black */
		if ( brother->left->is_black  && brother->right->is_black )
			{
			/* case 4.1.1: parent is red */
			if ( !parent->is_black )
				{
				if ( brother != &tree->nl_node )
					brother->is_black = 0;

				parent->is_black = 1;
				}
			else	{ /* case 4.1.2: parent is black */
				if ( brother != &tree->nl_node )
					brother->is_black = 0;

				DoDeleteMaintain(tree, parent);
				}

			return;
			}
		else	{/* case 4.2: brother has red children */
			if ( !brother->left->is_black )
				{
				brother->left->is_black = 1;

				if ( brother == parent->left )
					DoRotateL(tree, parent);
				else	{
					DoRotateL(tree, brother);
					DoRotateR(tree, parent);
					}

				DoDeleteMaintain(tree, parent->parent);
				return;
				}
			else	{
				brother->right->is_black = 1;

				if ( brother == parent->right )
					DoRotateR(tree, parent);
				else	{
					DoRotateR(tree, brother);
					DoRotateL(tree, parent);
					}

				DoDeleteMaintain(tree, parent->parent);
				return;
				}
			}
		}

	return;
}


inline static RB_TREE_NODE *DoFindDeletionScapegoat(
				RB_NODE	*tree,
			RB_TREE_NODE	*node
			)
{
RB_TREE_NODE	*scapegoatNode;

	/* find scapegoat */
	if ( node->left != &tree->nl_node )
		{
		scapegoatNode = node->left;

		while ( scapegoatNode->right != &tree->nl_node )
			scapegoatNode = scapegoatNode->right;

		if ( scapegoatNode != node )
			return scapegoatNode;
		}
	else if ( node->right != &tree->nl_node )
		{
		scapegoatNode = node->right;

		while ( scapegoatNode->left != &tree->nl_node )
			scapegoatNode = scapegoatNode->left;

		if ( scapegoatNode != node )
			return scapegoatNode;
		}

	return	node;
}




int	rb_tree_init	(
		RB_NODE *tree,
		int	keySize,
		int	(*keycmp)(RB_NODE *tree, const void *key1, const void *key2)
		)
{
	/* Initialize a new red-black tree structure */
	tree->magic = * (__starlet_ul__);
	tree->keysz = keySize;
	tree->keycmp = keycmp;
	tree->nr_node = 0;

	/* Initialize the null node */
	memset(&tree->nl_node, 0 , sizeof(RB_TREE_NODE));

	tree->nl_node.is_black = 1;
	tree->rootnode = &tree->nl_node;

	tree->nl_node.left = &tree->nl_node;
	tree->nl_node.right = &tree->nl_node;
	tree->nl_node.parent = &tree->nl_node;

	return	1;
}



int	rb_tree_insert	(
			RB_NODE	*tree,
		RB_TREE_NODE	*node
				)
{
	/* Initialize the new node */
	node->left = &tree->nl_node;
	node->right = &tree->nl_node;
	node->parent = &tree->nl_node;
	node->is_black = 0;

	if ( tree->rootnode == &tree->nl_node )
		{ /* regard the new node as the root if it's an empty tree */
		tree->rootnode = node;
		}
	else	{ /* insert the new node into the red-black tree */
		DoInsertNode(tree, tree->rootnode, node);
		}

	DoInsertMaintain(tree, node);

	tree->nr_node += 1;

	return	1;
}


void	rb_tree_remove	(
		RB_NODE		*tree,
		RB_TREE_NODE	*node
		)
{
RB_TREE_NODE	*scapegoatNode, *checkNode;

	/*
	** Copy the scapegoat's key and data into the deleting node
	** the scapegoat is the real node to be deleted
	*/
	scapegoatNode = DoFindDeletionScapegoat(tree, node);

	if ( node != scapegoatNode )
		{
		memcpy(&node->key, &scapegoatNode->key, tree->keysz);

		return	rb_tree_remove(tree, scapegoatNode);
		}

	/* Maintain red-black rules before deleting */
	DoDeleteMaintain(tree, scapegoatNode);

	/* Delete scapegoat */
	checkNode = scapegoatNode->parent;

	if ( checkNode != &tree->nl_node )
		{
		if ( checkNode->left == scapegoatNode )
			checkNode->left = &tree->nl_node;

		if ( checkNode->right == scapegoatNode )
			checkNode->right = &tree->nl_node;
		}
	else	tree->rootnode = &tree->nl_node;

	tree->nr_node -= 1;

	return;
}



int	rb_tree_search (RB_NODE *tree, void *pkey, RB_TREE_NODE **node)
{
RB_TREE_NODE	*currentNode = tree->rootnode;
int	status = * ((int *) pkey);

	/* Search the given key in the tree */
	while (currentNode != &tree->nl_node)
		{
		if ( (status = tree->keycmp(tree, pkey, &currentNode->key)) == 0)
			return	(*node =  currentNode, 1);

		currentNode = (status < 0) ? currentNode->left : currentNode->right;

		/*
		 * Fucking China stuff ...

		else if ( tree->keycmp(pkey, &currentNode->key) < 0)
			currentNode = currentNode->left;
		else if ( tree->keycmp(pkey, &currentNode->key) > 0 )
			currentNode = currentNode->right;
		*/

		}

	return	0;
}


RB_TREE_NODE	*rb_tree_node_head (RB_NODE* tree)
{
RB_TREE_NODE	*currentNode = tree->rootnode;

	if ( currentNode != &tree->nl_node )
		{
		/* Find the smallest node */
		while ( currentNode->left != &tree->nl_node )
			currentNode = currentNode->left;

		return currentNode;
		}

	return NULL;
}

RB_TREE_NODE	*rb_tree_node_tail	(
			RB_NODE* tree
				)
{
RB_TREE_NODE	*currentNode = tree->rootnode;

	if ( currentNode != &tree->nl_node )
		{
		/* find the greatest node */
		while ( currentNode->right != &tree->nl_node )
			currentNode = currentNode->right;

		return	currentNode;
		}

	return	NULL;
}

RB_TREE_NODE	*rb_tree_node_next	(
			RB_NODE	*tree,
		RB_TREE_NODE	*node
				)
{
RB_TREE_NODE	*currentNode;

	if (node->right != &tree->nl_node )
		{
		/* look for right child's smallest node */
		currentNode = node->right;

		while(currentNode->left != &tree->nl_node)
			currentNode = currentNode->left;

		return currentNode;
		}
	else if ( node->parent != &tree->nl_node )
		{
		currentNode = node;

		/* the node has no right child, look back */
		while ( currentNode != tree->rootnode )
			if ( currentNode->parent->right != currentNode )
				return currentNode->parent;
			else	currentNode = currentNode->parent;
		}

	return	NULL;
}

RB_TREE_NODE	*rb_tree_node_prev	(
			RB_NODE	*tree,
		RB_TREE_NODE	*node
				)
{
RB_TREE_NODE	*currentNode;

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
		while ( currentNode != tree->rootnode )
			if ( currentNode->parent->left != currentNode )
				return currentNode->parent;
			else currentNode = currentNode->parent;
		}

	return	NULL;
}
