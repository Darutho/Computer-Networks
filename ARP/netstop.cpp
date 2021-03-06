#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <wait.h>
#include <signal.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
 #include <netinet/if_ether.h>
 #include <bits/stdc++.h>      
#include <arpa/inet.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <unistd.h>
#define ARP_REQUEST 1   /* ARP Request             */ 
#define ARP_REPLY 2     /* ARP Reply               */ 
#define MAXBYTES2CAPTURE 2048 

using namespace std;
char target_ip[50];
int linkhdrlen;

void capture_loop(pcap_t* pd, int packets, pcap_handler func)
{
    int linktype;
 
    // Determine the datalink layer type.
    if ((linktype = pcap_datalink(pd)) < 0)
    {
        printf("pcap_datalink(): %s\n", pcap_geterr(pd));
        return;
    }
 
    // Set the datalink layer header size.
    switch (linktype)
    {
    case DLT_NULL:
        linkhdrlen = 4;
        break;
 
    case DLT_EN10MB:
        linkhdrlen = 14;
        break;
 
    case DLT_SLIP:
    case DLT_PPP:
        linkhdrlen = 24;
        break;
 
    default:
        printf("Unsupported datalink (%d)\n", linktype);
        return;
    }
 
    // Start capturing packets.
    if (pcap_loop(pd, packets, func, 0) < 0)
        printf("pcap_loop failed: %s\n", pcap_geterr(pd));
}

void parse_packet(u_char *user, struct pcap_pkthdr *packethdr, 
                  u_char *packetptr)
{
    struct ip* iphdr;
    struct icmphdr* icmphdr;
    struct tcphdr* tcphdr;
    struct udphdr* udphdr;
    char errbuf[PCAP_ERRBUF_SIZE];
    int i;
    char iphdrInfo[256], srcip[256], dstip[256];
    unsigned short id, seq;

    // ETHERNET HEADER----------------------------------------
    // cout<<endl<<"-------ETHERNET HEADER-----------"<<endl;
    struct pcap_pkthdr hdr = *packethdr;     /* pcap.h */
    struct ether_header *eptr;  /* net/ethernet.h */

    u_char *ptr; /* printing out hardware header info */

    eptr = (struct ether_header *) packetptr;
    if (ntohs (eptr->ether_type) == ETHERTYPE_ARP)
    {
      return ;
    }
    /* copied from Steven's UNP */
    ptr = eptr->ether_dhost;
    i = ETHER_ADDR_LEN;
    // printf(" Destination Address:  ");
    // do{
    //     printf("%s%x",(i == ETHER_ADDR_LEN) ? " " : ":",*ptr++);
    // }while(--i>0);
    printf("\n");

    ptr = eptr->ether_shost;
    i = ETHER_ADDR_LEN;
    // printf(" Source Address:  ");
    // do{
    //     printf("%s%x",(i == ETHER_ADDR_LEN) ? " " : ":",*ptr++);
    // }while(--i>0);

    // Skip the datalink layer header and get the IP header fields.
    packetptr += linkhdrlen;
    iphdr = (struct ip*)packetptr;
    strcpy(srcip, inet_ntoa(iphdr->ip_src));
    strcpy(dstip, inet_ntoa(iphdr->ip_dst));
    cout<<" RCVD : "<<srcip<<" "<<dstip<<endl;
    // printf(
        // "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
}

