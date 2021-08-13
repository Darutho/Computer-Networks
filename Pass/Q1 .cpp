#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
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

void recv_fd(int usfd, int *pfd)
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
	
	memcpy(pfd, CMSG_DATA(cmsg), sizeof(int));
    cout<<"Extracted fd: "<<*pfd<<endl;
}

int main()
{
	int usfd = socket(AF_UNIX,SOCK_STREAM,0);

	unlink(PATH);
	struct sockaddr_un socky,clid;
	socky.sun_family = AF_UNIX;
	strcpy(socky.sun_path,PATH);

	if(-1==bind(usfd,(struct sockaddr*)&socky,sizeof(socky)))	perror("bind");
	else cout<<"bind success"<<endl;

	listen(usfd,5);

	int clsize = sizeof(clid);
	socklen_t *len = (socklen_t *)&clsize;
	int nsfd = accept(usfd,(struct sockaddr*)&clid,len);

	cout<<"Client connected\n"<<endl;

	mkfifo("bang",O_CREAT|0666);
	int pfd,fd;
	if(-1==(fd = open("bang",O_WRONLY))) perror("open");
	send_fd(nsfd,fd);

	while(1)
	{
		char buff[] = "Hey, this is server writing to your stdout.. 2\n";
		write(fd, buff, sizeof buff);
		char buf1[] = "Server done \n";
		send(nsfd,buf1,sizeof buf1,0);
		
		char buf2[50] = {0};
		recv(nsfd,buf2,sizeof buf2,0);
		cout<<buf2<<endl;
		sleep(2);
	}
	
	close(fd);

	return 0;
}