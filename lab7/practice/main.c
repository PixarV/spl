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

#define STATE_OK          0
#define STATE_WARNING     1
#define STATE_CRITICAL    2
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

typedef struct dhcp_offer {
	struct in_addr server_address;   /* address of DHCP server that sent this offer */
	struct in_addr offered_address;  /* the IP address that was offered to us */
	u_int32_t lease_time;            /* lease time in seconds */
	u_int32_t renewal_time;          /* renewal time in seconds */
	u_int32_t rebinding_time;        /* rebinding time in seconds */
	struct dhcp_offer_struct* next;
} dhcp_offer;

typedef struct requested_server {
	struct in_addr server_address;
	struct requested_server_struct* next;
} requested_server;

#define DHCP_SERVER_PORT   67
#define DHCP_CLIENT_PORT   68


/* creates a socket for DHCP communication */
int create_dhcp_socket(void){
    struct sockaddr_in myname; // socket for ip protocols
    int sock;

    /* Set up the address we're going to bind to. */
	bzero(&myname, sizeof(myname));
    myname.sin_family = AF_INET; 
    myname.sin_port = htons(DHCP_SERVER_PORT); // htons - define correct order of bytes
    myname.sin_addr.s_addr = INADDR_ANY;                 /* listen on any address */
    bzero(&myname.sin_zero, sizeof(myname.sin_zero));

    /* create a socket for DHCP communications */
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
    if (sock < 0) {
		printf("Error: Could not create socket!\n");
		exit(STATE_UNKNOWN);
	}

	printf("DHCP socket: %d\n",sock);

    /* bind the socket */
    if (bind(sock, (struct sockaddr*)&myname, sizeof(myname)) < 0){
		printf("Error: Could not bind to DHCP socket (port %d)!\n", DHCP_SERVER_PORT);
		exit(STATE_UNKNOWN);
	}

    return sock;
}

char const* network_interface_name = "enp0s31f6";

/* determines hardware address on client machine */
unsigned char* get_hardware_address(int sock, unsigned char* client_hardware_address){
	struct ifreq ifr;

	strncpy((char*)&ifr.ifr_name, network_interface_name, sizeof(ifr.ifr_name));

	/* try and grab hardware address of requested interface */
	if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0){
		printf("Error: Could not get hardware address of interface '%s'\n", network_interface_name);
		exit(STATE_UNKNOWN);
	}

	memcpy(&client_hardware_address[0], &ifr.ifr_hwaddr.sa_data, 6);
	return client_hardware_address;
}

/* sends a DHCP packet */
int send_dhcp_packet(void* buffer, int buffer_size, int sock, struct sockaddr_in* dest){
	struct sockaddr_in myname;
	int result;

	result = sendto(sock, (char*)buffer, buffer_size, 0, (struct sockaddr *)dest, sizeof(*dest));

	printf("send_dhcp_packet result: %d\n",result);

	if(result < 0)
		return ERROR;

	return OK;
}

#define BOOTREQUEST     1
#define BOOTREPLY       2
#define ETHERNET_HARDWARE_ADDRESS            1     /* ethernet type */
#define ETHERNET_HARDWARE_ADDRESS_LENGTH     6     /* length of our hardware address */
#define NUM_OF_ROUTERS     			 		 0     /* Number of intermediate routeirs */

#define DHCPDISCOVER    1
#define DHCPOFFER       2
#define DHCPREQUEST     3
#define DHCPDECLINE     4
#define DHCPACK         5
#define DHCPNACK        6
#define DHCPRELEASE     7

#define DHCP_OPTION_MESSAGE_TYPE        53 // discover
#define DHCP_OPTION_HOST_NAME           12
#define DHCP_OPTION_BROADCAST_ADDRESS   28
#define DHCP_OPTION_REQUESTED_ADDRESS   50
#define DHCP_OPTION_LEASE_TIME          51
#define DHCP_OPTION_RENEWAL_TIME        58
#define DHCP_OPTION_REBINDING_TIME      59

#define DHCP_BROADCAST_FLAG 32768

/* sends a DHCPDISCOVER broadcast message in an attempt to find DHCP servers */
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

	/*discover_packet.secs=htons(65535);*/
	//discover_packet.secs=0xFF;
	discover_packet.secs = 0;

	/* tell server it should broadcast its response */ 
	//discover_packet.flags=htons(DHCP_BROADCAST_FLAG);
	discover_packet.flags = 0;

	memcpy(discover_packet.chaddr, client_hardware_address, ETHERNET_HARDWARE_ADDRESS_LENGTH);

	// magic cookie 
	discover_packet.options[0] = '\x63';
	discover_packet.options[1] = '\x82';
	discover_packet.options[2] = '\x53';
	discover_packet.options[3] = '\x63';

	/* DHCP message type is embedded in options field */
	discover_packet.options[4] = DHCP_OPTION_MESSAGE_TYPE;    /* DHCP message type option identifier */
	discover_packet.options[5] = '\x01';               /* DHCP message option length in bytes */
	discover_packet.options[6] = DHCPDISCOVER;
	
	/* send the DHCPDISCOVER packet to broadcast address */
    sockaddr_broadcast.sin_family = AF_INET;
    sockaddr_broadcast.sin_port = htons(DHCP_CLIENT_PORT);
    sockaddr_broadcast.sin_addr.s_addr = INADDR_ANY;
	bzero(&sockaddr_broadcast.sin_zero,sizeof(sockaddr_broadcast.sin_zero));

	printf("DHCPDISCOVER to %s port %d\n", inet_ntoa(sockaddr_broadcast.sin_addr), ntohs(sockaddr_broadcast.sin_port));
	printf("DHCPDISCOVER XID: %lu (0x%X)\n", (unsigned long) ntohl(discover_packet.xid), ntohl(discover_packet.xid));
	printf("DHCDISCOVER ciaddr:  %s\n", inet_ntoa(discover_packet.ciaddr));

	printf("DHCDISCOVER yiaddr:  %s\n", inet_ntoa(discover_packet.yiaddr));
	printf("DHCDISCOVER siaddr:  %s\n", inet_ntoa(discover_packet.siaddr));
	printf("DHCDISCOVER giaddr:  %s\n", inet_ntoa(discover_packet.giaddr));
	printf("Hardware address: ");
	for (int i = 0; i < 6; ++i) {
		printf("%2.2x", discover_packet.chaddr[i]);
	}
	printf( "\n");

	/* send the DHCPDISCOVER packet out */
	send_dhcp_packet(&discover_packet, sizeof(discover_packet), sock, &sockaddr_broadcast);

	printf("\n\n");

	return OK;
}




int main(int argc, char** argv){
	int dhcp_socket, result;
	
	printf("hey\n");

//	if (process_arguments(argc,argv)!=OK) {
//		printf("Could not parse arguments");
//	}

	/* create socket for DHCP communications */
	dhcp_socket=create_dhcp_socket();
	if (dhcp_socket < 0) {
		exit(STATE_UNKNOWN);
	}

	/* get hardware address of client machine */
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

	/* send DHCPDISCOVER packet */
	send_dhcp_discover(dhcp_socket, client_hardware_address);

	/* wait for a DHCPOFFER packet */
	//get_dhcp_offer(dhcp_socket);

	/* close socket we created */
	//close_dhcp_socket(dhcp_socket);

	/* determine state/plugin output to return */
	//result=get_results();

	/* free allocated memory */
	//free_dhcp_offer_list();
	//free_requested_server_list();

	return result;
}
