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

/*
The options for the XAB$B_DTP field are listed in the following table:
	Keyword 	Data Type		Sort Order
	XAB$C_BN2 	Unsigned 2-byte binary 	Ascending
	XAB$C_DBN2 	Unsigned 2-byte binary 	Descending
	XAB$C_BN4 	Unsigned 4-byte binary 	Ascending
	XAB$C_DBN4 	Unsigned 4-byte binary 	Descending
	XAB$C_BN8 	Unsigned 8-byte binary 	Ascending
	XAB$C_DBN8 	Unsigned 8-byte binary 	Descending

	XAB$C_IN2 	Signed 2-byte integer 	Ascending
	XAB$C_DIN2 	Signed 2-byte integer 	Descending
	XAB$C_IN4 	Signed 4-byte integer 	Ascending
	XAB$C_DIN4 	Signed 4-byte integer 	Descending
	XAB$C_IN8 	Signed 8-byte integer 	Ascending
	XAB$C_DIN8 	Signed 8-byte integer 	Descending

	XAB$C_COL 	Collating key		Ascending
	XAB$C_DCOL 	Collating key		Descending
	XAB$C_PAC 	Packed decimal string 	Ascending
	XAB$C_DPAC 	Packed decimal string 	Descending

	XAB$C_STG 1 	Left-justified string of unsigned 8-bit bytes 	Ascending
	XAB$C_DSTG 	Left-justified string of unsigned 8-bit bytes 	Descending
*/
enum	{
	KDTP$K_STG	= 0,
	KDTP$K_BN2,
	KDTP$K_BN4,
	KDTP$K_BN8,
	KDTP$K_IN2,
	KDTP$K_IN4,
	KDTP$K_IN8

};

#define	RBTREE$K_ASC	0x100	/* Ascending sort order */
#define	RBTREE$K_DESC	0x200	/* Descending sort order */
#define	RBTREE$K_NOCASE	0x400	/* Descending sort order */


#define	RBTREE$K_STG	KDTP$K_STG
#define	RBTREE$K_DSTG	(KDTP$K_STG  | RBTREE$K_DESC)

#define	RBTREE$K_BN2	KDTP$K_BN2
#define	RBTREE$K_DBN2	(KDTP$K_BN2  | RBTREE$K_DESC)
#define	RBTREE$K_BN4	KDTP$K_BN4
#define	RBTREE$K_DBN4	(KDTP$K_DBN4 | RBTREE$K_DESC)
#define	RBTREE$K_BN8	KDTP$K_BN8
#define	RBTREE$K_DBN8	(KDTP$K_BN8  | RBTREE$K_DESC)

#define	RBTREE$K_IN2	KDTP$K_IN2
#define	RBTREE$K_DIN2	(KDTP$K_IN2  | RBTREE$K_DESC)
#define	RBTREE$K_IN4	KDTP$K_IN4
#define	RBTREE$K_DIN4	(KDTP$K_IN4 | RBTREE$K_DESC)
#define	RBTREE$K_IN8	KDTP$K_IN8
#define	RBTREE$K_DIN8	(KDTP$K_IN8  | RBTREE$K_DESC)


#define	RBTREE$M_KEYDTP	0x0ff



typedef	struct __rb_node__
{
	int	color;
	struct __rb_node__ *link[2];

	u_int64_t	key;

	union	{
		int	data;
		void	*ptr;
	unsigned char	dblock[0];		/* Placeholder for data block as a continuation of the RB_NODE */
			};

} RB_NODE;

typedef struct __rb_tree__
{
	struct __rb_node__ *root;
	int	node_nr;


	int	keydtp;				/* Key data type see constats RBTREE$K_STG */
	int	keyoff;				/* Offfset to key in the "data" buffer */
	int	keysz;				/* A size of the key in the node */

	int	(* keycmp) (struct __rb_tree *tree, void *key1, void *key2);
} RB_TREE;


static int	__rbtree_cmpkey		(
				RB_TREE		*tree,
				void 		*key1,
				void		*key2
					)
{
int	res;

		switch ( tree->keydtp & RBTREE$M_KEYDTP )
			{
			case	RBTREE$K_BN2:
				res = *((unsigned  short *) key1) - *((unsigned short *) key2);
				break;

			case	RBTREE$K_BN4:
				res = *((unsigned int *) key1) - *((unsigned int *) key2);
				break;

			case	RBTREE$K_BN8:
				res = *((u_int64_t *) key1) - *((u_int64_t *) key2);
				break;

			case	RBTREE$K_IN2:
				res = *((short *) key1) - *((short *) key2);
				break;

			case	RBTREE$K_IN4:
				res = *((int *) key1) - *((int *) key2);
				break;

			case	RBTREE$K_IN8:
				res = *((int64_t *) key1) - *((int64_t *) key2);
				break;

			case	RBTREE$K_STG:
				res = memcmp(key1, key2, tree->keysz);
				break;
			}


	return	res;
}




