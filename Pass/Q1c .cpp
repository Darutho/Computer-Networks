#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/un.h>
#define PATH "./SOCKNAME"
using namespace std;

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

void recv_fd(int usfd, int *fd)
{
	struct msghdr msg;
	struct cmsghdr *cmsg;
	memset(&msg, 0, sizeof(struct msghdr));
	char buf[256] = {0};
	
	char m_buffer[1];
    struct iovec io = { .iov_base = m_buffer, .iov_len = sizeof(m_buffer) };
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
	
	msg.msg_control = buf;
	msg.msg_controllen = sizeof buf;
	//cout<<1<<endl;
	if (recvmsg(usfd, &msg, 0) < 0)
    {
        perror("recvmsg");
        exit(-1);
    }
	//printf("1\n");
	cmsg = CMSG_FIRSTHDR(&msg);
	
	memcpy(fd, CMSG_DATA(cmsg), sizeof(int));
    cout<<"Extracted fd: "<<*fd<<endl;
}

int main()
{
	int usfd, fd;
	struct sockaddr_un addr,clid;
	
	if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("socket ");
		exit(0);
	}
	
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, PATH);
	if(connect(usfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		perror("connect ");
		exit(0);
	}
	
	recv_fd(usfd, &fd);
	while(1)
	{
		char buf2[50] = {0};
		if(-1==recv(usfd,buf2,sizeof buf2,0))	perror("recv");
		cout<<buf2<<endl;
		sleep(2);
		char buff[] = "Hey, this is client writing to your stdout..\n";
		write(fd, buff, sizeof buff);
		char buf1[] = "Client done \n";
		send(usfd,buf1,sizeof buf1,0);
		
	}

	close(fd);
	exit(0);
}
