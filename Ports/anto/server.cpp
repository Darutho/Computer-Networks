#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#define SERV "127.0.0.1"
using namespace std;
void server_function(int nsfd)
{
	cout<<"You have entered the server function\n";
	while(1)
		{	
		
		char buf[256],buf2[256];
		memset(&buf,'\0',256);
		memset(&buf2,'\0',256);
		read(nsfd,buf,sizeof(buf));
		cout<<"recieved:"<<buf<<endl<<"Reply: ";
		cin.getline(buf2,256);
		int len=strlen(buf2);
		write(nsfd,buf2,len);
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
	int sfd,nsfd;
	struct sockaddr_in server_add,client_add;		
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		cout<<"Failed to assign socket\n";
		exit(1);		
	}
	else
	{
		cout<<"Socket successfully assigned\n";
	}
	
	//Initializing address
	
	bzero((char *) &server_add,sizeof(server_add));
	
	server_add.sin_family=AF_INET;
	server_add.sin_port=htons(port_no);
	if(inet_aton(SERV,(in_addr*)&server_add.sin_addr.s_addr)==0)
	{
		cout<<"Server address not assigned\n";
	}
	else
	{
		cout<<"server address assigned\n";
	}
	
	
	//Binding the socket now
	if(bind(sfd,(struct sockaddr *) &server_add,sizeof(server_add))<0)
	{
		cout<<"Socket binding failed\n";
		exit(1);	
	}
	else
	{
		cout<<"Socket binded succkessfully\n";
	}

	//Listening
	listen(sfd,5);
	
	//now to acccept clients
	cout<<"Listening....\n";
	while(1)
	{
		cout<<"Waiting to accept\n";
		int client_len=sizeof(client_add);
		socklen_t *m=(socklen_t *) &client_len;
		nsfd=accept(sfd,(struct sockaddr *) &client_add,m);
		if(nsfd<0)
		{
			cout<<"error on accept\n";
		}
		else
		{
			cout<<"Client connected\n";
		}
		int child=fork();
		if(child==0)
		{
			close(sfd);
			server_function(nsfd);
			exit(1);

		}
		else
		{
			close(nsfd);
			cout<<"Back to listening\n";
		}
	}
	return 0;
}		
	
		
