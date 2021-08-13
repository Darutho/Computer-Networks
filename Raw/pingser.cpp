/*
    Raw TCP packets
    Silver Moon (m00n.silv3r@gmail.com)
*/
#include <bits/stdc++.h>
#include <stdio.h> //for printf
#include <string.h> //memset
#include <sys/socket.h>    //for socket ofcourse
#include <stdlib.h> //for exit(0);
#include <errno.h> //For errno - the error number
#include <netinet/tcp.h>   //Provides declarations for tcp header
#include <netinet/ip.h>    //Provides declarations for ip header
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>
 
using namespace std;
 
unsigned short in_cksum(unsigned short *addr, int len)  //for the ICMP protocol since it mandates a checksum
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

int main (void)
{
    //Create a raw socket
    int rsfd = socket (PF_INET, SOCK_RAW, IPPROTO_ICMP);
     
    if(rsfd == -1)
    {
        //socket creation failed, may be because of non-root privileges
        perror("Failed to create socket");
        exit(1);
    }
     
    //Datagram to represent the packet
    char datagram[4096] , source_ip[32] , *data , *pseudogram;
     
    //zero out the packet buffer
    memset (datagram, 0, 4096);
     
    //IP header
    struct icmp icmp;
    struct iphdr *iph = (struct iphdr *) datagram;
    struct sockaddr_in socky;
    // struct pseudo_header psh;
     
    //Data part
    // data = datagram + sizeof(struct iphdr);
    strcpy(data , "I love sockets :)");
     
    //some address resolution
    strcpy(source_ip , "1.90.4.1");
    socky.sin_family = AF_INET;
    socky.sin_port = htons(0);
    socky.sin_addr.s_addr = inet_addr ("127.0.0.1");

    //Fill in the IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct iphdr) + sizeof (struct iphdr) + strlen(data);
    iph->id = htonl (546621); //Id of this packet
    iph->frag_off = 0;
    iph->ttl = 64;
    iph->protocol = IPPROTO_ICMP;
    iph->check = 0;      //Set to 0 before calculating checksum
    iph->saddr = inet_addr ( source_ip );    //Spoof the source ip address
    iph->daddr = socky.sin_addr.s_addr;

    // memcpy(packet, &ip, sizeof(ip));

    icmp.icmp_type = ICMP_ECHO;
    
// Code 0. Echo Request.

    icmp.icmp_code = 0;
    
// ID. random number:

    icmp.icmp_id = htons(1000);
    
// Icmp sequence number:

    icmp.icmp_seq = htons(0);
    
// Just like with the Ip header, we set the ICMP header checksum to zero and pass the icmp packet into the cheksum function. We store the returned value in the checksum field of ICMP header:

    icmp.icmp_cksum = 0;
    icmp.icmp_cksum = in_cksum((unsigned short *)&icmp, 8);
    // icmp.icmp_cksum = in_cksum((unsigned short *)&icmp, 8);

    memcpy(datagram, &icmp, sizeof(icmp));
    memcpy(datagram+sizeof(icmp), data, sizeof(data));
    
    int one = 1;
    const int *val = &one;
     
    if (setsockopt (rsfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    {
        perror("Error setting IP_HDRINCL");
        exit(0);
    }
     
    //loop if you want to flood :)
    while (1)
    {
        cout<<"hello"<<endl;
        if (sendto (rsfd, datagram, sizeof(icmp)+sizeof(data) ,  0, (struct sockaddr *) &socky, sizeof (socky)) < 0)
        {
            perror("sendto failed");
        }
        //Data send successfully
        else
        {
            printf ("Packet Sent. Length : %d \n" , iph->tot_len);
        }
        sleep(2);
    }
     
    return 0;
}