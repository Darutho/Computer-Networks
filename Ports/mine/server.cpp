#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<vector>
#define IPPY "127.0.0.1"
using namespace std;

int sfd;
vector <int> v;
void term(int sno)
{
	for(int i=0;i<v.size();i++)
		close(v[i]);
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
		cout<<"Reply: ";
		cin.getline(buf,100);
		if(-1==send(nsfd,buf,sizeof(buf),0))	perror("send");
	}
}

int main(int n,char *argv[])
{
	signal(SIGINT,term);
	int nsfd,port = atoi(argv[1]);
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

	return 0;
}