/**
 * @file G-2361-06-P1-Daemon.h
 * @brief Declaracion de funciones, definición de tipos: implementacion del daemon utiliza el servidor para ejecutarse en segundo plano
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 13/02/17
 *
 */
#ifndef _DAEMON_H
#define _DAEMON_H

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

/**
 * @page daemonizar daemonizar
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Funcion que permite que un proceso se ejecute en segundo plano.
 *
 * @description 
 * Esta funcion permite a un proceso estar ejecutandose en segundo plano. 
 *
 * @parameters
 * @param[in] sig: la sennal.
 *
 * @return
 * 0 si se ejecuta sin ningun problema o -1 en caso de error.
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
int daemonizar (char * servicio );

#endif