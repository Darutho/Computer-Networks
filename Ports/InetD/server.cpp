#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<vector>
#define IPPY "127.0.0.1"
#define nos 2
using namespace std;

int sfd;
fd_set rfds;
vector <int> fd;
void term(int sno)
{
	for(int i=0;i<fd.size();i++)
		close(fd[i]);
	//for(int i=0;i<fd.size();i++)
	//	close(v[i]);
	close(sfd);
	kill(getpid(),SIGKILL);
}

void func(int nsfd)
{
	while(1)
	{
		char buff[100]={0};
		if(-1==recv(nsfd,buff,sizeof(buff),0))	perror("recv");
		char buf[100]={0};
		cout<<buff<<endl;
		//cout<<"Reply: ";
		//cin.getline(buf,100);
		//if(-1==send(nsfd,buf,sizeof(buf),0))	perror("send");
	}
}

int main(int n,char *argv[])
{
	signal(SIGINT,term);
	int nsfd;//port = atoi(argv[1]);
	struct sockaddr_in clid;
	FD_ZERO(&rfds);

	const char *a[2];
	a[0] = "ftp";
	a[1] = "http";

	int port[2]={20705,20706};
	struct sockaddr_in socky[2];
	
	for(int i=0;i<nos;i++)
	{
		sfd = socket(AF_INET,SOCK_STREAM,0);
		if(sfd == -1)	perror("sock");
		else cout<<"Sucessfully obtained socket"<<endl;
		fd.push_back(sfd);
		bzero((char*)&socky[i],sizeof(socky[i]));

		//Initializing in respective processes 
		bzero((char*)&socky[i],sizeof(socky[i]));
		socky[i].sin_family=AF_INET;
		socky[i].sin_port=htons(port[i]);
	
		if(inet_aton(IPPY,(in_addr*)&socky[i].sin_addr.s_addr)==0)
		{
			cout<<"Server address not assigned\n";
		}
		else
		{
			cout<<"server address assigned\n";
		}

		if(-1==bind(sfd,(struct sockaddr *)&socky[i],sizeof(socky[i])))	perror("bind");
		else cout<<"Successful bind"<<endl;
		listen(sfd,2);
	}

	while(1)
	{
		cout<<"Wait..."<<endl;
		for(int i=0;i<nos;i++)
		{
			FD_SET(fd[i],&rfds);
		}
			
		if(-1==select(FD_SETSIZE,&rfds,NULL,NULL,NULL))	perror("select");

		for(int i=0;i<nos;i++)
		{
			if(FD_ISSET(fd[i],&rfds))
			{
				int clsize = sizeof(clid);
				socklen_t *m = (socklen_t *)&clsize;
				nsfd = accept(fd[i],(struct sockaddr *)&clid,m);
				if(nsfd<0) perror("accept");
				else cout<<"Connected service: "<<i<<endl;

				int r = fork();
				if(r==0)
				{
					close(sfd);
					dup2(nsfd,0);
					dup2(nsfd,1);
					string str="./";
					str+=a[i];
					stringstream ss;
					ss<<nsfd;
					string s=ss.str();
					if(-1==execl(str.c_str(),a[i],s.c_str(),NULL))	perror("exec");
					//cout<<"After exec: "<<endl;
					//func(nsfd);
					exit(1);
				}
				else
				{
					close(nsfd);
					//exit(1);
				}
			}
		}
	}

	return 0;
}
