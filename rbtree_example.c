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

//#include	"rbtree.h"

/*
* Defines and includes for enable extend trace and logging
*/
#define		__FAC__	"RBTEXMPL"
#define		__TFAC__ __FAC__ ": "
#include	"utility_routines.h"

int	g_trace = 1;


#if	0
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

#endif


#define	RBTREE$K_RED	1
#define	RBTREE$K_BLACK	(!RBTREE$K_RED)
#define	RBTREE$K_LLINK	0
#define	RBTREE$K_RLINK	1

#define	RBTREE$K_BIN	0	/* An octet sequence */
#define	RBTREE$K_WORD	1	/* 16 bits */
#define	RBTREE$K_LONG	2	/* 32 bits */
#define	RBTREE$K_QUAD	3	/* 64 bits */
#define	RBTREE$K_OCTA	4	/* 128 bits */

#define	RBTREE$K_ASC	0x10	/* Ascending sort order */
#define	RBTREE$K_DESC	0x20	/* Descending sort order */
#define	RBTREE$K_NOCASE	0x40	/* Descending sort order */

typedef	struct __rb_node__
{
	int	color;
	struct __rb_node__ *link[2];

	union	{
		int	data;
		void	*ptr;
		};

} RB_NODE;

typedef struct __rb_tree__
{
	struct __rb_node__ *root;
	int	node_nr;


	int	keytype;
	void	*kyeoff;			/* Offfset to key in the "data" buffer */
	int	keysz;				/* A size of the key in the node */

	int	(* keycmp) (struct __rb_tree *tree, struct __rb_tree__ *node1, struct __rb_tree__ *node2);
	int	(* alloc_node) (struct __rb_tree__ **node);
	int	(* free_node)  (struct __rb_tree__ *node);
} RB_TREE;


static	inline int	is_red	(
			RB_NODE *node
				)
{
	return ((node != NULL) && (node->color == RBTREE$K_RED));
}

static inline RB_NODE	*rb_single (
			RB_NODE  *root,
			int	dir
				)
{
RB_NODE *save = root->link[!dir];

	root->link[!dir] = save->link[dir];
	save->link[dir] = root;

	root->color = RBTREE$K_RED;
	save->color = RBTREE$K_BLACK;

	return	save;
}

static inline RB_NODE	*rb_double (
			RB_NODE	 *root,
			int	dir
				)
{
	root->link[!dir] = rb_single ( root->link[!dir], !dir );

	return rb_single ( root, dir );
}


static inline	RB_NODE  *rbtree_mknode (
			int data
				)
{
RB_NODE *rn = malloc ( sizeof  (RB_NODE) );

	if ( !rn )
		return rn;

	rn->data = data;
	rn->color = RBTREE$K_RED;
	rn->link[RBTREE$K_LLINK] = rn->link[RBTREE$K_RLINK] = NULL;

	return	rn;
}


static int	rb_insert (
			RB_TREE	*tree,
			int	data
			)
{
RB_NODE	head = {0},	/* временный корень дерева*/
	*g, *t,		/* дедушка и родитель */
	*p, *q;		/* родитель и итератор */

int	dir = RBTREE$K_LLINK, last;


	/* если добавляемый элемент оказывается первым – то ничего делать не нужно*/
	if ( !tree->root == NULL )
		{
		if ( !tree->root = rbtree_mknode ( data ) )
			return	0;

		/* сделать корень дерева черным */
		tree->root->color = RBTREE$K_BLACK;
		return 1;
		}

	/* вспомогательные переменные */
	t = &head;
	g = p = NULL;
	q = t->link[1] = tree->root;

	/* основной цикл прохода по дереву */
	for ( ; ; )
		{
		if ( q == NULL )
			{
			/* вставка ноды */
			p->link[dir] = q = rbtree_mknode ( data );
			tree->node_nr++ ;

			if ( q == NULL )
				return 0;
			}
		else if ( is_red ( q->link[0] ) && is_red ( q->link[1] ) )
			{
			/* смена цвета */
			q->color = RBTREE$K_RED;
			q->link[RBTREE$K_LLINK]->color = q->link[RBTREE$K_RLINK]->color = !RBTREE$K_RED;
			}

		/* совпадение 2-х красных цветов */
		if ( is_red ( q ) && is_red ( p ) )
			{
			int dir2 = t->link[RBTREE$K_RLINK] == g;

			if ( q == p->link[last] )
				t->link[dir2] = rb_single ( g, !last );
			else	t->link[dir2] = rb_double ( g, !last );
			}

		/* такой узел в дереве уже есть - выход из функции*/
		if ( q->data == data )
			break;

		last = dir;
		dir = q->data < data;

		if ( g != NULL )
			t = g;

		g = p, p = q;
		q = q->link[dir];
		}


	tree->root = head.link[RBTREE$K_RLINK];	/* обновить указатель на корень дерева*/
	tree->root->color = RBTREE$K_BLACK;	/* сделать корень дерева черным */

	return 1;
}