int main(int argc,const char* argv[]) {
   if (argc<2) {
        fprintf(stderr,"usage: send_arp <interface> <ipv4-address>\n");
        exit(1);
    }
    strcpy(target_ip,argv[2]);
    const char* if_name=argv[1];
    const char* target_ip_string=argv[2];
    int pid=fork();
    if(pid==0)
    {
        while(1){
            // Construct Ethernet header (except for source MAC address).
            // (Destination set to broadcast address, FF:FF:FF:FF:FF:FF.)
            struct ether_header header;
            header.ether_type=htons(ETH_P_ARP);
            memset(header.ether_dhost,0xff,sizeof(header.ether_dhost));

            // Construct ARP request (except for MAC and IP addresses).
            struct ether_arp req;
            req.arp_hrd=htons(ARPHRD_ETHER);
            req.arp_pro=htons(ETH_P_IP);
            req.arp_hln=ETHER_ADDR_LEN;
            req.arp_pln=sizeof(in_addr_t);
            req.arp_op=htons(ARPOP_REQUEST);
            memset(&req.arp_tha,0,sizeof(req.arp_tha));

            // Convert target IP address from string, copy into ARP request.
            struct in_addr target_ip_addr={0};
            if (!inet_aton(target_ip_string,&target_ip_addr)) {
            //if (!inet_aton("255.255.255.255",&target_ip_addr)) {
               fprintf(stderr,"%s is not a valid IP address",target_ip_string);
               exit(1);
            }
            memcpy(&req.arp_tpa,&target_ip_addr.s_addr,sizeof(req.arp_tpa));

            // Write the interface name to an ifreq structure,
            // for obtaining the source MAC and IP addresses.
            struct ifreq ifr;
            size_t if_name_len=strlen(if_name);
            if (if_name_len<sizeof(ifr.ifr_name)) {
                memcpy(ifr.ifr_name,if_name,if_name_len);
                ifr.ifr_name[if_name_len]=0;
            } else {
                fprintf(stderr,"interface name is too long");
                exit(1);
            }

            // Open an IPv4-family socket for use when calling ioctl.
            int fd=socket(AF_INET,SOCK_DGRAM,0);
            if (fd==-1) {
                perror(0);
                exit(1);
            }

            // Obtain the source IP address, copy into ARP request
            if (ioctl(fd,SIOCGIFADDR,&ifr)==-1) {
                perror(0);
                close(fd);
                exit(1);
            }
            struct sockaddr_in* source_ip_addr = (struct sockaddr_in*)&ifr.ifr_addr;
            source_ip_addr->sin_addr.s_addr = inet_addr(target_ip);
            //inet_aton(target_ip_string,&rep.arp_spa);
           //sockaddr_in maskaddr;
           //inet_aton(target_ip_string,&maskaddr.sin_addr);
            memcpy(&req.arp_spa,&source_ip_addr->sin_addr.s_addr,sizeof(req.arp_spa));
            //memcpy(&req.arp_spa,&maskaddr.sin_addr.s_addr,sizeof(req.arp_spa));

            // Obtain the source MAC address, copy into Ethernet header and ARP request.
            if (ioctl(fd,SIOCGIFHWADDR,&ifr)==-1) {
                perror(0);
                close(fd);
                exit(1);
            }
            if (ifr.ifr_hwaddr.sa_family!=ARPHRD_ETHER) {
                fprintf(stderr,"not an Ethernet interface");
                close(fd);
                exit(1);
            }
            const unsigned char* source_mac_addr=(unsigned char*)ifr.ifr_hwaddr.sa_data;
            memcpy(header.ether_shost,source_mac_addr,sizeof(header.ether_shost));
            memcpy(&req.arp_sha,source_mac_addr,sizeof(req.arp_sha));
            close(fd);

            // Combine the Ethernet header and ARP request into a contiguous block.
            unsigned char frame[sizeof(struct ether_header)+sizeof(struct ether_arp)];
            memcpy(frame,&header,sizeof(struct ether_header));
            memcpy(frame+sizeof(struct ether_header),&req,sizeof(struct ether_arp));

            // Open a PCAP packet capture descriptor for the specified interface.
            char pcap_errbuf[PCAP_ERRBUF_SIZE];
            pcap_errbuf[0]='\0';
            pcap_t* pcap=pcap_open_live(if_name,96,0,0,pcap_errbuf);
            if (pcap_errbuf[0]!='\0') {
                fprintf(stderr,"%s\n",pcap_errbuf);
            }
            if (!pcap) {
                exit(1);
            }

            // Write the Ethernet frame to the interface.
            if (pcap_inject(pcap,frame,sizeof(frame))==-1) {
                pcap_perror(pcap,0);
                pcap_close(pcap);
                exit(1);
            }
            printf(" sent frame \n ");
            // Close the PCAP descriptor.
            pcap_close(pcap);
            //return 0;
        }
    }
    else
    {
          char errbuf[PCAP_ERRBUF_SIZE];
          pcap_t* descr;
          descr = pcap_open_live(argv[1],BUFSIZ,0,-1,errbuf);

        if(descr == NULL)
        {
            printf("pcap_open_live(): %s\n",errbuf);
            exit(1);
        }
        printf(" after geting desc\n");
        // while(1);
        printf("\n");
        printf(
            "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
        while(1)
        {
          int packets=10;
          capture_loop(descr, packets, (pcap_handler)parse_packet);
        }
          
    } 

}