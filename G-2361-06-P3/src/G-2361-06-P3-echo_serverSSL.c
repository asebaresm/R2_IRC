/** 
 * @file echo_server.c
 * @brief Servidor echo utilizado para comprender y desarrollar el servidor IRC con protocolo SSL.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 28/04/17
 * 
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
#include "../includes/G-2361-06-P3-funciones_ssl.h"

#define BUFSIZE 1024   /**< Tam. max. del buffer que se lee*/
#define MAX_CON_REQ 10 /**< Max. de peticiones de conexion activas (e.g. la 11 falla si puesto a 10)*/

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
	SSL_CTX* ctx=NULL;
	SSL * ssl=NULL;
	 
	portno = 8888;
	
	inicializar_nivel_SSL();
	ctx=fijar_contexto_SSL(SERVER_CERT, SERVER_KEY);

	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		perror("Could not create socket");
		return 1;
	}
	
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
	 
	//Listen
	listen(socket_desc , MAX_CON_REQ);

	//Accept and incoming connection
	c = sizeof(struct sockaddr_in);
	 
	//accept connection from an incoming client
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}

	hostp = gethostbyaddr((const char *)&client.sin_addr.s_addr, sizeof(client.sin_addr.s_addr), AF_INET);
	if (hostp == NULL){
		perror("ERROR on gethostbyaddr");
		return 1;
	}

	hostaddrp = inet_ntoa(client.sin_addr);

	if (hostaddrp == NULL){
		perror("ERROR on inet_ntoa\n");
		return 1;
	}
	printf("\nserver established connection with %s (%s)\n", hostp->h_name, hostaddrp);
	
	ssl=aceptar_canal_seguro_SSL(ctx, client_sock); 

	//Receive a message from client
	while( strcmp(client_message, "exit") != 0 )
	{

		read_size = recibir_datos_SSL(ssl, client_message);
		if (read_size < 0){
			break;
		}

		if (enviar_datos_SSL(ssl, client_message) < 0)
			break; 

		puts(client_message);
		fflush(stdout);
		
	}
	
	cerrar_canal_SSL(&ssl, &ctx, client_sock);
	fflush(stdout);
	close(client_sock);
	close(socket_desc); 

	return 0;
}
