
#include "../includes/G-2361-06-P1-Sockets.h"


int abrir_socketTCP(int puerto){
	int IDsocket, opt = 1;
	struct sockaddr_in addr;

	printf("Creando socket.... \n");
	IDsocket = socket(AF_INET, SOCK_STREAM, 0);

	if (IDsocket < 0){
		syslog(LOG_ERR, "Error en la creacion del socket");
		/*printf("Error en la creacion del socket \n");*/
		return EXIT_FAILURE;
	}
		//setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
	if (setsockopt(IDsocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		syslog(LOG_ERR, "Error - setsockopt (SO_REUSEADDR)");
		printf("\nError - setsockopt (SO_REUSEADDR)");
		/*printf("Error - setsockopt (SO_REUSEADDR)");*/
		close(IDsocket);
		return EXIT_FAILURE;
	}
	memset(&addr, 0, sizeof(struct sockaddr_in));
	bzero(&addr, sizeof(addr));

	addr.sin_family = AF_INET; /* familia TCP/IP*/
	addr.sin_port = htons(puerto); /*Asignacion de puerto*/
	addr.sin_addr.s_addr = htonl(INADDR_ANY); /*Aceptacion de todas las direcciones*/

	if (bind (IDsocket, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		syslog(LOG_ERR, "Error al nombrar socket");
		/*printf("Error al nombrar el socket \n");*/
		return EXIT_FAILURE;
	}

	printf("Servidor activo escuchando\n");
	if (listen (IDsocket, NUM_SOCKETS) < 0){
		syslog(LOG_ERR, "Error al escuchar");
		/*printf("Error al escuchar \n");*/
		return EXIT_FAILURE;
	}
	return IDsocket;
}


int acepta_conexion(int IDsocket){
	int descriptor;
	socklen_t  len;
	struct sockaddr conexion;

	if ((descriptor = accept(IDsocket, &conexion, &len)) < 0){
		syslog(LOG_ERR, "Error al aceptar conexion");
		
		//printf("Error al aceptar conexion \n");
		return EXIT_FAILURE;
	}
	return descriptor;
}





