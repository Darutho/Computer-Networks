#include <stdio.h> //for printf
#include <string.h> //memset
#include <sys/socket.h>    //for socket ofcourse
#include <stdlib.h> //for exit(0);
#include <errno.h> //For errno - the error number
#include <netinet/tcp.h>   //Provides declarations for tcp header
#include <netinet/ip.h>    //Provides declarations for ip header
#include <netinet/in.h>
#include <arpa/inet.h>
 
/* 
    96 bit (12 bytes) pseudo header needed for tcp header checksum calculation 
*/
struct pseudo_header
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    // u_int16_t tcp_length;
};
 
int main (void)
{
    //Create a raw socket
    int rsfd = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
     
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
    struct iphdr *iph = (struct iphdr *) datagram;
    struct sockaddr_in socky;
    struct pseudo_header psh;
     
    //Data part
    data = datagram + sizeof(struct iphdr);
    strcpy(data , "I spoofed you :D");
     
    //some address resolution
    strcpy(source_ip , "1.90.4.1");
    socky.sin_family = AF_INET;
    socky.sin_port = htons(0);
    socky.sin_addr.s_addr = inet_addr ("172.30.103.84");

    //Fill in the IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct iphdr) + strlen(data);
    iph->id = htonl (546621); //Id of this packet
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = 240;
    iph->check = 0;      //Set to 0 before calculating checksum
    iph->saddr = inet_addr ( source_ip );    //Spoof the source ip address
    iph->daddr = socky.sin_addr.s_addr;

    int one = 1;
    const int *val = &one;
     
    if (setsockopt (rsfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    {
        perror("Error setting IP_HDRINCL");
        exit(0);
    }
     
    //loop if you want to flood :)
    //while (1)
    {
        //Send the packet
        if (sendto (rsfd, datagram, iph->tot_len ,  0, (struct sockaddr *) &socky, sizeof (socky)) < 0)
        {
            perror("sendto failed");
        }
        //Data send successfully
        else
        {
            printf ("Packet Sent. Length : %d \n" , iph->tot_len);
        }
    }
     
    return 0;
}