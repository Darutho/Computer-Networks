#include "IDL2.h"
#include <stdio.h>

int *
add_6_svc(values *argp, struct svc_req *rqstp)
{
	static int  result;
    for(int i=0;i<argp->len;i++)
	result = argp->arr[i]+result;
	return &result;
}
