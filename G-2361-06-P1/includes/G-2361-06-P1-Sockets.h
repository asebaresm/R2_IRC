#ifndef _SOCKET_H
#define _SOCKET_H


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <redes2/irc.h>


#define  NUM_SOCKETS 100 /**< Numero de sockects maximo*/
#define  SIZE 8192 /**< Tamanno para la entrada*/


int abrir_socketTCP(int puerto);

int acepta_conexion(int IDsocket);


#endif
