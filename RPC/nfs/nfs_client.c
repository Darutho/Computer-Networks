#include "nfs.h"
#include <stdio.h>

int w;

int compute_6(char *host,char b[])
{
CLIENT *clnt;
int  *result_1;
values  add_6_arg;
int i;
for(i=0;i<strlen(b);i++)
	add_6_arg.fname[i]=b[i];
//	add_6_arg.fname=b;
//for(i=strlen(b);i<20;i++)
	add_6_arg.fname[strlen(b)]='\0';
//	printf("%s",add_6_arg.fname);
clnt = clnt_create (host, COMPUTE, COMPUTE_VERS, "udp");
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

char b[10]; //int w;
	//memset(b,'\0',sizeof(b));
	//printf("Enter the length of filename\n");
	//scanf("%d",&w); 
	//printf("%d",w);
//	w=4;
    printf("Enter filename\n");
	gets(b);
	int i;	
/*	for(i=0;i<(int)w;i++)
    scanf("%c",&b[i]);*/

host = argv[1];
printf("Answer= %d\n",compute_6 (host,b));
        exit(0);

}