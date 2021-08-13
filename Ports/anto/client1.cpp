#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>
using namespace std;
#define SERV "127.0.0.1"
void client_function(int nsfd)
{
	cout<<"You have entered the client function\n";
	while(1)
		{	
		char buf[256],buf2[256];
		memset(&buf,'\0',256);
		memset(&buf2,'\0',256);
		cout<<"Text: ";
		cin.getline(buf2,256);
		int len=strlen(buf2);
		write(nsfd,buf2,len);
		read(nsfd,buf,sizeof(buf));
		cout<<buf<<endl;
	}
	
	
}

int main(int argc, char *argv[])
{	int port_no;
	if(argc<2)
	{
		cout<<"Please provide port\n";
		exit(1);
	}
	else
	{
		 port_no=atoi(argv[1]);	
	}
	int sfd;
	struct sockaddr_in server_add;
	if((sfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		cout<<"failed to open client socket\n";
		exit(1);
	}
	else
	{
		cout<<"client socket opened\n";
	}
	bzero(&server_add,sizeof(server_add));
	server_add.sin_family=AF_INET;
	server_add.sin_port=htons(port_no);
	if(inet_aton(SERV,(in_addr*) &server_add.sin_addr.s_addr)==0)
	{
		cout<<"failed to bind to server ip\n";
		exit(1);
	}
	else
	{
		cout<<"server ip assigned\n";
	}
	if(connect(sfd,(struct sockaddr*) &server_add,sizeof(server_add))!=0)
	{
		cout<<"Failed to connect to server\n";
	}
	else
	{
		cout<<"Connected to localhost\n";
	}
	client_function(sfd);
	return 0;
}
	
	
