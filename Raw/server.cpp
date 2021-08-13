#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/ip.h>

using namespace std;

int main()
{
	int rsfd,one;
	if((rsfd = socket(AF_INET,SOCK_RAW,240))==-1)    perror("sock");

	struct iphdr *iph;
	struct sockaddr_in dest_ip;
	char data[50]="I love sockets :)",source_ip;

    // strcpy(source_ip , "192.168.1.2");
    dest_ip.sin_family = AF_INET;
    dest_ip.sin_port = htons(8000);
    dest_ip.sin_addr.s_addr = inet_addr ("127.0.0.1");

    if(sendto(rsfd,data,sizeof(data),0,(sockaddr*)&dest_ip,sizeof(dest_ip))<0)	perror("send");
    cout<<"success"<<endl;

	return 0;
}