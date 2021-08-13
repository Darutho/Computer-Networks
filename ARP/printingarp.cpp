#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pcap.h>
#include <net/ethernet.h>
#include <linux/ip.h>
#include <netinet/udp.h> 
using namespace std;

#define ARP_REQUEST 1   /* ARP Request             */ 
#define ARP_REPLY 2     /* ARP Reply               */ 
typedef struct arphdr { 
    u_int16_t htype;    /* Hardware Type           */ 
    u_int16_t ptype;    /* Protocol Type           */ 
    u_char hlen;        /* Hardware Address Length */ 
    u_char plen;        /* Protocol Address Length */ 
    u_int16_t oper;     /* Operation Code          */ 
    u_char sha[6];      /* Sender hardware address */ 
    u_char spa[4];      /* Sender IP address       */ 
    u_char tha[6];      /* Target hardware address */ 
    u_char tpa[4];      /* Target IP address       */ 
}arphdr_t; 

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header) 
{
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
}

int cnt=0;

void my_packet_handler(u_char *args,const struct pcap_pkthdr *packet_header,const u_char *packet)
{
	struct ether_header *eth_header;
	eth_header = (struct ether_header *) packet;

	int flag=0;
	if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
        printf("IP\n");
    } else  if (ntohs(eth_header->ether_type) == ETHERTYPE_ARP) {
        printf("ARP\n"); flag=1;
    } else  if (ntohs(eth_header->ether_type) == ETHERTYPE_REVARP) {
        printf("Reverse ARP\n");
    }

    if(flag)
    {
    	cnt++; int i;
        struct arphdr *arpheader = (struct arphdr *)(packet+14);
        if (ntohs(arpheader->htype) == 1 && ntohs(arpheader->ptype) == 0x0800)
        {
            printf("Operation: %s\n", (ntohs(arpheader->oper) == ARP_REQUEST)? "ARP Request" : "ARP Reply"); 
            printf("Sender MAC: "); 
            for(i=0; i<6;i++)
                printf("%02X:", arpheader->sha[i]); 
            printf("\nSender IP: ");
            for(i=0; i<4;i++)
                printf("%d.", arpheader->spa[i]); 
            printf("\nTarget MAC: "); 
            for(i=0; i<6;i++)
                printf("%02X:", arpheader->tha[i]); 
            printf("\nTarget IP: "); 
            for(i=0; i<4; i++)
                printf("%d.", arpheader->tpa[i]); 
            printf("\n"); 
          }

    }
    return;
}

int main()
{
	char *device,name[20];
    char error_buffer[PCAP_ERRBUF_SIZE];
    char ip[13];
    char subnet_mask[13];
    bpf_u_int32 ip_raw; /* IP address as integer */
    bpf_u_int32 subnet_mask_raw; /* Subnet mask as integer */
    int lookup_return_code;
    struct in_addr address;

    struct bpf_program filter;
    char filter_exp[10];
    strcpy(filter_exp,"arp");
    bpf_u_int32 fmask;

    pcap_t *handle;
    const u_char *packet;
    struct pcap_pkthdr packet_header;
    int packet_count_limit = 1;
    int timeout_limit = 10000; /* In milliseconds */

    strcpy(name,"eth0");
    device = pcap_lookupdev(error_buffer);
    if (device == NULL) {
        printf("Error finding device: %s\n", error_buffer);
        return 1;
    }
    device = name;

    /* Get device info */
    lookup_return_code = pcap_lookupnet(device,&ip_raw,&subnet_mask_raw,error_buffer);
    if (lookup_return_code == -1) { printf("%s\n", error_buffer); return 1; }

    /* Get ip in human readable form */
    address.s_addr = ip_raw;
    strcpy(ip, inet_ntoa(address));
    if (ip == NULL) { perror("inet_ntoa"); return 1; }

    /* Get subnet mask in human readable form */
    address.s_addr = subnet_mask_raw;
    strcpy(subnet_mask, inet_ntoa(address));
    if (subnet_mask == NULL) { perror("inet_ntoa"); return 1; }

    printf("Device: %s\n", device);
    printf("IP address: %s\n", ip);
    printf("Subnet mask: %s\n", subnet_mask);


    /* Open device for live capture */
    handle = pcap_open_live(device,BUFSIZ,packet_count_limit,timeout_limit,error_buffer);
    perror("open ");

    if (pcap_compile(handle, &filter, filter_exp, 0, fmask) == -1) 
    {
        printf("Bad filter - %s\n", pcap_geterr(handle));
        return 2;
    }
    if (pcap_setfilter(handle, &filter) == -1) 
    {
        printf("Error setting filter - %s\n", pcap_geterr(handle));
        return 2;
    }

    pcap_loop(handle, 3, my_packet_handler, NULL);

    // packet = pcap_next(handle, &packet_header);
    pcap_close(handle);

    cout<<"\n\nTotal arp packets : "<<cnt<<"\n\n";

    return 0;
}