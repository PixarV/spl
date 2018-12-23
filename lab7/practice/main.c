#include <stdio.h>
#include <stdlib.h>
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
        u_int32_t xid;                  /* random transaction id number - chosen by this machine */
        u_int16_t secs;                 /* seconds used in timing */
        u_int16_t flags;                /* flags */
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
	//send_dhcp_discover(dhcp_socket);

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
