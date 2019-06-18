#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <sys/time.h>
#define true 1
#define false 0
//#define DEBUG


// Writes a simple HTTP request to the server.
// Returns Number of characters that should have been put into buffer.
// len = length of buffer, host = hostname, path = path on host.
int createHttpRequest(char * buffer, int len, char *host, char *port, char *path);
// Connects to server and returns the socket file descriptor that has the connection.
int connect_to_server(struct addrinfo *servinfo);
// Prints out the IP addresses for an addrinfo (and if it is IPv6)
void print_addrinfo(char *hostname, struct addrinfo *servinfo);

int main(int argc, char **argv) {
	int port_number;
	char *server_host, *server_path, *port_string;
	char print_rtt = false; // Treat as boolean -- whether to print round trip time

	// Read in parameters:

	if(argc <= 2 || argc > 4) {
		fprintf(stderr,"Usage:\t %s [-p] server_url port_number\n", argv[0]);
		exit(1);
	}
	if(! strcmp(argv[1], "-p")) {
		// -p option is used
		print_rtt = true;
		if(argc == 3) {
			fprintf(stderr,"Usage:\t %s [-p] server_url port_number\n", argv[0]);
			exit(1);
		}
	} else if(argc == 4) {
		fprintf(stderr,"Usage:\t %s [-p] server_url port_number\n", argv[0]);
		exit(1);
	}

	port_number = atoi(argv[print_rtt ? 3 : 2]);
	port_string = 	   argv[print_rtt ? 3 : 2];
	server_host = 	   argv[print_rtt ? 2 : 1];

	server_path = server_host;
	while(server_path[0] != '\0' && server_path[0] != '/') {
		server_path++;
	}
	if(server_path[0] == '/') {
		server_path[0] = '\0';
		server_path++;
	} else {
		server_path = "";
	}

	if(port_number == 0) {
		fprintf(stderr, "Invalid port number.\n");
		return 1;
	}

#ifdef DEBUG
	printf("Input:\n\tPort: %d,\n\tHost: %s,\n\tPath: %s,\n\tprint_rtt: %s\n", port_number, server_host, server_path, print_rtt ? "true" : "false");
#endif


	// Get information about the server:
	////////////////////////////////////

	// This chunk of code is based on Beej's Guide to network Programming
	// http://beej.us/guide/bgnet/
	int status;
	struct addrinfo hints;		// Information about the connection we wish to make.
	struct addrinfo *servinfo;	// Data about server (is a linked list)

	memset(&hints, 0, sizeof(hints));	// any protocol, addrlen, addr, canonname, or next
	hints.ai_family = AF_UNSPEC;		// Don't care about IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;	// TCP stream sockets
	//hints.ai_flags = AI_PASSIVE;		// Fill in my IP address

	/*
	getaddrinfo(const char *node, 			//IP or "www.google.com"
				const char *service,		// "http" or port number (can use services in IANA port list (in /etc/services/ list))
				const struct *addrinfo, 	// Information about the client side address
				const struct addrinfo **res // The returned information - list of addresses that the node can be found at. Usually we care about the 1st.
	);
	*/
	if(0 != (status = getaddrinfo(server_host, port_string, &hints, &servinfo))) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}

#ifdef DEBUG
	print_addrinfo(server_host, servinfo);
#endif

	// Create a socket & connect to the server:
	///////////////////

	int socket_fd = connect_to_server(servinfo);
	freeaddrinfo(servinfo); // Free the linked list.

	// Send some data
	//send(socket_fd, message, length, flags); // if flags == 0, send is equivalent to write
	// ssize_t write(int fd, const void *buf, size_t count); // count is a maximum
	
	char http_request[512];
	createHttpRequest(http_request, 512, server_host,port_string, server_path);

	struct timeval start, stop;
	char hasnt_printed_yet = true;
	if(print_rtt) {
		gettimeofday(&start, NULL);
	}

	ssize_t written_bytes = write(socket_fd, http_request, strlen(http_request));
	if(written_bytes <= 0 || written_bytes != strlen(http_request)) {
		fprintf(stderr, "Failed to write to socket.\n");
		exit(1);
	}


	// Receive some data
	// recv(socket_fd, buffer, length, flags);
	char buffer[512];
	ssize_t read_bytes;

	while( 0 < (read_bytes = read(socket_fd, buffer, 512))) {

		if(print_rtt && hasnt_printed_yet) {
			// printout round trip time.
			gettimeofday(&stop, NULL);
			long unsigned rtt_millis = (stop.tv_usec - start.tv_usec)/1000 + (stop.tv_sec - start.tv_sec) * 1000;
			printf("Round trip time: %lums\n\n", rtt_millis);
			hasnt_printed_yet = false;
		}

		buffer[read_bytes] = '\0';

		printf("%s", buffer);
	}

	// Close connection
	if(close(socket_fd)) {
		fprintf(stderr, "Failed to close socket.\n");
		exit(1);
	}

	return 0;
}

// Writes a simple HTTP request to the server.
// Returns Number of characters that should have been put into buffer.
// len = length of buffer, host = hostname, path = path on host.
int createHttpRequest(char * buffer, int len, char *host, char *port, char *path) {
	return snprintf(buffer, len,
	"GET /%s HTTP/1.1\r\n"
	"Host: %s:%s\r\n"
	"Connection: close\r\n"
	"\r\n", path, host, port);
}

// Connects to server and returns the socket file descriptor that has the connection.
int connect_to_server(struct addrinfo *servinfo) {
	if(servinfo == NULL) {
		// If all possible IP addresses have already failed to connect.
		fprintf(stderr, "No possible connection.\n");
		exit(1);
	}

	/*
	int socket( int domain, - PF_INET or PF_INET6
				int type,	- SOCK_STREAM or SOCK_DGRAM
				int protocol - use 0 or getprotobyname("tcp").p_proto - pulls from /etc/protocols 
				);
	*/
	int socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	if(socket_fd == -1) {
		fprintf(stderr, "Problem creating socket...Trying again with next address.\n");
		return connect_to_server(servinfo->ai_next);
	}

	// connect(int socket_file_descriptor, const struct sockaddr *addr, socklen_t addrlen);
	if(connect(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen)) {
		fprintf(stderr, "Problem connecting to server. %s\n", strerror(errno));
		close(socket_fd);
		return connect_to_server(servinfo->ai_next);
	}

	return socket_fd;
}

// Prints out the IP addresses for an addrinfo (and if it is IPv6)
void print_addrinfo(char *hostname, struct addrinfo *servinfo) {
	char ipstr[INET6_ADDRSTRLEN];

	printf("IP addresses for %s:\n", hostname);

	while(servinfo != NULL) {

		void *addr;
		char *ipver;

		// get the pointer to the address itself.
		// different fields in IPv4 and v6
		if(servinfo->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4  = (struct sockaddr_in *) servinfo->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)servinfo->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		// Convert the IP to a string and print it:
		inet_ntop(servinfo->ai_family, addr, ipstr, sizeof ipstr);
		printf("\t%s: %s\n", ipver, ipstr);

		servinfo = servinfo->ai_next;
	}
	printf("\n");
}
