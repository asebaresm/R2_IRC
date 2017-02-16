/*
	C socket server example
*/

#include <stdio.h>
#include <unistd.h> /*write*/
#include <stdlib.h>
#include <string.h> /*strlen*/
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> /*inet_addr*/
#include <pthread.h>   /*for threading , link with lpthread*/
#include <errno.h>	   /*for strerror() use*/

#define BUFSIZE 1024
#define MAX_CON_REQ 10 /*Max of active connection requests (e.g. 11th fails if set to 10)*/

void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , read_size;
	int portno;
	int optval; /* flag value for setsockopt */
	struct sockaddr_in server , client;
	struct hostent *hostp; /* client host info */
	char client_message[BUFSIZE];
	char *hostaddrp; /* dotted decimal host addr string */
	 
	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}
	portno = atoi(argv[1]);
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	optval = 1;
	setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

	//Prepare the sockaddr_in structure
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons((unsigned short)portno );
	 
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		//printf("\nstrerror: %s\n", strerror(errno));
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	 
	//Listen
	listen(socket_desc , MAX_CON_REQ);
	 
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	 
	//accept connection from an incoming client
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}

	hostp = gethostbyaddr((const char *)&client.sin_addr.s_addr, sizeof(client.sin_addr.s_addr), AF_INET);
	if (hostp == NULL)
		error("ERROR on gethostbyaddr");

	hostaddrp = inet_ntoa(client.sin_addr);

	if (hostaddrp == NULL)
		error("ERROR on inet_ntoa\n");
	printf("\nserver established connection with %s (%s)\n", hostp->h_name, hostaddrp);
	 
	//Receive a message from client
	while( strcmp(client_message, "_STOP_") != 0 )
	{
		read_size = recv(client_sock , client_message , BUFSIZE , 0);
		printf("\nRecibido: %s", client_message);
		if (read_size < 0){
			perror("recv failed");
		}
		//Send the message back to client
		//write(client_sock , client_message , read_size);
		/*afinar un poco mas que mandar siempre BUFSIZE:*/
		write(client_sock , client_message , strlen(client_message)+1); 
	}
	
	puts("Client disconnected");
	fflush(stdout);
	close(client_sock);
	 
	return 0;
}