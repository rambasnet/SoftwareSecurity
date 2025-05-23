#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "./util/utility.h"
#include "./util/net_utility.h"

enum REQUEST_TYPE {GET, POST, HEAD, UNKNOWN};

using namespace std;

#define PORT 80   // the port users will be connecting to
#define WEBROOT "./webroot" // the web server's root directory

void handle_connection(int, struct sockaddr_in *); // handle web requests
int get_file_size(int); // returns the filesize of open file descriptor
void send_file_not_found(const int); // send file not found error
void send_file(const int, char *, unsigned int); // send file/resource requested
void send_buffer_address(const int, char *); // send buffer address

int main(void) {
   int sockfd, new_sockfd, yes=1; 
   struct sockaddr_in host_addr, client_addr;   // my address information
   socklen_t sin_size;

   printf("Accepting web requests on port %d\n", PORT);

   if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
      fatal("in socket");

   if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
      fatal("setting socket option SO_REUSEADDR");

   host_addr.sin_family = AF_INET;      // host byte order
   host_addr.sin_port = htons(PORT);    // short, network byte order
   host_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
   memset(&(host_addr.sin_zero), '\0', 8); // zero the rest of the struct

   if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
      fatal("binding to socket");

   if (listen(sockfd, 20) == -1)
      fatal("listening on socket");

   while(1) {   // Accept loop
      sin_size = sizeof(struct sockaddr_in);
      new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
      if(new_sockfd == -1)
         fatal("accepting connection");

      handle_connection(new_sockfd, &client_addr);
   }
   return 0;
}

/* This function handles the connection on the passed socket from the
 * passed client address.  The connection is processed as a web request
 * and this function replies over the connected socket.  Finally, the 
 * passed socket is closed at the end of the function.
 */
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr) {
	char *ptr, request[500];
	int length = 0;
	REQUEST_TYPE req_type = UNKNOWN;
	
	char address[100];
	snprintf(address, sizeof(address), "Request buffer @ %p\r\n", request);
	printf("%s", address);
	// receive one line from client and store it into request buffer
	length = recv_line(sockfd, request);

	printf("Got request from %s:%d \"%s\"\n", inet_ntoa(client_addr_ptr->sin_addr), ntohs(client_addr_ptr->sin_port), request);
	printf("Received: %d bytes\n", length);

	ptr = strstr(request, " HTTP/"); // search for valid looking request
	if(ptr == NULL) { // then this isn't valid HTTP
		printf(" NOT HTTP!\n");
	} 
	else {
		*ptr = 0; // terminate the buffer at the end of the URL
		ptr = NULL; // set ptr to NULL (used to flag for an invalid request)
		if(strncmp(request, "GET ", 4) == 0) {  // get request
			ptr = request+4; // ptr is the URL=
			req_type = GET;
		}
		if(strncmp(request, "HEAD ", 5) == 0) { // head request
		 	ptr = request+5; // ptr is the URL
		 	req_type = HEAD;
		}

		if(req_type == UNKNOWN) { //this is not a recognized request
			printf("\tUNKNOWN REQUEST!\n");
		} 
		else if (req_type == GET) { // valid request, with ptr pointing to the resource name
		    if (strncmp(ptr, "/buffer", 7) == 0)
		        send_buffer_address(sockfd, address);
		    else
				send_file(sockfd, ptr, sizeof(request));
	    }
   } // end if block for valid HTTP
   shutdown(sockfd, SHUT_RDWR); // close the socket gracefully
}

/* This function accepts an open file descriptor and returns     
 * the size of the associated file.  Returns -1 on failure.
 */
int get_file_size(int fd) {
	struct stat stat_struct;

	if(fstat(fd, &stat_struct) == -1)
		return -1;
	return (int) stat_struct.st_size;
}

void send_file_not_found(const int sockfd) {
	printf(" 404 Not Found\n");
	send_string(sockfd, "HTTP/1.1 404 NOT FOUND\r\n");
	send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
	send_string(sockfd, "<html><head><title>404 Not Found</title></head>");
	send_string(sockfd, "<body><h1>URL not found</h1></body></html>\r\n");
}

void send_file(const int sockfd, char *ptr, unsigned int ptr_size) {
	// handle GET request
	char resource[500];
	int fd, length;

	if (ptr[strlen(ptr) - 1] == '/')  // for resources ending with '/'
		strncat(ptr, "index.html", ptr_size); // add 'index.html' to the end
	strncpy(resource, WEBROOT, sizeof(resource)-1); // begin resource with web root path
	strncat(resource, ptr, sizeof(resource)-1); //  and append resource path

	fd = open(resource, O_RDONLY, 0); // try to open the file
	printf("\tOpening \'%s\'\t", resource);

	if(fd == -1) { // if file is not found
		send_file_not_found(sockfd);
	} 
	else {      // otherwise, serve up the file
		printf(" 200 OK\n");
		send_string(sockfd, "HTTP/1.1 200 OK\r\n");
		send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
		
		if( (length = get_file_size(fd)) == -1)
			fatal("getting resource file size");

		ptr = (char *) malloc(length);
		if(ptr == NULL)
			fatal("allocating memory for reading resource");

		read(fd, ptr, length); // read the file into memory
		send(sockfd, ptr, length, 0);  // send it to socket
		free(ptr); // free file memory
		close(fd); // close the file
	}
}

void send_buffer_address(const int sockfd, char *ptr) {
    printf(" 200 OK\n");
	send_string(sockfd, "HTTP/1.1 200 OK\r\n");
	send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
	send_string(sockfd, "<html><head><title>Request Buffer Address</title></head>");
	char response[200];
	snprintf(response, sizeof(response), "<body><p style=\"color: red;\">%s</p></body></html>\r\n", ptr);
	send_string(sockfd, response);
}
