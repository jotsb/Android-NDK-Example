/* 
 * File:   dns-spoof.h
 * Author: jb
 *
 * Created on March 30, 2016, 9:39 PM
 */

#ifndef DNS_SPOOF_H
#define DNS_SPOOF_H

#ifdef __cplusplus
extern "C" {
#endif

    // NDK Header Files
#include <jni.h>
#include <android/log.h>

    // Regular C Header Files
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <errno.h>
    //#include <features.h>

    // Network Header Files
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <net/if.h> 
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <linux/if_ether.h> 
#include <linux/if_packet.h>
#include <netinet/ether.h> 
#include <netinet/in.h>
#include <netinet/udp.h>
#include <pcap.h>
#include <arpa/inet.h>

    //#include <netinet/ip.h>
    //#include <netinet/tcp.h>
    //#include <netinet/ip_icmp.h>


    // ethernet headers are always exactly 14 bytes [1] 
#undef ETHER_HDRLEN
#define ETHER_HDRLEN 14

    // Ethernet addresses are 6 bytes
#undef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN  6

#define INET_ADDR_STRLEN 16
#define MAC_ADDR_STRLEN 18

#define DEBUG_TAG "\n[ANDROID_SECURITY_SUITE] ===> LIBPCAP_DEBUGGING ======> "

#define TRUE 1

    struct sniff_ethernet {
        u_char ether_dhost[ETHER_ADDR_LEN]; // Destination host address 
        u_char ether_shost[ETHER_ADDR_LEN]; // Source host address 
        u_short ether_type; // IP? ARP? RARP? etc 
    };

    // IP Header

    struct my_ip {
        u_int8_t ip_vhl; // header length, version 
#define IP_V(ip) (((ip)->ip_vhl & 0xf0) >> 4)
#define IP_HL(ip) ((ip)->ip_vhl & 0x0f)
        u_int8_t ip_tos; // type of service 
        u_int16_t ip_len; // total length 
        u_int16_t ip_id; // identification 
        u_int16_t ip_off; // fragment offset field 
#define IP_DF 0x4000   // dont fragment flag 
#define IP_MF 0x2000   // more fragments flag
#define IP_OFFMASK 0x1fff  // mask for fragmenting bits 
        u_int8_t ip_ttl; // time to live 
        u_int8_t ip_p; // protocol 
        u_int16_t ip_sum; // checksum 
        struct in_addr ip_src, ip_dst; // source and dest address 
    };

    // UDP Header 

    struct udp_hdr {
        u_short uh_sport; // source port 
        u_short uh_dport; // destination port
        u_short uh_ulen; // datagram length 
        u_short uh_sum; // datagram checksum
    };

    //DNS header
//
//    struct DNS_HEADER {
//        unsigned short id;
//        unsigned char rd : 1;
//        unsigned char tc : 1;
//        unsigned char aa : 1;
//        unsigned char opcode : 4;
//        unsigned char qr : 1;
//
//        unsigned char rcode : 4;
//        unsigned char cd : 1;
//        unsigned char ad : 1;
//        unsigned char z : 1;
//        unsigned char ra : 1;
//
//        unsigned short q_count;
//        unsigned short ans_count;
//        unsigned short auth_count;
//        unsigned short add_count;
//    };
//
//#pragma pack(push, 1)
//
//    struct R_DATA {
//        unsigned short type;
//        unsigned short _class;
//        unsigned int ttl;
//        unsigned short data_len;
//    };
//#pragma pack(pop)s
//
//    struct RES_RECORD {
//        unsigned char *name;
//        struct R_DATA *resource;
//        unsigned char *rdata;
//    };

    /* DNS header definition */
    struct dnshdr {
        char id[2];
        char flags[2];
        char qdcount[2];
        char ancount[2];
        char nscount[2];
        char arcount[2];
    };

    /* DNS query structure */
    struct dnsquery {
        char *qname;
        char qtype[2];
        char qclass[2];
    };

    /* DNS answer structure */
    struct dnsanswer {
        char *name;
        char atype[2];
        char aclass[2];
        char ttl[4];
        char RdataLen[2];
        char *Rdata;
    };



    int submit_log(char *msgType, char *string);
    int submit_log_i(char *msgType, int value);

    // find the first NIC that is up and sniff packets from it
    char* get_device();

    // Starts setting up the packet capturing based on the filter
    void* pcap_setup(char *filter);

    // Handles captured packets
    void pkt_callback(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char* packet);

    // Handles Ethernet packets and returns the message type
    u_int16_t handle_ethernet(u_char *args, const struct pcap_pkthdr* pkthdr, const u_char* packet);
    u_char* handle_IP(u_char *args, const struct pcap_pkthdr* pkthdr, const u_char* packet);
    u_char* handle_UDP(u_char *args, const struct pcap_pkthdr* pkthdr, const u_char* packet);
    void handle_DNS(u_char *args, const struct pcap_pkthdr* pkthdr, const char* packet);
    void extract_dns_request(struct dnsquery *dns_query, char *request);


#ifdef __cplusplus
}
#endif

#endif /* DNS_SPOOF_H */

