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
#include	<errno.h>

//#include	"rbtree.h"
#include	"lib_rbtree.h"

/*
* Defines and includes for enable extend trace and logging
*/
#define		__FAC__	"RBTEXMPL"
#define		__TFAC__ __FAC__ ": "
#include	"utility_routines.h"

int	g_trace = 1;




struct my_data	{
	u_int64_t	key;		/* Key field */

	int	n;			/* Payload part of the data record */
	ASC	sts;
	int	n2;
};



static	int	my_keycmp	(
			RB_TREE		*tree,
			void		*key1,
			struct my_data	*data2
			)
{
	return	* ((u_int64_t *)key1) - data2->key;
}


int	main	(int argc, char **argv)
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
			my_pdata = oldnode->pdata;

			$IFTRACE(g_trace, "--------	key=%09d, pdata%p, data={%d, %.*s, %d}", key, oldnode->pdata, my_pdata->n, $ASC(&my_pdata->sts), my_pdata->n2);

			lib$rbtree_delete(&my_tree, oldnode, &retnode);

			my_pdata = retnode->pdata;
			$IFTRACE(g_trace, "--------	key=%09d, pdata%p, data={%d, %.*s, %d}", key, retnode->pdata, my_pdata->n, $ASC(&my_pdata->sts), my_pdata->n2);

			free(retnode->pdata);
			free(retnode);
			}
		else	{
			newnode = malloc(sizeof(RB_NODE));
			newnode->pdata = my_pdata = malloc(sizeof(struct my_data));

			my_pdata->key = key;
			my_pdata->n = ct;
			my_pdata->n2 = ct;

			$ASCLEN(&my_pdata->sts) = (unsigned char) snprintf($ASCPTR(&my_pdata->sts), ASC$K_SZ, "Key/Data = %05lu/%05d", key, ct);

			$IFTRACE(g_trace, "+++++++++	key=%09d, pdata%p, data={%d, %.*s, %d}", key, newnode->pdata, my_pdata->n, $ASC(&my_pdata->sts), my_pdata->n2);

			if ( !(1 & (status = lib$rbtree_insert(&my_tree, &key, newnode, &retnode))) )
				{
				$LOG(STS$K_ERROR, "key=%09d, pdata%p, data={%d, %.*s, %d}", key, newnode->pdata, my_pdata->n, $ASC(&my_pdata->sts), my_pdata->n2);
				$LOG(STS$K_ERROR, "key=%09d, pdata%p, data={%d, %.*s, %d}", key, newnode->pdata, my_pdata->n, $ASC(&my_pdata->sts), my_pdata->n2);
				}
			}

		}



	for (retnode = lib$rbtree_tail(&my_tree); retnode; )
		{
		my_pdata = retnode->pdata;
		$IFTRACE(g_trace, "--------	key=%09d, pdata%p, data={%d, %.*s, %d}", my_pdata->key, retnode->pdata, my_pdata->n, $ASC(&my_pdata->sts), my_pdata->n2);

		oldnode = retnode;
		retnode = lib$rbtree_prev(&my_tree, retnode);

		lib$rbtree_delete(&my_tree, oldnode, &oldnode);
		}


	for (retnode = lib$rbtree_head(&my_tree); retnode; )
		{
		my_pdata = retnode->pdata;
		$IFTRACE(g_trace, "--------	key=%09d, pdata%p, data={%d, %.*s, %d}", my_pdata->key, retnode->pdata, my_pdata->n, $ASC(&my_pdata->sts), my_pdata->n2);

		oldnode = retnode;
		retnode = lib$rbtree_next(&my_tree, retnode);

		lib$rbtree_delete(&my_tree, oldnode, &oldnode);
		}




}
