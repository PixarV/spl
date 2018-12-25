#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

/**** Common definitions ****/

#define STATE_UNKNOWN     -1

#define OK                0
#define ERROR             -1

#define FALSE             0
#define TRUE              1

/**** DHCP definitions ****/

#define MAX_DHCP_CHADDR_LENGTH           16
#define MAX_DHCP_SNAME_LENGTH            64
#define MAX_DHCP_FILE_LENGTH             128
#define MAX_DHCP_OPTIONS_LENGTH          312

typedef struct dhcp_packet {
    uint8_t op, htype, hlen, hops;
    uint32_t xid;                  /* random transaction id number - chosen by this machine */
    uint16_t secs;                 /* seconds used in timing */
    uint16_t flags;                /* flags */
    struct in_addr ciaddr;          /* IP address of this machine (if we already have one) */
    struct in_addr yiaddr;          /* IP address of this machine (offered by the DHCP server) */
    struct in_addr siaddr;          /* IP address of DHCP server */
    struct in_addr giaddr;          /* IP address of DHCP relay */
    unsigned char chaddr [MAX_DHCP_CHADDR_LENGTH];      /* hardware address of this machine */
    char sname [MAX_DHCP_SNAME_LENGTH];    /* name of DHCP server */
    char file [MAX_DHCP_FILE_LENGTH];      /* boot file name (used for diskless booting?) */
    char options[MAX_DHCP_OPTIONS_LENGTH];  /* options */
} dhcp_packet;

#define DHCP_SERVER_PORT   67
#define DHCP_CLIENT_PORT   68

#define DHCP_SERVER_ADDR "172.16.4.1"
#define DHCP_CLIENT_ADDR "172.16.4.5"
#define DHCP_MASK "255.255.255.0"

int create_dhcp_socket(void){
    struct sockaddr_in myname; // socket for ip protocols
    int sock;

    bzero(&myname, sizeof(myname));
    myname.sin_family = AF_INET; 
    myname.sin_port = htons(DHCP_SERVER_PORT); // htons - define correct order of bytes
    myname.sin_addr.s_addr = inet_addr(DHCP_SERVER_ADDR);                 
    bzero(&myname.sin_zero, sizeof(myname.sin_zero));

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if (sock < 0) {
        printf("Error: Could not create socket!\n");
        exit(STATE_UNKNOWN);
    }

    printf("DHCP socket: %d\n",sock);

    if (bind(sock, (struct sockaddr*)&myname, sizeof(myname)) < 0) {
        printf("Error: Could not bind to DHCP socket (port %d)!\n", DHCP_SERVER_PORT);
        exit(STATE_UNKNOWN);
    }

    return sock;
}

char const* network_interface_name = "enp0s31f6";

unsigned char* get_hardware_address(int sock, unsigned char* client_hardware_address){
    struct ifreq ifr;

    strncpy((char*)&ifr.ifr_name, network_interface_name, sizeof(ifr.ifr_name));

    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0){
        printf("Error: Could not get hardware address of interface '%s'\n", network_interface_name);
        exit(STATE_UNKNOWN);
    }

    memcpy(&client_hardware_address[0], &ifr.ifr_hwaddr.sa_data, 6);
    return client_hardware_address;
}

/* sends a DHCP packet */
int send_dhcp_packet(void* buffer, int buffer_size, int sock, struct sockaddr_in* dest){
    int result;

    result = sendto(sock, (char*)buffer, buffer_size, 0, (struct sockaddr*)dest, sizeof(*dest));
    printf("send_dhcp_packet result: %d\n",result);

    if(result < 0) {
			perror("hey");
			return ERROR; 
	}

    return OK;
}

void print_info(dhcp_packet const* packet, char const* name) {
    printf("%s XID: %lu (0x%X)\n", name, (unsigned long) ntohl(packet->xid), ntohl(packet->xid));
    
    printf("%s ciaddr: %s\n", name, inet_ntoa(packet->ciaddr));
    printf("%s yiaddr: %s\n", name, inet_ntoa(packet->yiaddr));
    printf("%s siaddr: %s\n", name, inet_ntoa(packet->siaddr));
    printf("%s giaddr: %s\n", name, inet_ntoa(packet->giaddr));
    
    printf("Hardware address: ");
    for (int i = 0; i < 6; ++i) {
        printf("%2.2x", packet->chaddr[i]);
    }
    printf( "\n");
}

#define BOOTREQUEST     1
#define BOOTREPLY       2
#define ETHERNET_HARDWARE_ADDRESS            1     /* ethernet type */
#define ETHERNET_HARDWARE_ADDRESS_LENGTH     6     /* length of our hardware address */
#define NUM_OF_ROUTERS                       0     /* Number of intermediate routeirs */

#define DHCPDISCOVER    1
#define DHCPOFFER       2
#define DHCPREQUEST     3
#define DHCPACK         5
#define DHCPNACK        6

#define DHCP_OPTION_MESSAGE_TYPE        53 // discover

