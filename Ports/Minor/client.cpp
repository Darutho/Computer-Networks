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
void term(int sno)
{
	//for(int i=0;i<v.size();i++)
		//close(v[i]);
	close(sfd);
	kill(getpid(),SIGKILL);
}

void func(int nsfd)
{
	while(1)
	{
		char buf[100]={0};
		cout<<"msg: ";
		cin.getline(buf,100);
		if(-1==send(nsfd,buf,sizeof(buf),0))	perror("send");
		char buff[100]={0};
		if(-1==recv(nsfd,buff,sizeof(buff),0))	perror("recv");
		cout<<buff<<endl;
	}
}

int main(int n,char *argv[])
{
	int port = atoi(argv[1]);
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd == -1)	perror("sock");
	else cout<<"Sucessfully obtained socket"<<endl;
	struct sockaddr_in socky;
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

	if(connect(sfd,(struct sockaddr*) &socky,sizeof(socky))!=0)
	{
		cout<<"Failed to connect to server\n";
	}
	else
	{
		cout<<"Connected to localhost\n";
	}
	func(sfd);

	return 0;
}