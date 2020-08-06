#define	__MODULE__	"RBTEXMPL"
#define	__IDENT__	"X.00-03"
#define	__REV__		"0.03.0"

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
#include	<errno.h>

//#include	"rbtree.h"

/*
* Defines and includes for enable extend trace and logging
*/
#define		__FAC__	"RBTEXMPL"
#define		__TFAC__ __FAC__ ": "
#include	"utility_routines.h"

int	g_trace = 1;


#define	RBTREE$K_RED	1
#define	RBTREE$K_BLACK	(!RBTREE$K_RED)


//#define compLT(a,b) (a < b)
#define compEQ(a,b) (a == b)

/* Red-Black tree description */
typedef struct __rb_tree_node__ {
    struct __rb_tree_node__	*left,			/* left child */
			*right,				/* right child */
			*parent;			/* parent */
		int	color;				/* node color (BLACK, RED) */

	union	{
		u_int64_t data;				/* Key itslef or address of the key's area */
		void	*pdata;
		};

} RB_NODE;


typedef struct __rb_tree__
{
		RB_NODE	 *root,
			sentinel;

		int	node_nr;

	union	{
		u_int64_t	user;			/* An user data itself or address to user data area */
		void	*	puser;
		};

		int	(* keycmp) (struct __rb_tree__ *tree, void *key1, void *key2);
} RB_TREE;


static inline void	__rbtree_rotateLeft	(
				RB_TREE		*tree,
				RB_NODE		*x
					)
{
RB_NODE	*y = x->right;

	/* establish x->right link */
	if ( (x->right = y->left) != &tree->sentinel )
		y->left->parent = x;

	/* establish y->parent link */
	if (y != &tree->sentinel)
		y->parent = x->parent;

	if (x->parent)
		{
		if (x == x->parent->left)
			x->parent->left = y;
		else	x->parent->right = y;
		}
	else	tree->root = y;

	/* link x and y */
	if ( (y->left = x) != &tree->sentinel)
		x->parent = y;
}

static inline void	__rbtree_rotateRight	(
				RB_TREE		*tree,
				RB_NODE	*x
					)

