/** 
 * @file servidor.h
 * @brief Prototipos de funciones que implementan el servidor IRC.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * 
 */

#ifndef _SERVIDOR_H_
#define _SERVIDOR_H_

#include "../includes/daemon.h"
#include "../includes/conexion_tcp.h"

#define PORT_LIMIT_FLOOR 1024 /**< Por debajo de 1024 requiere 'sudo' */

/** 
 * @name main
 *
 * @brief Funcion para ejecutar el servidor
 * 
 * @param int argc: numero de parametros de entrada
 * @param char * argv[]: parametros de entrada
 *  
 *
 * @return OK si todo ha salido bien, ERROR si hay algun fallo
 *
 */
int main (int argc, char * argv[]);

#endif