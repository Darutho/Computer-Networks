#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

using namespace std;

int main()
{
	int rsfd;
	rsfd = socket(AF_INET,SOCK_RAW,0);

	struct iphdr *iph;

	cout<<"version: "<<iph->version<<endl;
	cout<<"source addr: "<<htonl(iph->saddr)<<endl;
	cout<<"dest addr: "<<ntohl(iph->daddr)<<endl;
	cout<<"proto: "<<iph->protocol<<endl;
	cout<<"check: "<<iph->check<<endl;
	cout<<"total length: "<<iph->tot_len<<endl;
	cout<<"header length: "<<iph->ihl<<endl;
	cout<<"ID: "<<iph->id<<endl;
	cout<<"TOS: "<<iph->tos<<endl;
	cout<<"TTL: "<<iph->ttl<<endl;


	return 0;
}