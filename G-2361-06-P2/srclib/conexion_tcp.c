/**
 * @file conexion_tcp.c
 * @brief Implementacion de la liberia para conexiones TCP
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @date 13/02/17
 *
 */

 /* -- Includes -- */
#include "../includes/conexion_tcp.h"

int crearConexion(int portno, struct sockaddr_in* server){
	int socket_desc;
	int optval; 						/* flag value for setsockopt */
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		logERR("No se ha podido crear socket");
		return ERR;
	}

	logEvent("Socket creado");
	
	optval = 1;

	setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

	//Prepare the sockaddr_in structure
	memset((char *) server, 0, sizeof(*server));
	server->sin_family = AF_INET;
	server->sin_addr.s_addr = INADDR_ANY;
	server->sin_port = htons((unsigned short)portno );
	 
	//Bind
	if( bind(socket_desc,(struct sockaddr *)server , sizeof(*server)) < 0)
	{
		logERR("bind failed");
		return ERR;
	}
	logEvent("bind done");
	 
	//Listen
	listen(socket_desc , MAX_CON_REQ);
	 
	//Accept and incoming connection
	logEvent("Esperando conexiones entrantes...");
	
	return socket_desc;
}

int recvDatos(int client_sock, char* client_message, int cm_size, char *hostaddrp){
	int read_size;
	char buf[AUX_SBUF];

	read_size = recv(client_sock , client_message , cm_size , 0);

	if (read_size == 0){
		sprintf(buf,"recv(): leido un tam 0 bytes de %s", hostaddrp);
		logEvent(buf);
		return 0;

	}else if(read_size < 0){
		sprintf(buf,"recv(): leido un tam -1 bytes de %s, error", hostaddrp);
		logERR(buf);
		return ERR;

	}else{
		//if(client_message[strlen(client_message) - 2] == 13) //check si es CR,LF
		//	client_message[strlen(client_message) - 2] = '\0';
		sprintf(buf, "recv(): \"%s\" (%d Bytes) de %s", client_message, read_size, hostaddrp);
		logEvent(buf);
	}

	return read_size;
}

//ssize_t write(int fd, const void *buf, size_t count);
//ssize_t send(int sockfd, const void *buf, size_t len, int flags);

int enviarDatos(int client_sock, char* message, int message_size){
	int sent_size;
	char buf[AUX_SBUF];

	sent_size = send(client_sock, message, message_size, 0);

	if (sent_size == 0){
        logEvent("send(): enviado un tam 0 bytes");
		return 0;

    }else if(sent_size < 0){
		logERR("send(): enviado un tam -1 bytes, error");
		return ERR;

	}else{
		//printf("\nYEE '%d', '%d', '%d'\n", message[strlen(message) - 3], message[strlen(message) - 2], message[strlen(message) - 1]);
		//if(message[strlen(message) - 2] == 13) //check si es CR,LF
		//	message[strlen(message) - 2] = '\0';
		sprintf(buf, "send(): \"%s\" (%d Bytes)", message, sent_size);
		logEvent(buf);
	}

    return sent_size;
}

int cerrarConexion(int client_sock, char* hostaddrp){
	int sent_size;
	char buf[AUX_SBUF];

	if(close(client_sock) < 0){
    	logERR("Error al cerrar la conexion");
    	return ERR;

    }else{
    	if (hostaddrp != NULL)
			sprintf(buf, "conexion_tcp.c: conexion cerrada con %s", hostaddrp);
		else
			sprintf(buf, "conexion_tcp.c: conexion cerrada");
		logEvent(buf);
    }

    return OK;
}