int send_dhcp_discover(int sock, unsigned char* client_hardware_address){
    dhcp_packet discover_packet;
    struct sockaddr_in sockaddr_broadcast;

    bzero(&discover_packet, sizeof(discover_packet));
    
    discover_packet.op = BOOTREQUEST;
    discover_packet.htype = ETHERNET_HARDWARE_ADDRESS; 
    discover_packet.hlen = ETHERNET_HARDWARE_ADDRESS_LENGTH; 
    discover_packet.hops = NUM_OF_ROUTERS;

    srand(time(NULL));
    uint32_t packet_xid = random();
    discover_packet.xid = htonl(packet_xid);
    ntohl(discover_packet.xid);

    discover_packet.secs = 0;
    discover_packet.flags = 0;

    memcpy(discover_packet.chaddr, client_hardware_address, ETHERNET_HARDWARE_ADDRESS_LENGTH);

    // magic cookie 
    discover_packet.options[0] = '\x63';
    discover_packet.options[1] = '\x82';
    discover_packet.options[2] = '\x53';
    discover_packet.options[3] = '\x63';

    discover_packet.options[4] = DHCP_OPTION_MESSAGE_TYPE; 
    discover_packet.options[4] = '\x1'; // length 
    discover_packet.options[5] = DHCPDISCOVER; 
    // options...
   
	sock= socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int broadcastEnable = 1;
	int ret=setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));	

    // fill info about server
    sockaddr_broadcast.sin_port = htons(DHCP_SERVER_PORT);
    sockaddr_broadcast.sin_addr.s_addr = inet_addr("172.16.4.1");
    bzero(&sockaddr_broadcast.sin_zero, sizeof(sockaddr_broadcast.sin_zero));

    printf("DHCPDISCOVER to %s port %d\n", inet_ntoa(sockaddr_broadcast.sin_addr), ntohs(sockaddr_broadcast.sin_port));
    print_info(&discover_packet, "DISCOVERY");

    return send_dhcp_packet(&discover_packet, sizeof(discover_packet), sock, &sockaddr_broadcast);
}

int receive_dhcp_packet(void *buffer, int buffer_size, int sock, struct sockaddr_in* address) {
        int recv_result;
        socklen_t address_size;
        struct sockaddr_in source_address;

        bzero(&source_address, sizeof(source_address));
        address_size = sizeof(source_address);
        
        recv_result = recvfrom(sock,
                        (char *)buffer,
                        buffer_size,
                        MSG_PEEK,
                        (struct sockaddr *)&source_address,
                        &address_size);

        printf("recv_result_1: %d\n",recv_result);

        if(recv_result < 0) {
            printf("recvfrom() failed");
            return ERROR;
        }

        printf("receive_dhcp_packet() result: %d\n", recv_result);
        printf("receive_dhcp_packet() source: %s\n", inet_ntoa(source_address.sin_addr));

        memcpy(address, &source_address, sizeof(source_address));
        return OK;
}

int get_dhcp_offer(int sock){
	dhcp_packet offer_packet;
    struct sockaddr_in source, pseudo_client;
    bzero(&offer_packet, sizeof(offer_packet));
    
    uint32_t result = receive_dhcp_packet(&offer_packet, sizeof(offer_packet), sock, &source);
    
    if(result != OK){
        return ERROR;
    }

    offer_packet.op = BOOTREPLY;
    offer_packet.yiaddr.s_addr = inet_addr(DHCP_CLIENT_ADDR);
    offer_packet.siaddr.s_addr = inet_addr(DHCP_SERVER_ADDR);

    // magic cookie T
    offer_packet.options[0] = '\x63';
    offer_packet.options[1] = '\x82';
    offer_packet.options[2] = '\x53';
    offer_packet.options[3] = '\x63';

    offer_packet.options[4] = DHCP_OPTION_MESSAGE_TYPE; 
    offer_packet.options[4] = '\x1'; // length 
    offer_packet.options[5] = DHCPOFFER; 
	
    print_info(&offer_packet, "OFFER");
    // options....
	
	// fill info about client
    pseudo_client.sin_port = htons(DHCP_CLIENT_PORT);
    //pseudo_client.sin_addr.s_addr = INADDR_ANY;
    bzero(&pseudo_client.sin_zero, sizeof(pseudo_client.sin_zero));

    printf("DHCPOFFER from IP address %s\n", inet_ntoa(source.sin_addr));
    
    return send_dhcp_packet(&offer_packet, sizeof(offer_packet), sock, &pseudo_client);
}


int main(int argc, char** argv){
    uint32_t dhcp_socket, result;
    
    printf("hey\n");

    dhcp_socket=create_dhcp_socket();
    if (dhcp_socket < 0) {
        exit(STATE_UNKNOWN);
    }

    unsigned char client_hardware_address[MAX_DHCP_CHADDR_LENGTH];
    get_hardware_address(dhcp_socket, client_hardware_address);
    
    if (dhcp_socket < 0) {
        exit(STATE_UNKNOWN);
    }

    printf("Hardware address: ");
    for (int i = 0; i < 6; ++i) {
        printf("%2.2x", client_hardware_address[i]);
    }
    printf( "\n");

    send_dhcp_discover(dhcp_socket, client_hardware_address);

    get_dhcp_offer(dhcp_socket);
    //print_info(offer_packet, "OFFER");

    //send_dhcp_request(dhcp_socket, offer_packet);


    close(dhcp_socket);
    return result;
}
