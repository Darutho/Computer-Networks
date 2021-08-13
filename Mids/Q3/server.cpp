#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<vector>
#include<map>
#define IPPY "127.0.0.1"
#define nos 2
using namespace std;

int sfd;
fd_set rfds;
vector <int> fd;
int serv[10]={0,1,-1,-1,-1,-1,-1,-1,-1,-1};
void term(int sno)
{
	for(int i=0;i<fd.size();i++)
		close(fd[i]);
	close(sfd);
	kill(getpid(),SIGKILL);
}

void func(int f,int s)
{
	cout<<"Providing service no. "<<s<<endl;
	/*while(1)
	{
		char buff[100]={0};
		cout<<"Reply: ";
		if(-1==recv(f,buff,sizeof(buff),0))	perror("recv");
		char buf[100]={0};
		cout<<buff<<endl;
		cout<<"Reply: ";
		//cin.getline(buf,100);
		//if(-1==send(nsfd,buf,sizeof(buf),0))	perror("send");
	}*/
	cout<<"Done";
}

int main()
{
	signal(SIGINT,term);
	struct sockaddr_in  socky[nos],clid;
	FD_ZERO(&rfds);

	int port[nos]={20705,20706};
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
		for(int i=0;i<fd.size();i++)
		{
			FD_SET(fd[i],&rfds);
		}
			
		if(-1==select(FD_SETSIZE,&rfds,NULL,NULL,NULL))	perror("select");

		for(int i=0;i<fd.size();i++)
		{
			if(FD_ISSET(fd[i],&rfds))
			{
				if(i<nos)
				{
					int clsize = sizeof(clid);
					socklen_t *m = (socklen_t *)&clsize;
					int nsfd = accept(fd[i],(struct sockaddr *)&clid,m);
					if(nsfd<0) perror("accept");
					else{ 
						cout<<"Connected service: "<<i<<endl;
						fd.push_back(nsfd);
						if(serv[fd.size()-1]==-1) {
							serv[fd.size()-1]=i;
							cout<<"changed "<<fd.size()-1<<endl;
						}
					}
				}
				else{
					cout<<"pass "<<i<<endl;
					func(fd[i],serv[i]);
				}
			}
		}
	}
	return 0;
}