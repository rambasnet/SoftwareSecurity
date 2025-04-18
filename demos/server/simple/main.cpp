#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "./util/utility.h"


#define PORT 7890	// the port users will be connecting to

using namespace std;

int main(void) {
	int sockfd, new_sockfd;  // listen on sock_fd, new connection on new_fd
	struct sockaddr_in host_addr, client_addr;	// my address information
	socklen_t sin_size;
	int recv_length=1, yes=1;
	char buffer[1024];

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		fatal("in socket");

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		fatal("setting socket option SO_REUSEADDR");
	
	host_addr.sin_family = AF_INET;		 // host byte order
	host_addr.sin_port = htons(PORT);	 // short, network byte order
	host_addr.sin_addr.s_addr = INADDR_ANY;  // automatically fill with my IP
	memset(&(host_addr.sin_zero), '\0', 8);  // zero the rest of the struct

	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
		fatal("binding to socket");

	if (listen(sockfd, 5) == -1)
		fatal("listening on socket");

	while(1) {    // Accept loop
        	printf("Server running on: %s:%d\n", inet_ntoa(host_addr.sin_addr), ntohs(host_addr.sin_port));
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
		if(new_sockfd == -1)
			fatal("accepting connection");
		printf("server: got connection from %s port %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		
		char address[100];
		sprintf(address, "Buffer @ %p\n", buffer);
		printf("%s", address);
		
		// send Hello there... to client
		send(new_sockfd, "Hello there!\n", 13, 0);
		
		// receive upto 1024 bytes and store into buffer
		recv_length = recv(new_sockfd, buffer, 1024, 0);
		while(recv_length > 0 and strncmp(buffer, "bye", 3) != 0) {
			printf("RECV: %d bytes\n", recv_length);
			dump(buffer, recv_length);
			if (strncmp(buffer, "buffer ?", 8) == 0)
				send(new_sockfd, address, strlen(address), 0);
			else
				send(new_sockfd, buffer, recv_length, 0);
	    		// receive upto 1024 bytes
			recv_length = recv(new_sockfd, &buffer, 1024, 0);
		}
		close(new_sockfd);
	}
	return 0;
}

