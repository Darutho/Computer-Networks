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
        printf("IP\n"); flag=1;
    } else  if (ntohs(eth_header->ether_type) == ETHERTYPE_ARP) {
        printf("ARP\n");
    } else  if (ntohs(eth_header->ether_type) == ETHERTYPE_REVARP) {
        printf("Reverse ARP\n");
    }

    if(flag)
    {
    	struct iphdr *ip = (iphdr*) (packet+14);
	    cout<<"Internet Header length : "<<((unsigned int)ip->ihl*4)<<endl;
	    cout<<"Protocol : "<<(unsigned int)ip->protocol<<endl;

	    if((unsigned int)ip->protocol == 17)
	    {
	    	cnt++;
	    	struct udphdr *upckt = (udphdr*) (packet+14+(ip->ihl*4));
	    	cout<<"Source port : "<<ntohs(upckt->source)<<"\n";
	    	cout<<"Destination port : "<<ntohs(upckt->dest)<<"\n";
	    	cout<<"length : "<<ntohs(upckt->len)<<"\n";
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
    strcpy(filter_exp,"udp");
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

    pcap_loop(handle,5, my_packet_handler, NULL);

    // packet = pcap_next(handle, &packet_header);
    pcap_close(handle);

    cout<<"\n\nTotal udp packets : "<<cnt<<"\n\n";

    return 0;
}