static	int rb_remove (
		RB_TREE		*tree,
			int	data
			)
{
RB_NODE	head = {0},	/* временный указатель на корень дерева */
	*q, *p, *g,	/* вспомогательные переменные */
	*f = NULL;	/* узел, подлежащий удалению */
int	dir = RBTREE$K_RLINK;

	if ( !tree->root )
		return	1;

	/* инициализация вспомогательных переменных */
	q = &head;
	g = p = NULL;
	q->link[RBTREE$K_RLINK] = tree->root;

	/* поиск и удаление */
	while ( q->link[dir] )
		{
		int last = dir;

		/* сохранение иерархии узлов во временные переменные */
		g = p, p = q;
		q = q->link[dir];
		dir = q->data < data;

		/* сохранение найденного узла */
		if ( q->data == data )
			f = q;

		if ( !is_red ( q ) && !is_red ( q->link[dir] ) )
			{
			if ( is_red ( q->link[!dir] ) )
				p = p->link[last] = rb_single ( q, dir );
			else if ( !is_red ( q->link[!dir] ) )
				{
				RB_NODE *s = p->link[!last];

				if ( s != NULL )
					{
					if ( !is_red ( s->link[!last] ) && !is_red ( s->link[last] ) )
						{
						/* смена цвета узлов */
						p->color = RBTREE$K_BLACK;
						s->color = RBTREE$K_RED;
						q->color = RBTREE$K_RED;
						}
					else	{
						int dir2 = g->link[RBTREE$K_RLINK] == p;

						if ( is_red ( s->link[last] ) )
							g->link[dir2] = rb_double ( p, last );
						else if ( is_red ( s->link[!last] ) )
							g->link[dir2] = rb_single ( p, last );

						/* корректировка цвета узлов */
						q->color = g->link[dir2]->color = RBTREE$K_RED;
						g->link[dir2]->link[RBTREE$K_LLINK]->color = RBTREE$K_BLACK;
						g->link[dir2]->link[RBTREE$K_RLINK]->color = RBTREE$K_BLACK;
						}
					}
				}
			}
		}

	/* удаление найденного узла */
	if ( f )
		{
		f->data = q->data;
		p->link[ (p->link[RBTREE$K_RLINK] == q) ] = q->link[ (q->link[RBTREE$K_LLINK] == NULL) ] ;

		free ( q );
		}

	/* обновление указателя на корень дерева */
	if ( tree->root = head.link[RBTREE$K_RLINK] )
		tree->root->color = RBTREE$K_BLACK;

	return 1;
}




RB_TREE	 my_tree = {0};


int main()
{
int	count = 10000000, res = 0, i = 0, rnd = 0;
time_t start,time2;
volatile long unsigned t;

	start = time(NULL);
	srand (time (NULL));

	for( i = 0; i < count; i++)
		{
		rnd = (rand() % count);


		res = rb_insert ( &my_tree, rnd );

		if ( my_tree.node_nr == 5000000 )
			break;
		}

	time2 = time(NULL);

	$IFTRACE(g_trace, "Elapsed %f seconds", difftime(time2, start));
	$IFTRACE(g_trace, "A number of node in the tree=%d\n", my_tree.node_nr);

	return 0 ;
}
