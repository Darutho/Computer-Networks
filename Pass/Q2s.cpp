#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<vector>
#define IPPY "127.0.0.1"
#define noc 2
#define PATH "./SOCKNAME2"
using namespace std;
int sfd;
vector <int> v;

void send_fd(int nsfd,int fd)
{
	struct msghdr msg;
	struct cmsghdr *cmsg;
	char buff[CMSG_SPACE(sizeof fd)];
	memset(&msg,0,sizeof(struct msghdr));

	struct iovec io;
	io.iov_base = buff;
	io.iov_len = 1;

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
	msg.msg_control = buff;
	msg.msg_controllen = sizeof buff;
	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof(int));
	
	memcpy(CMSG_DATA(cmsg), &fd, sizeof(int));
	
	if(sendmsg(nsfd, &msg, 0) == -1)
	{
		perror("sendmsg ");
		exit(0);
	}
}

int main()
{
	struct sockaddr_in socks,clid;
	struct sockaddr_un socky,clids;
	int port = 20705;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd == -1)	perror("sock");
	else cout<<"Sucessfully obtained socket"<<endl;

	//Initializing
	bzero((char*)&socks,sizeof(socks));
	socks.sin_family=AF_INET;
	socks.sin_port=htons(port);
	
	if(inet_aton(IPPY,(in_addr*)&socks.sin_addr.s_addr)==0)
	{
		cout<<"Server address not assigned\n";
	}
	else
	{
		cout<<"server address assigned\n";
	}

	if(-1==bind(sfd,(struct sockaddr *)&socks,sizeof(socks)))	perror("bind");
	else cout<<"Successful bind"<<endl;

	listen(sfd,5);

	int fd = STDOUT_FILENO;
	for(int i=0;i<noc;i++)
	{
		cout<<"Wait..."<<endl;
		//socklen_t *sl = 
		int clsize = sizeof(clid);
		socklen_t *m = (socklen_t *)&clsize;
		int nsfd = accept(sfd,(struct sockaddr *)&clid,m);

		if(nsfd<0) perror("accept");
		else cout<<"Connected"<<endl;

		int r = fork();
		if(r==0)
		{
			close(sfd);
			//send_fd(nusfd,nsfd);
			v.push_back(nsfd);
			exit(1);
		}
		else
		{
			close(nsfd);
			//exit(1);
		}
	}

	//Clients connected
	cout<<"Maintenance: "<<endl;
	int usfd = socket(AF_UNIX,SOCK_STREAM,0);

	unlink(PATH);
	
	socky.sun_family = AF_UNIX;
	strcpy(socky.sun_path,PATH);

	if(-1==bind(usfd,(struct sockaddr*)&socky,sizeof(socky)))	perror("bind2");
	else cout<<"bind success"<<endl;

	listen(usfd,5);

	int clsize = sizeof(clid);
	socklen_t *len = (socklen_t *)&clsize;
	int nusfd = accept(usfd,(struct sockaddr*)&clid,len);

	cout<<"nusfd connected\n"<<endl;

	for(int i=0;i<v.size();i++)
		send_fd(nusfd,v[i]);
	//while(1)	fflush(stdout);

	return 0;
}