{
RB_NODE	*y = x->left;

	/* establish x->left link */
	if ( (x->left = y->right) != &tree->sentinel)
		y->right->parent = x;

	/* establish y->parent link */
	if (y != &tree->sentinel)
		y->parent = x->parent;

	if (x->parent)
		{
		if (x == x->parent->right)
			x->parent->right = y;
		else	x->parent->left = y;
		}
	else	tree->root = y;

	/* link x and y */
	if ( (y->right = x) != &tree->sentinel)
		x->parent = y;
}



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
			u_int64_t	*key,
			RB_NODE	*	node_to_insert,
			RB_NODE	**	node_is_found
				)
{
RB_NODE *current, *parent;
int	res;

	/* find where node belongs */
	current = tree->root;
	parent = 0;

	while ( current != &tree->sentinel )
		{
		if ( !(res = tree->keycmp(tree, key, current->data)) )
			return	*node_is_found = current, 1;	/* Return existen node, SS$_NORMAL */

		parent = current;
		current = (0 > res) ? current->left : current->right;
		}

	node_to_insert->parent = parent;
	node_to_insert->left = &tree->sentinel;
	node_to_insert->right = &tree->sentinel;
	node_to_insert->color = RBTREE$K_RED;

	/* insert node in tree */
	if ( parent )
		{
		if ( 0 > tree->keycmp(tree, key, parent->data) )
			parent->left = node_to_insert;
		else	parent->right = node_to_insert;
		}
	else	tree->root = node_to_insert;

	__rbtree_insertFixup(tree, node_to_insert);

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

int	lib$rbtree_delete	(
				RB_TREE	*	tree,
				RB_NODE *	node_to_kill,
				RB_NODE **	node_to_free
				)
{
RB_NODE	*x = NULL, *y = NULL;


	*node_to_free = NULL;

	if ( !node_to_kill || (node_to_kill == &tree->sentinel) )
		return	0;	/* RNF */


	if ( (node_to_kill->left == &tree->sentinel) || (node_to_kill->right == &tree->sentinel) )
		{
		/* y has a NIL node as a child */
		y = node_to_kill;
		}
	else	{
		/* find tree successor with a NIL node as a child */
		y = node_to_kill->right;

		while (y->left != &tree->sentinel)
			y = y->left;
		}

	/* x is y's only child */
	if ( y->left != &tree->sentinel )
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
}


int	lib$rbtree_lookup	(
			RB_TREE	*	tree,
			u_int64_t	*key,
			RB_NODE	**	retnode
				)
{
RB_NODE *current = tree->root;
int	res;

	while ( current != &tree->sentinel )
		{
		if ( !(res = tree->keycmp(tree, key, current->data)) )
			return	*retnode =  current, 1;	/* Return found node, SS$_NORMAL */

		current = 0 > res ? current->left : current->right;
		}

	return	0;
}


int	lib$rbtree_init		(
			RB_TREE		*tree,
			void		*keycmp
				)
{
	tree->sentinel.left = tree->sentinel.right = tree->root = &tree->sentinel;
	tree->sentinel.parent = 0;
	tree->sentinel.data = 0;
	tree->keycmp = keycmp;
	tree->sentinel.color = RBTREE$K_BLACK;

	return	1;	/* SS$_NORMAL */
}






struct my_data	{
	u_int64_t	key;		/* Key field */

	int	n;			/* Payload part of the data record */
	ASC	sts;
	int	n2;
};



static	int	my_keycmp	(
			RB_TREE		*tree,
			u_int64_t	*key1,
			struct my_data	*data2
			)
{
u_int64_t	d1 = *key1;

	return	*key1 - data2->key;
}


void	main	(int argc, char **argv)
{
u_int64_t	key;
int		ct, status;

RB_NODE		*newnode, *oldnode, *retnode;
RB_TREE		my_tree = {0};
struct my_data *my_pdata;

	status = lib$rbtree_init(&my_tree, my_keycmp);


	for (ct = 135; ct; ct--)
		{
		key = rand() % 9 + 1;

		$IFTRACE(g_trace, "key=%09d", key);

		if ( 1 & lib$rbtree_lookup(&my_tree, &key, &oldnode) )
			{
			my_pdata = oldnode->data;

			$IFTRACE(g_trace, "--------	key=%09d, pdata%p, data={%d, %.*s, %d}", key, oldnode->pdata, my_pdata->n, $ASC(&my_pdata->sts), my_pdata->n2);

			lib$rbtree_delete(&my_tree, oldnode, &retnode);

			my_pdata = retnode->data;
			$IFTRACE(g_trace, "--------	key=%09d, pdata%p, data={%d, %.*s, %d}", key, retnode->pdata, my_pdata->n, $ASC(&my_pdata->sts), my_pdata->n2);

			free(retnode->data);
			free(retnode);
			}
		else	{
			newnode = malloc(sizeof(RB_NODE));
			newnode->pdata = my_pdata = malloc(sizeof(struct my_data));

			my_pdata->key = key;
			my_pdata->n = ct;
			my_pdata->n2 = ct;

			$ASCLEN(&my_pdata->sts) = (unsigned char) snprintf($ASCPTR(&my_pdata->sts), ASC$K_SZ, "Key/Data = %09d/%09d", key, ct);

			$IFTRACE(g_trace, "+++++++++	key=%09d, pdata%p, data={%d, %.*s, %d}", key, newnode->pdata, my_pdata->n, $ASC(&my_pdata->sts), my_pdata->n2);

			if ( !(1 & (status = lib$rbtree_insert(&my_tree, &key, newnode, &retnode))) )
				{
				$LOG(STS$K_ERROR, "key=%09d, pdata%p, data={%d, %.*s, %d}", key, newnode->pdata, my_pdata->n, $ASC(&my_pdata->sts), my_pdata->n2);
				$LOG(STS$K_ERROR, "key=%09d, pdata%p, data={%d, %.*s, %d}", key, newnode->pdata, my_pdata->n, $ASC(&my_pdata->sts), my_pdata->n2);
				}
			}
		}
}
