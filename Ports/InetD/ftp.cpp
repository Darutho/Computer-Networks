#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<vector>
#define IPPY "127.0.0.1"
#define port 20705
using namespace std;

int main(int n,char *argv[])
{
	/*struct sockaddr_in socky;
	bzero((char*)&socky,sizeof(socky));
	socky.sin_family=AF_INET;
	socky.sin_port=htons(port);
	
	if(inet_aton(IPPY,(in_addr*)&socky.sin_addr.s_addr)==0)
	{
		cout<<"ftp address not assigned\n";
	}
	else
	{
		cout<<"ftp address assigned\n";
	}
	*/
	int nsfd = atoi(argv[1]);
	while(1)
	{
		char buff[100]={0};
		if(-1==recv(nsfd,buff,sizeof(buff),0))	perror("recv");
		cout<<buff<<endl;
		cout<<"In ftp"<<endl;
		sleep(3);	
	}
	return 0;
}
