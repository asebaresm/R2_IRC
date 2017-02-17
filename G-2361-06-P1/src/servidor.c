/**
 * @file servidor.c
 * @brief Implementacion del servidor IRC.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @date 13/02/17
 *
 */

 /* -- Includes -- */
#include "../includes/servidor.h"

/** 
 * @brief Main para ejecutar el servidor
 * 
 * @param int argc: numero de parametros de entrada
 * @param char * argv[]: parametros de entrada
 *  
 *
 * @return OK si todo ha salido bien, ERROR si hay algun fallo
 *
 */
int main (int argc, char * argv[]){

	int portno;
	/*
	if(daemonizar (argv[1]) == ERROR){
		syslog(LOG_ERR, "Error al daemonizar");
		exit(EXIT_FAILURE);
		return ERROR;
	}*/

	if (argc != 2) {
		fprintf(stderr, "uso: %s <puerto>\n", argv[0]);
		logERR("numero de argumentos invalido");
		return EXIT_FAILURE;
	}

	portno = atoi(argv[1]);
	if (portno < PORT_LIMIT_FLOOR){
		logERR("puerto por debajo de 1024 no valido");
		return EXIT_FAILURE;
	}
	
	return OK;
}