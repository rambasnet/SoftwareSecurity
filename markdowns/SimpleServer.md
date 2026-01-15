## A Simple Echo Server Example

- servers are created using System Socket API
- server and client connect and communite over network using TCP/IP protocol
- details of OSI or TCP/IP model will be coverred in Net/App Security class
- see `demos/server/simple/` folder for a simple server example
- a server that listens for TCP connection on port 7890
- when a client connects, it sends a message *Hello there!* and receives data until it receives `bye` from client
- `main.cpp` and `util/utility.h` are the two files required to build the simple server
- use the provided Makefile to build and run the executable
- we'll use Docker to build and run the simple server for a realistic demonstration
- `Makefile` is provided to build and run the server
- run `Makefile` from within the docker


```python
! pwd
```

    /home/kali/projects/SoftwareSecurity/notebooks



```python
%cd ../demos/server/simple
```

    /home/kali/projects/SoftwareSecurity/demos/server/simple



```python
! ls
```

    docker-compose.yml  main.cpp  Makefile		 util
    Dockerfile	    main.o    simple_server.exe



```python
! cat main.cpp
```

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
    



```python
! cat util/utility.h
```

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <string>
    
    using namespace std;
    
    // A function to display an error message and then exit
    void fatal(string message) {
       char error_message[100];
    
       strcpy(error_message, "[!!] Fatal Error ");
       strncat(error_message, message.c_str(), 83);
       perror(error_message);
       exit(-1);
    }
    
    // An error checked malloc() wrapper function
    void *ec_malloc(unsigned int size) {
       void *ptr;
       ptr = malloc(size);
       if(ptr == NULL)
          fatal("in ec_malloc() on memory allocation");
       return ptr;
    }
    
    // dumps raw memory in hex byte and printable split format
    void dump(char *data_buffer, const unsigned int length) {
    	unsigned char byte;
    	unsigned int i, j;
    	for(i=0; i < length; i++) {
    		byte = data_buffer[i];
    		printf("%02x ", data_buffer[i]);  // display byte in hex
    		if(((i%16)==15) || (i==length-1)) {
    			for(j=0; j < 15-(i%16); j++)
    				printf("   ");
    			printf("| ");
    			for(j=(i-(i%16)); j <= i; j++) {  // display printable bytes from line
    				byte = data_buffer[j];
    				if((byte > 31) && (byte < 127)) // outside printable char range
    					printf("%c", byte);
    				else
    					printf(".");
    			}
    			printf("\n"); // end of the dump line (each line 16 bytes)
    		} // end if
    	} // end for
    }
    



```python
! cat Dockerfile
```

    # Use Ubuntu 22:04 image
    FROM ubuntu:22.04
    
    # Prevent interactive prompts during package installation
    ENV DEBIAN_FRONTEND=noninteractive
    
    # Update packages and install make and g++
    RUN apt update && \
        apt install -y net-tools gdb build-essential && \
        apt install -y libc6-dev gcc-multilib g++-multilib && \
        apt clean && \
        rm -rf /var/lib/apt/lists/*
    
    # Expose your desired port
    EXPOSE 7890
    
    WORKDIR /app
    
    # Default command (you can change this)
    CMD ["/bin/bash"]
    



```python
!cat docker-compose.yml
```

    services:
      server:
        build:
          context: ./ # use current folder context
          dockerfile: Dockerfile # dockerfile name
        privileged: true
        container_name: server
        hostname: server
        cap_add:
          - ALL
        volumes:
          - ./:/app
        networks:
          simple-network:
            ipv4_address: 192.168.10.10
        ports:
          - "7890:7890"
        command: sh -c "tail -f /dev/null"
    
    networks:
      simple-network:
        ipam:
          config:
            - subnet: 192.168.10.0/24
    



```python
! cat Makefile
```

    # rule for compiling program
    # Note: -m32 is required to compile into object files and the final binary
    COMPILER = g++
    COMPILER_FLAGS = -c -g -Wall -std=c++17 -m32
    BUILD_FLAGS = -m32 -fno-stack-protector -z execstack -no-pie
    
    # list .cpp files separated by space
    CPP_FILES = main.cpp
    
    # executable program name
    PROGRAM_NAME = simple_server.exe
    
    .PHONY: all
    all: build run
    	@echo "Built and running server..."
    
    # rule for compiling and building program
    # make or make all triggers the following rule
    .PHONY: build
    build:
    	# compiles .cpp to object file .o
    	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES)
    	# builds executable from object files
    	$(COMPILER) $(BUILD_FLAGS) -o $(PROGRAM_NAME) *.o
    
    # rule for running programming
    # make run triggers the following rule
    .PHONY: run
    run:
    	./$(PROGRAM_NAME)
    
    # rule for clean up
    # make clean triggers the following rule
    .PHONY: clean
    clean:
    	rm -f $(PROGRAM_NAME) *.o


## Build and Run Docker image

- installl `docker-io` and `docker-compose` if needed
- Google "how to install Docker and docker-compose in Kali" or your system
- use docker-compose to build and run the container
- Docker-compose command disables ASLR to 0 when docker image is run

```bash
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/server/simple]
└─$ docker-compose --build up -d          

┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/server/simple]
└─$ docker ps                  
CONTAINER ID   IMAGE           COMMAND                  CREATED         STATUS         PORTS                                       NAMES
28e683379fc3   simple_server   "sh -c 'tail -f /dev…"   5 minutes ago   Up 5 minutes   0.0.0.0:7890->7890/tcp, :::7890->7890/tcp   server

┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/server/simple]
└─$ docker exec -it server bash

root@server:/app# ifconfig
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.10.10  netmask 255.255.255.0  broadcast 192.168.10.255
        ether 02:42:c0:a8:0a:0a  txqueuelen 0  (Ethernet)
        RX packets 27  bytes 2124 (2.1 KB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 9  bytes 624 (624.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

root@server:/app# ls
Dockerfile  Makefile  docker-compose.yml  main.cpp  main.o  simple_server.exe  util

root@server:/app# make 
# compiles .cpp to object file .o
g++ -c -g -Wall -std=c++17 -m32 main.cpp
# builds executable from object files
g++ -m32 -fno-stack-protector -z execstack -no-pie -o simple_server.exe *.o
./simple_server.exe
Server running on: 0.0.0.0:7890

```

- use another host Terminal to ping the IP for connection

```bash                                                                                                                     
(base) ┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/server/simple]
└─$ ping 192.168.10.10 
PING 192.168.10.10 (192.168.10.10) 56(84) bytes of data.
64 bytes from 192.168.10.10: icmp_seq=1 ttl=64 time=0.127 ms
64 bytes from 192.168.10.10: icmp_seq=2 ttl=64 time=0.072 ms
64 bytes from 192.168.10.10: icmp_seq=3 ttl=64 time=0.065 ms
^C
--- 192.168.10.10 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 2051ms
rtt min/avg/max/mdev = 0.065/0.088/0.127/0.027 ms
```

- run the server on Docker Terminal server using Makefile
- connect to the Docker server at port 7890

```bash
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/server/simple]
└─$ nc 192.168.10.10 7890
Hello there!
hi
hi
buffer ?
Buffer @ 0xffffd2dc
bye
```

- shutdown all the containers

```bash
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/server/simple]
└─$ docker-compose down
Stopping server ... done
Removing server ... done
Removing network simple_simple-network
```


```python

```
