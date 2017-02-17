/**
 * @file conexion_tcp.h
 * @brief Prototipos de funcion de la liberia para conexiones TCP.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @date 13/02/17
 *
 */

#ifndef conexion_tcp_h
#define conexion_tcp_h 

/* -- Includes -- */
#include "types.h"
#include "logger.h"

#include <unistd.h> /*write*/
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> /*inet_addr*/
#include <pthread.h>   /*for threading , link with lpthread*/

#define BUFSIZE 1024   /**< Tam. max. del buffer que se lee */
#define AUX_SBUF 128   /**< Tam. max. del buffer auxiliar para logeo de eventos */
#define MAX_CON_REQ 10 /**< Max. de peticiones de conexion activas (e.g. la 11 falla si puesto a 10) */


int crearConexion(int portno, struct sockaddr_in* server);

int recvDatos(int client_sock, char* client_message, int cm_size, char *hostaddrp);

int enviarDatos(int client_sock, char* message, int message_size);
#endif