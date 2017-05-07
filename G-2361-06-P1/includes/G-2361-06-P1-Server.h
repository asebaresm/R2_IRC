#ifndef _SERVER_H
#define _SERVER_H

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

#define PREFIJO "servidor-P6" /**< Nombre del prefijo*/
#define SIZE 8192 /**< Tamanno para la entrada*/
#define NUM_SOCKETS 100 /**< Numero de sockets maximo*/


/**
 * @page manejador_alarma manejador_alarma
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Funcion que se llama cada 30 segundos para que muestre
 * el mensaje de PING.
 *
 * @description 
 * Esta funcion se llama cada 30 segundos con una alarma para 
 * mostrar el mensaje de PING. 
 *
 * @parameters
 * @param[in] sig: la sennal.
 *
 * @return
 * No devuelve nada
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
void manejador_alarma(int sig);

/**
 * @page procesar procesar
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Funcion que recibe los comandos del cliente.
 *
 * @description 
 * Esta funcion recibe y separa los comandos que manda el cliente.
 *
 * @parameters
 * @param[in] *entrada: los comandos enviados por el cliente.
 * @param[in] IDsocket: numero del socket.
 * @param[in] readset: la lectura.
 *
 * @return
 * No devuelve nada
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
void procesar(char* entrada, int IDsocket, fd_set readset);

/**
 * @page servidor servidor
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Funcion que se encarga de iniciar el servidor.
 *
 * @description 
 * Esta funcion realiza la apertura del socket y permite atender
 * las distintas peticiones de los clientes.
 *
 * @parameters
 * @param[in] puerto: numero del puerto.
 * @param[in] path: direccion del fichero del mensaje del dia.
 *
 * @return
 * No devuelve nada
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
void servidor(int puerto, char* path);

/**
 * @page parseCommand parseCommand
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Funcion en la que dependiendo del comando realiza una accion u otra.
 *
 * @description 
 * Esta funcion recibe un identificador que dependiendo de cual sea, permitira
 * realizar una accion determinada.
 *
 * @parameters
 * @param[in] query: identificador del comando.
 * @param[in] comando: cadena a procesar.
 * @param[in] IDsocket: identificador del socket.
 *
 * @return
 * No devuelve nada
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
void parseCommand(long int query, char* comando, int IDsocket);


#endif