static	inline int	__rb_is_red	(
			RB_NODE *node
				)
{
	return ((node != NULL) && (node->color == RBTREE$K_RED));
}

static inline RB_NODE	*__rb_single (
			RB_NODE  *node,
			int	dir
				)
{
RB_NODE *save = node->link[!dir];

	node->link[!dir] = save->link[dir];
	save->link[dir] = node;

	node->color = RBTREE$K_RED;
	save->color = RBTREE$K_BLACK;

	return	save;
}

static inline RB_NODE	*__rb_double (
			RB_NODE	 *node,
			int	dir
				)
{
	node->link[!dir] = __rb_single ( node->link[!dir], !dir );

	return __rb_single ( node, dir );
}


static int	__util$rbtree_insert (
				RB_TREE	*tree,
				RB_NODE	*newnode
					)
{
RB_NODE head = {0},	/* temporary root of tree */
	*g, *t,		/* grandftaher and parent */
	*p, *q;		/* parent and iterator */
int	dir = RBTREE$K_LLINK, last = 0xDEADBEEF, i;

	/* If it's first element in the tree - just add new node and exit */
	if ( !tree->root )
		{
		tree->root = newnode;			/* Set newnode as tree's root */
		tree->node_nr++;			/* Adjust node counter */
		tree->root->color = RBTREE$K_BLACK;	/* Root  is BLACK */

		return	1;				/* Return SS$_NORMAL */
		}

	/* Initialization of work variables .... */
	t = &head;
	g = p = NULL;
	q = t->link[RBTREE$K_RLINK] = tree->root;


	/* Main walking on the tree loop */
	for (i = 0; ; i++)
		{
		if ( !q )
			{

			p->link[dir] = q = newnode;	/* Insert new node */
			tree->node_nr++ ;		/* Adjust node counter */
			}
		else if ( __rb_is_red ( q->link[RBTREE$K_LLINK] ) && __rb_is_red ( q->link[RBTREE$K_RLINK] ) )
			{
			/* Change colors ... */
			q->color = RBTREE$K_RED;
			q->link[RBTREE$K_LLINK]->color = RBTREE$K_BLACK;
			q->link[RBTREE$K_RLINK]->color = RBTREE$K_BLACK;
			}

		/* совпадение 2-х красных цветов */
		if ( __rb_is_red ( q ) && __rb_is_red ( p ) )
			{
			int	dir2 = (t->link[RBTREE$K_RLINK] == g);

			t->link[dir2] = (q == p->link[last]) ? __rb_single ( g, !last ) : __rb_double ( g, !last );
			}

		last = dir;

		/* So node is exist in the tree already - break from loop ! */
		if ( !(dir = tree->keycmp(tree, &q->key, &newnode->key)) )
			break;

		/* dir = q - newnode
		 * q->data < data       true             false
		 */
		dir = (dir < 0)       ? RBTREE$K_RLINK : RBTREE$K_LLINK;

		t = g ? g : t;

		g = p, p = q;
		q = q->link[dir];
		}

	tree->root = head.link[RBTREE$K_RLINK];		/* Update tree's root */
	tree->root->color = RBTREE$K_BLACK;		/* Make root is BLACK */

	return 1;
}



