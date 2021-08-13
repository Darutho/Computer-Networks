#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
using namespace std;

int main()
{
	int rsfd,len;
    if((rsfd = socket(AF_INET,SOCK_RAW,IPPROTO_UDP))==-1)    perror("sock");

	struct iphdr *iph;
	struct udphdr *udp;
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
    cout<<"msg is: "<<data+sizeof(iphdr)+sizeof(udp)<<endl;
    cout<<"Recveived "<<endl;

    iph = (iphdr*) data;
    udp = (udphdr *) data + 20;

    cout<<"version: "<<udp->uh_ulen<<endl;
    cout<<"source port: "<<ntohs(udp->uh_sport)<<endl;
    cout<<"dest port: "<<ntohs(udp->uh_dport)<<endl;

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