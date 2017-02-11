/** 
 * @brief Controla el manejo del servidor
 * 
 * @file servidor.c
 * @author Alfonso Sebares y Beatriz de Pablo
 * 
 */
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

	if(daemonizar (argv[1]) == ERROR){
		syslog(LOG_ERR, "Error al daemonizar");
		exit(EXIT_FAILURE);
		return ERROR;
	}

	syslog(LOG_INFO, "Se ha creado el demonio");

	while (1);
	
	return OK;
}