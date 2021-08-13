#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
using namespace std;

int main()
{
	int rsfd,len;
    if((rsfd = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP))==-1)    perror("sock");

	struct iphdr *iph;
    struct sockaddr_in dest_ip;
	char data[50];

    // dest_ip.sin_family = AF_INET;
    // dest_ip.sin_port = htons(8000);
    // dest_ip.sin_addr.s_addr = inet_addr ("127.0.0.1");

    int clsize = sizeof(dest_ip);
    socklen_t *m = (socklen_t *)&clsize;
    
    int i = 0;
    while(1)
    {
    if(recvfrom(rsfd,data,sizeof(data),0,NULL,m)==-1)  perror("recv");
    cout<<"msg is: "<<data+sizeof(iphdr)<<endl;


    iph = (iphdr*) data;
    cout<<"version: "<<iph->version<<endl;
    cout<<"source addr: "<<ntohl(iph->saddr)<<endl;
    cout<<"dest addr: "<<ntohl(iph->daddr)<<endl;
    cout<<"proto: "<<iph->protocol<<endl;
    cout<<"check: "<<iph->check<<endl;
    cout<<"total length: "<<iph->tot_len<<endl;
    cout<<"header length: "<<iph->ihl<<endl;
    cout<<"ID: "<<ntohl(iph->id)<<endl;
    // cout<<"TOS: "<<iph->tos<<endl;
    cout<<"TTL: "<<iph->ttl<<endl;
     cout<<i++<<endl;
    }

   
	return 0;
}