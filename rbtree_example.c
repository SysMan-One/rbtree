#define	__MODULE__	"RBTEXMPL"
#define	__IDENT__	"X.00-01"
#define	__REV__		"0.01.0"

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
#include	<pthread.h>

#include	"rbtree.h"

/*
* Defines and includes for enable extend trace and logging
*/
#define		__FAC__	"RBTEXMPL"
#define		__TFAC__ __FAC__ ": "
#include	"utility_routines.h"

ASC	q_logfspec = {0},
	q_confspec = {0};



int	g_exit_flag = 0,		/* Global flag 'all must to be stop'	*/
	g_trace = 1,			/* A flag to produce extensible logging	*/
	g_logsize = 0;


OPTS optstbl [] =
{
	{$ASCINI("config"),	&q_confspec, ASC$K_SZ,	OPTS$K_CONF},

	{$ASCINI("trace"),	&g_trace, 0,		OPTS$K_OPT},
	{$ASCINI("logfile"),	&q_logfspec, ASC$K_SZ,	OPTS$K_STR},
	{$ASCINI("logsize"),	&g_logsize, 0,		OPTS$K_INT},

	OPTS_NULL
};


pthread_rwlock_t	tree_rwlock = PTHREAD_RWLOCK_INITIALIZER;
RB_TREE	tree = {0};

static int	cmpkey	(
		void	*x,
		void	*y
			)

{
int	ix, iy;

	ix = (int) x;
	iy = (int) y;

	return	(ix - iy);

}

typedef	struct __flow__ {
	union	{
		RB_TREE_NODE	rb_node;
		ENTRY		ent;
		};

	ASC	val;

} FLOW;

#define	NODECOUNT	13

int	main	(int argc, char* argv[])
{
int	status, i;
pthread_t	tid;
FLOW	fe[NODECOUNT] = {0}, *pf;
RB_TREE_NODE	*pnode;

	$LOG(STS$K_INFO, "Rev: " __IDENT__ "/"  __ARCH__NAME__   ", (built  at "__DATE__ " " __TIME__ " with CC " __VERSION__ ")");

	/*
	 * Process command line arguments
	 */
	__util$getparams(argc, argv, optstbl);

	if ( $ASCLEN(&q_logfspec) )
		{
		__util$deflog($ASCPTR(&q_logfspec), NULL);

		$LOG(STS$K_INFO, "Rev: " __IDENT__ "/"  __ARCH__NAME__   ", (built  at "__DATE__ " " __TIME__ " with CC " __VERSION__ ")");
		}

	if ( g_trace )
		__util$showparams(optstbl);

	status = rb_tree_init(&tree, sizeof(int), cmpkey);

	for (i = 0; i < NODECOUNT; i++)
		{
		pf = &fe[i];
		pf->rb_node.key = NODECOUNT - i;
		$ASCLEN(&pf->val) = (unsigned char) snprintf($ASCPTR(&pf->val), ASC$K_SZ - 1, "Record #%d", i);

		status = rb_tree_insert(&tree, &pf->rb_node);

		$IFTRACE(g_trace, "Inserted node[key=%d, val=%.*s]", pf->rb_node.key, $ASC(&pf->val));
		}

	for (pnode = rb_tree_node_head(&tree); pnode; pnode = rb_tree_node_next(&tree, pnode), i)
		{
		pf = (FLOW *) pnode;
		$IFTRACE(g_trace, "Retrieved node[key=%d, val=%.*s]", pnode->key, $ASC(&pf->val));
		}



	rb_tree_search(&tree, (void *)5, &pnode);
	rb_tree_remove(&tree, pnode);

	for (pnode = rb_tree_node_head(&tree); pnode; pnode = rb_tree_node_next(&tree, pnode), i)
		{
		pf = (FLOW *) pnode;
		$IFTRACE(g_trace, "Retrieved node[key=%d, val=%.*s]", pnode->key, $ASC(&pf->val));
		}


	return	0;
}




