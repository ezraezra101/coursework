#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sched.h>

#define DEFAULT_FILE "./TMDG.html"

#define BUFFER_SIZE 128
#define CHUNK_SIZE  512 // Bytes read from a file at a time

#define THREAD_COUNT 10

// Adds a HTTP header to the buffer and sends it over the network.
int sendHttp(int connection_socket_fd, char *buffer);

// Sends the filepath to the socket, including the header.
int sendFile(char *filepath, int socket_fd);

// Sends an http header.
int sendHttpHeader(int socket_fd, int content_length);

void *server_worker(void *n);

// Get the filepath from the http request, discarding the rest of the request..
int getFilepath(int connection_socket_fd, char *filepath_buffer, int filepath_buffer_length);

// Sends a 404 Not Found error.
int send404Error(int socket_fd);

// Starts the server shutdown process.
void terminate(int signum);

// Returns whether the server should be shutting down.
int shouldContinue();

void getDate(char *str_date, int size) {
	time_t t = time(NULL);
	struct tm *tm;
	tm = gmtime( &t);
	// Hopefully will be of the form: Date: Fri, 31 Dec 1999 23:59:59 GMT
	strftime(str_date, size, "Date: %a, %d %b %H:%M:%S GMT\r\n", tm);
}

int main(int argc, char **argv) {
	int port_number;
	if(argc != 2 ||  0 >= (port_number = atoi(argv[1])) ) {
		fprintf(stderr,"Usage:\t %s port_number\n", argv[0]);
		exit(1);
	}

	if(port_number > 65535) {
		fprintf(stderr, "Maximum port number is 65535\n");
		exit(1);
	}
	char *port_string = argv[1];


	// Allow graceful exit
	signal(SIGINT, terminate);


	// getaddrinfo about my machine
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));	// any protocol, addrlen, addr, canonname, or next
	hints.ai_family = AF_UNSPEC;		// Don't care about IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;	// TCP stream sockets
	hints.ai_flags = AI_PASSIVE;		// Fill in my IP address

	getaddrinfo(NULL, port_string, &hints, &res);

	/*
	int socket( int domain, - PF_INET or PF_INET6
				int type,	- SOCK_STREAM or SOCK_DGRAM
				int protocol - use 0 or getprotobyname("tcp")->p_proto - pulls from /etc/protocols 
				);
	*/
	int socket_fd = socket(PF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto);

	if(socket_fd == -1) {
		fprintf(stderr, "Problem creating socket.\n");
		exit(1);
	}

	// Attach the socket to a TCP port.
	// int bind(int socket_file_descriptor, struct sockaddr *my_addr, int addrlen);
	if(-1 == bind(socket_fd, res->ai_addr, res->ai_addrlen)) {
		fprintf(stderr, "Problem binding socket.\n");
		exit(1);
	}

	// Set the socket to 'recieve calls'
	// int listen(int socket_fd, int backlog);
	if(0 != listen(socket_fd, 5)) {
		fprintf(stderr, "Problem listening to socket.\n");
		exit(1);
	}

	// Start up a bunch of threads!
	pthread_t threads[THREAD_COUNT];
	int i;
	for(i=0; i<THREAD_COUNT; i++) {
		pthread_create(threads+i, NULL/*flags*/, &server_worker, (void *) (long) socket_fd);
	}

	printf("Started server at on port %d\n", port_number);
	char date[100];
	getDate(date, 100);
	printf("%s", date);

	while(shouldContinue()) {
		// pthread_yield(); // nonstandard
		sched_yield();
	}
	for(i=0; i<THREAD_COUNT; i++) {
		pthread_cancel(threads[i]);
	}
	for(i=0; i<THREAD_COUNT; i++) {
		pthread_join(threads[i], NULL);
	}
	close(socket_fd);

	return 0;
}

volatile sig_atomic_t end_program = 0;
// Starts the server shutdown process.
void terminate(int signum) {
	end_program = 1;
	printf("Shutting server down...\n");
}

// Returns whether the server should be shutting down.
int shouldContinue() {
	return !end_program;
}

// Runs a thread on the server.
void *server_worker(void *socket_fd_void) {
	int socket_fd =(int) (long) socket_fd_void;
	while(shouldContinue()) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int connection_socket_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &client_addr_len);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		if(connection_socket_fd <= 0) {
			fprintf(stderr, "Error accepting connection to client!\n");
		}		

		char filepath[256];
		getFilepath(connection_socket_fd, filepath, 256);

		if(strcmp(filepath, "./") && strcmp(filepath, "./index") && strcmp(filepath, "./index.html")) {
			sendFile(filepath, connection_socket_fd);
		} else { // Send the default file!
			sendFile(DEFAULT_FILE, connection_socket_fd);
		}

		close(connection_socket_fd);

		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	}

	pthread_exit(socket_fd_void);
}

