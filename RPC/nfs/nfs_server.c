#include "nfs.h"
#include <stdio.h>
#include<fcntl.h>
int *add_6_svc(values *argp, struct svc_req *rqstp)
{
static int n; char buff[50];
printf("****");
printf("%s",argp->fname);
int fd=open(argp->fname,O_RDONLY);
n=read(fd,buff,sizeof(buff));

return &n;
}

