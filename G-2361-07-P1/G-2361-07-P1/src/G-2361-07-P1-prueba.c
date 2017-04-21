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
#include "../includes/G-2361-07-P1-Daemon.h"
#include "../includes/G-2361-07-P1-Server.h"




int main(int argc,char * argv[]){
	int dm;
	char *path, buffer[80];

	dm  = daemonizar (argv[0]);

	if(dm == -1){
		return -1;
	}

	path = realpath("motd.txt", buffer);
	servidor(PUERTO, path);

	return 0;

}