// Extracts the filepath from the http header.
// Just discards the rest of the header.
int getFilepath(int connection_socket_fd, char *filepath_buffer, int filepath_buffer_length) {
	char buffer[BUFFER_SIZE];
	filepath_buffer[0] = '\0';

	int bytes_read = 0, current_read;
	do {
		current_read = recv(connection_socket_fd, buffer, BUFFER_SIZE - 1, 0/*flags*/);
		
		if(filepath_buffer[0] == '\0') {
			// Get the second 'word': e.g. the / from GET / HTTP 1.1
			int i=0, j;
			while(buffer[i] != ' ' && i < current_read) { i++; }
			j = ++i;
			while(buffer[j] != ' ' && buffer[j] != '?' && j < current_read) { j++; }
			buffer[j] = '\0';

			filepath_buffer[0] = '.'; // Make paths relative!
			strncpy(filepath_buffer+1, buffer+i, filepath_buffer_length-1);
		}

		bytes_read += current_read;
		// We don't care what we receive, so we're ignoring the bytes we read.
		// printf("\nRead %d bytes: \n%s\n", bytes_read, buffer);
	} while(current_read > 0 && !(current_read < BUFFER_SIZE-1));
	return bytes_read;
}

// Sends the filepath to the socket, including the header.
int sendFile(char *filepath, int socket_fd) {

	FILE *file;
	char buffer[CHUNK_SIZE];
	struct stat file_stats;
	int fd, bytes_read, bytes_written, total_bytes;

	// Get file size
	if(stat(filepath, &file_stats) != 0) {
		fprintf(stderr, "Failed to determine size of %s: %s\n", filepath, strerror(errno));

		send404Error(socket_fd);
		
		return -1; // File may not exist!
	}

	// Send header
	total_bytes = sendHttpHeader(socket_fd, file_stats.st_size);

	// Open file
	file = fopen(filepath, "rb");
	if(file == NULL) {
		fprintf(stderr, "Unable to open file %s.\n", filepath);
		return -1;
	}

	// Send chunks from file to socket.
	fd = fileno(file);
	do {
		bytes_read = read(fd, buffer, CHUNK_SIZE);
		bytes_written = write(socket_fd, buffer, bytes_read);
		total_bytes += bytes_written;
		if(bytes_read < 0 || bytes_read != bytes_written) {
			fprintf(stderr, "Error transferring file.\n");
			return -1;
		}
	} while(bytes_read > 0);
	
	close(fd);

	return total_bytes;
}

// Sends an http header.
int sendHttpHeader(int socket_fd, int content_length) {
	char http_header[128]; // Original string is 65 characters.

	snprintf( http_header, 128,
		"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\nConnection: close\r\n\r\n",
		content_length);

	int bytes_written = write(socket_fd, http_header, strlen(http_header));
	if(bytes_written < 0) {
		fprintf(stderr, "Problem writing outgoing message!\n");
		return bytes_written;
	}
	return bytes_written;
}

int send404Error(int socket_fd) {
	char message[256] = 
	"HTTP/1.1 404 Not found\r\n"
	"Content-Type: text/html\r\n"
	"Content-Length: 44\r\n"
	"Connection: close\r\n";
	char date[100];
	getDate(date, 98);
	strcat(date, "\r\n");
	strcat(message, date);
	strcat(message, "<html><body>File not found :-(</body></html>");
	
	int bytes_written = write(socket_fd, message, strlen(message));
	if(bytes_written != strlen(message)) {
		fprintf(stderr, "Problem writing outgoing message!\n");
	}
	return bytes_written;

}

// Adds a HTTP header to the buffer and sends it over the network.
int sendHttp(int connection_socket_fd, char *buffer) {
	int buffer_length = strlen(buffer);

	char http_header[128]; // Original string is 65 characters.

	snprintf( http_header, 128,
		"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\nConnection: close\r\n\r\n",
		buffer_length//Content-length: "the size of the entitiy body in decimal number of OCTETs..." - RFC2616
		);

	
	int bytes_written = write(connection_socket_fd, http_header, strlen(http_header));
	
	if(bytes_written < 0) {
		fprintf(stderr, "Problem writing outgoing message!\n");
		return bytes_written;
	}
	int n = write(connection_socket_fd, buffer, buffer_length);
	if(n < 0) {
		fprintf(stderr, "Problem writing outgoing message!\n");
		return n;
	}

	return bytes_written + n;
}
