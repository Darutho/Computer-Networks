#include "IDL2.h"
#include <stdio.h>


int
compute2_6(char *host,int a[],int len)
{
	CLIENT *clnt;
	int  *result_1;
	values  add_6_arg;


    for(int i=0;i<len;i++)
	add_6_arg.arr[i]=a[i];
	add_6_arg.len=len;

	clnt = clnt_create (host, ARRAY, ARRAY_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	result_1 = add_6(&add_6_arg, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	clnt_destroy (clnt);

	return (*result_1);	
}
int main (int argc, char *argv[])
{
	char *host;

	int a[100];
	int len;
    	printf("Enter the total length of array\n");
    	scanf("%d",&len);
    	for(int i=0;i<len;i++)
        scanf("%d",&a[i]);
	host = argv[1];
	printf("Answer= %d\n",compute2_6 (host,a,len));
        exit(0);

}


