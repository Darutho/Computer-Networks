// /*
//  * This is sample code generated by rpcgen.
//  * These are only templates and you can use them
//  * as a guideline for developing your own functions.
//  */

// #include "date.h"

// long *
// bin_date_1_svc(void *argp, struct svc_req *rqstp)
// {
// 	static long  result;

// 	/*
// 	 * insert server code here
// 	 */

// 	return &result;
// }

// char **
// str_date_1_svc(long *argp, struct svc_req *rqstp)
// {
// 	static char * result;

// 	/*
// 	 * insert server code here
// 	 */

// 	return &result;
// }

/* dateproc.c - remote procedures; called by server stub. */
#include <rpc/rpc.h> /* standard RPC include file */
#include "date.h" /* this file is generated by rpcgen */
/* Return the binary date and time. */
long * bin_date_1(void *x,CLIENT *clnt)
{
 static long timeval; /* must be static */
 long time(); /* Unix system call */
 timeval = time((long *) 0); /* returns time in seconds since 00:00:00, January 1, 1970 */
 return(&timeval);
}
/* Convert a binary time and return a human readable string. */
//long *bintime;
char **str_date_1(long *bintime, CLIENT *clnt)
{
 static char *ptr; /* must be static */
 char *ctime(); /*c standard function */
 ptr = ctime(bintime); /* convert to local time */
 return(&ptr); /* return the address of pointer */
} 