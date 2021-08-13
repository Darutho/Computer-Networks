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
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>

#include <arpa/inet.h>


#define pf(a...) printf(a);
#define pe(a...) fprintf(stderr, a);

/*----copied code for checksum------*/
unsigned short in_cksum(unsigned short *addr, int len)	//for the ICMP protocol since it mandates a checksum
{
	int nleft = len;
	int sum = 0;
	unsigned short *w = addr;
	unsigned short answer = 0;

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(unsigned char *) (&answer) = *(unsigned char *) w;
		sum += answer;
	}
	
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}
/*----checksum ends-----------------*/

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		pf("Usage : %s IP_ADDRESS_OF_OTHER_SIDE", argv[0]);
		exit(0);
	}

	struct ip ip;
	struct sockaddr_in sin;
	int rsfd;
	const int on = 1;
	u_char *packet;	

	//create the socket
	if((rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
	{
		perror("socket");
		exit(0);
	}

	//tell the kernel we will fill the ip header
	if(setsockopt(rsfd,IPPROTO_IP, IP_HDRINCL, &on, sizeof(on))<0)
	{
		perror("setsockopt");
		exit(0);
	}
	pf("\nRaw Socket created!");

	for(int i=0;i<4;i++)
	{
		//create space for the packet
		packet = (u_char *)malloc(60);
			
		/*------fill the ip header struct------*/
		ip.ip_hl = 0x5;				//header length
		ip.ip_v = 0x4;				//ipv4
		ip.ip_tos = 0x0;			//type of service
		ip.ip_len = htons(60);		//length of our packet
		ip.ip_id = htons(12345);	//id to identify the packet
		ip.ip_off = 0x0;			//fragment offset
		ip.ip_ttl = 64;				//#of hops of life
		ip.ip_p = IPPROTO_ICMP;		//protocol number
		ip.ip_sum = 0x0;			//checksum, 0 before passing to the checksum function
		ip.ip_src.s_addr = inet_addr("172.30.106.129");
		ip.ip_dst.s_addr = inet_addr(argv[1]);
		ip.ip_sum = in_cksum((unsigned short *)&ip, sizeof(ip));
		/*------done creating the IP HEADER------*/

		/*------create the ICMP HEADER-----------*/
		struct icmp icmp;
		icmp.icmp_type = ICMP_ECHO;
		icmp.icmp_code = 0;				// code 0 echo request
		icmp.icmp_id = 1000;
		//icmp.icmp_seq = 1;				//ICMP sequence number
		icmp.icmp_cksum = 0;
		icmp.icmp_cksum = in_cksum((unsigned short *)&icmp, 8);
		/*------done creating the ICMP HEADER----*/
		memcpy(packet, &ip, sizeof(ip));			//copy the HEADER TO THE PACKET
		
		memcpy((packet + 20), &icmp, 8);
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = ip.ip_dst.s_addr;

		struct ip iprecv;
		char * recvpacket = (char *)malloc(64);

	
		if(sendto(rsfd, packet, 60, 0, (struct sockaddr *)&sin, sizeof(struct sockaddr)) < 0)
		{
			perror("send to");
			exit(0);
		}

		struct sockaddr_in rcv;
		// rcv.sin_family = AF_INET;
		// rcv.sin_addr.s_addr = inet_addr(argv[1]);
		socklen_t size = sizeof(rcv);
		if(recvfrom(rsfd, recvpacket, sizeof(struct ip) + sizeof(struct icmp), 0, (struct sockaddr *)&rcv,&size)<0)
		{
			perror("recvfrom");
			exit(0);
		}


		pf("\nReceived packet from %s:%d\n", inet_ntoa(rcv.sin_addr), ntohs(rcv.sin_port));
		memcpy(&iprecv, recvpacket, (int)(ntohs(((struct ip *)recvpacket)->ip_len)));

		pf("\nMessage details : \n");
		pf("\nIPheader_length : %d", iprecv.ip_hl);
		pf("\nIPVERSION       : %d", iprecv.ip_v);
		pf("\nTOS             : %d", iprecv.ip_tos);
		pf("\nTTL             : %d", iprecv.ip_ttl);
		pf("\nID              : %d", ntohs(iprecv.ip_id));
		pf("\nPROTOCOL        : %d", iprecv.ip_p);
		pf("\nSOURCE          : %d", iprecv.ip_src.s_addr);
		pf("\nDEST            : %d", iprecv.ip_dst.s_addr);
		pf("\nIP len          : %d", ntohs(iprecv.ip_len));
		sleep(1);
	
	//pe("\nreached here");
	free(packet);
	free(recvpacket);
	//pe("\nfreed packet");
	}
	pf("\n");
	return 0;
	
}