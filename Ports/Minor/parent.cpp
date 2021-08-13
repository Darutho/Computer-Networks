#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<vector>
#define IPPY "127.0.0.1"
#define noc 2	//no. of child processes
#define lim 2
using namespace std;

int fl=0,count=0;
void handler(int sno)
{
	++count;
	if(count==3)
	{
		fl=1;
	}
}

void term(int sno)
{
	for(int i=0;i<fd.size();i++)
		close(fd[i]);
	//for(int i=0;i<fd.size();i++)
	//	close(v[i]);
	close(sfd);
	kill(getpid(),SIGKILL);
}

int main()
{
	signal(SIGUSR1,handler);
	signal(SIGINT,term);
	for(int i=0;i<noc;i++)
	{
		if(fork()==0)
		{
			//child created
			/*
			stringstream ss;
			ss<<i;
			string s="p"+ss.str();
			string str="./"+s;
			execl(s.c_str(),str.c_str(),i,NULL);
			*/
		}
	}

	while(fl==0);

	int sfd;
	struct  sockaddr_in socky,clid;

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd == -1)	perror("sock");
	else cout<<"Sucessfully obtained socket"<<endl;

	//Initializing
	bzero((char*)&socky,sizeof(socky));
	socky.sin_family=AF_INET;
	socky.sin_port=htons(port);
	
	if(inet_aton(IPPY,(in_addr*)&socky.sin_addr.s_addr)==0)
	{
		cout<<"Server address not assigned\n";
	}
	else
	{
		cout<<"server address assigned\n";
	}

	if(-1==bind(sfd,(struct sockaddr *)&socky,sizeof(socky)))	perror("bind");
	else cout<<"Successful bind"<<endl;

	listen(sfd,5);

	while(1)
	{
		cout<<"Wait..."<<endl;
		//socklen_t *sl = 
		int clsize = sizeof(clid);
		socklen_t *m = (socklen_t *)&clsize;
		nsfd = accept(sfd,(struct sockaddr *)&clid,m);

		if(nsfd<0) perror("accept");
		else cout<<"Connected"<<endl;

		int r = fork();
		if(r==0)
		{
			close(sfd);
			func(nsfd);
			v.push_back(nsfd);
			exit(1);
		}
		else
		{
			close(nsfd);
			//exit(1);
		}
	}
}