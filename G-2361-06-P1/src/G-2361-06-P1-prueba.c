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
#include "../includes/G-2361-06-P1-Daemon.h"
#include "../includes/G-2361-06-P1-Server.h"




int main(int argc,char * argv[]){
	int dm, socket;
	char *path, buffer[80];

	if (argc != 2){
		printf("\nERROR - Uso ./<ejecutable> <numero_de_puerto>\n");
		return EXIT_FAILURE;
	}

	socket = atoi(argv[1]);
	if(socket < 1024){
		printf("\nERROR - Numero de puerto en el espacio de puertos del sistema.\n");
		return EXIT_FAILURE;
	}
	
	path = realpath("motd.txt", buffer);
	/*syslog(LOG_INFO,"DIRECTORIO %s", path);*/
	
	
	dm  = daemonizar (argv[0]);
	if(dm == -1){
		return -1;
	}

	//servidor(PUERTO, path);
	servidor(socket, path);

	return 0;

}



