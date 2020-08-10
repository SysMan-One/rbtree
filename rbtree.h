#ifndef	RBTREE_H_INCLUDED
#define	RBTREE_H_INCLUDED	1


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
**	in the yaDPI modules.
**
**  AUTHORS: Ruslan R. (The BadAss SysMan) Laishev
**
**  CREATION DATE:  27-FEB-2020
**
**  MODIFICATION HISTORY:
**
**
**--
*/


#ifdef __cplusplus
extern "C" {
#endif


#pragma	pack	(push, 2)

typedef struct __rb_tree_node__
{
		int	is_black;		/* A color of node	*/

						/* Links to paired nodes*/
	struct __rb_tree_node__ *left;
	struct __rb_tree_node__ *right;
	struct __rb_tree_node__ *parent;

	long long	key;

	union	{
	void		*pdata;			/* Pointer to data block*/
	long long	data;
	};


} RB_TREE_NODE;

typedef	struct __rb_tree__
{
	int	magic;		/* A marker for initialized tree */

	int	(*keycmp) (struct __rb_tree__ *tree, const void * key1, const void *key2);
	int	keysz;		/* Key size */
	int	nr_node;	/* A number of nodes in the tree */

	RB_TREE_NODE	*rootnode;
	RB_TREE_NODE	nl_node;
} RB_TREE;

#pragma	pack	(pop)

int		rb_tree_init (RB_TREE *tree, int keySize, int (*keycmp) (RB_TREE *tree, const void *key1, const void *key2));
void		rb_tree_destroy (RB_TREE *tree);
int		rb_tree_size (RB_TREE *tree);
int		rb_tree_insert (RB_TREE *tree, RB_TREE_NODE *node);
void		rb_tree_remove (RB_TREE *tree, RB_TREE_NODE *node_to_kill, RB_TREE_NODE **node_to_free);
int		rb_tree_search (RB_TREE *tree, void *key, RB_TREE_NODE **node);

RB_TREE_NODE	*rb_tree_node_head (RB_TREE *tree);
RB_TREE_NODE	*rb_tree_node_tail (RB_TREE *tree);
RB_TREE_NODE	*rb_tree_node_next (RB_TREE *tree, RB_TREE_NODE *node);
RB_TREE_NODE	*rb_tree_node_prev (RB_TREE *tree, RB_TREE_NODE *node);



#ifdef __cplusplus
	}
#endif

#endif
