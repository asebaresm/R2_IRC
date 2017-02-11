/** 
 * @brief Funciones para controlar el manejo del servidor
 * 
 * @file servidor.h
 * @author Alfonso Sebares y Beatriz de Pablo
 * 
 */
#ifndef _SERVIDOR_H_
#define _SERVIDOR_H_

#include "../includes/daemon.h"

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