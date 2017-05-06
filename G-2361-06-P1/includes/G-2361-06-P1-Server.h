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

#include "../includes/G-2361-06-P1-Sockets.h"
#define PREFIJO "servidor-P6"
#define SIZE 8192
#define NUM_SOCKETS 100
#define PUERTO 6667


void procesar(char* entrada, int IDsocket, fd_set readset);
void servidor(int puerto, char* path);
void parseCommand(long int query, char* comando, int IDsocket);