static	int __util$rbtree_remove (
		RB_TREE		*tree,
		void		*key,
		RB_NODE		**oldnode
			)
{
RB_NODE	head = {0},	/* Temporary root */
	*q, *p, *g,	/* some stuff  */
	*f = NULL;	/* node to kill */
int	dir = RBTREE$K_RLINK, last;

	/* If tree is sempty - nothing to do */
	if ( !tree->root )
		return	0;

	/* Initialize of works variables */
	q = &head;
	g = p = NULL;
	q->link[RBTREE$K_RLINK] = tree->root;

	/* Main looop to search and kill ... */
	while ( q->link[dir] )
		{
		last = dir;

		/* сохранение иерархии узлов во временные переменные */
		g = p, p = q;
		q = q->link[dir];


		/* dir = q->data < data; */

		if ( !(dir = tree->keycmp(tree, q->dblock + tree->keyoff, key)) )
			f = q;	/* Found the node, save this into the work variable */
		else	dir = (dir < 0)       ? RBTREE$K_RLINK : RBTREE$K_LLINK;


		if ( !__rb_is_red ( q ) && !__rb_is_red ( q->link[dir] ) )
			{
			if ( __rb_is_red ( q->link[!dir] ) )
				p = p->link[last] = __rb_single ( q, dir );
			else if ( !__rb_is_red ( q->link[!dir] ) )
				{
				RB_NODE *s = p->link[!last];

				if ( s != NULL )
					{
					if ( !__rb_is_red ( s->link[!last] ) && !__rb_is_red ( s->link[last] ) )
						{
						/* смена цвета узлов */
						p->color = RBTREE$K_BLACK;
						s->color = RBTREE$K_RED;
						q->color = RBTREE$K_RED;
						}
					else	{
						int dir2 = g->link[RBTREE$K_RLINK] == p;

						if ( __rb_is_red ( s->link[last] ) )
							g->link[dir2] = __rb_double ( p, last );
						else if ( __rb_is_red ( s->link[!last] ) )
							g->link[dir2] = __rb_single ( p, last );

						/* корректировка цвета узлов */
						q->color = g->link[dir2]->color = RBTREE$K_RED;
						g->link[dir2]->link[RBTREE$K_LLINK]->color = RBTREE$K_BLACK;
						g->link[dir2]->link[RBTREE$K_RLINK]->color = RBTREE$K_BLACK;
						}
					}
				}
			}
		}

	/* Found ? Kill node ! */
	if ( f )
		{
		f->data = q->data;
		p->link[ (p->link[RBTREE$K_RLINK] == q) ] = q->link[ (q->link[RBTREE$K_LLINK] == NULL) ] ;

		*oldnode = q;
		}

	/* обновление указателя на корень дерева */
	if ( tree->root = head.link[RBTREE$K_RLINK] )
		tree->root->color = RBTREE$K_BLACK;

	return 1;
}





/* A record to keep application specific data */
typedef	struct __my_record__	{
	int	key;		/* A primary key of the record */
	ASC	sts;		/* "Payload" of the data record*/
} MY_RECORD;


RB_TREE	 my_tree = {0};



int	main	()
{
int	count = 1000000, res = 0, i = 0, rnd = 0, sz;
time_t start,time2;
volatile long unsigned t;
RB_NODE	*pn;
MY_RECORD *pr;

	/* Define RB Tree */
	my_tree.keyoff = offsetof (MY_RECORD, key);		/* An offset to key field in the record */

	my_tree.keydtp = RBTREE$K_IN4;				/* Signed integer 4-byte integer */
	my_tree.keysz = sizeof(int);				/* Keys size if the size of the FLOW_RECORD.key */
	my_tree.keycmp = __rbtree_cmpkey;			/* We will use a routine from the RB Tree API */

	start = time(NULL);
	srand (time (NULL));

	for( i = 0; i < count; i++)
		{
		/* Allocate memory for new RB TREE Entry */
		if ( !(pn = malloc (sz = sizeof(RB_TREE))) )
			{
			$LOG(STS$K_ERROR, "No memory (%d) octets for new RB Node/Data record, errno=%d", sz, errno);
			break;
			}

		/* Initialize RB Node/Data Entry with my data */
		pn->color = RBTREE$K_RED;
		pn->link[RBTREE$K_LLINK] = pn->link[RBTREE$K_RLINK] = NULL;


		/* Allocate memory for new Data Record */
		if ( !(pr = malloc (sizeof(MY_RECORD))) )
			{
			$LOG(STS$K_ERROR, "No memory (%d) octets for new RB Node/Data record, errno=%d", sz, errno);
			break;
			}

		/*
		** Initalize My Record area
		** (My record is a continuation of the RB NODE area )
		*/
		pn->ptr = pr;
		pr->key = (rand() % count);
		$ASCLEN(&pr->sts) = (unsigned char) snprintf($ASCPTR(&pr->sts), ASC$K_SZ, "Record #%09d, key=%08x", i, pr->key);

		/* Insert new node ! */
		//$IFTRACE(g_trace, "Inserting : %.*s ...", $ASC(&pr->sts));
		res = __util$rbtree_insert ( &my_tree, pn );


		if ( my_tree.node_nr == 500000 )
			break;
		}

	time2 = time(NULL);

	$IFTRACE(g_trace, "Elapsed %f seconds", difftime(time2, start));
	$IFTRACE(g_trace, "A number of node in the tree=%d\n", my_tree.node_nr);

	return 0 ;
}
