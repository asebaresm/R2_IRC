/** 
 * @file daemon.h
 * @brief Prototipo de funciones para daemonizar el servidor IRC.
 * 
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * 
 */
#ifndef _DAEMON_H_
#define _DAEMON_H_

/* Includes */
#include <redes2/irc.h>
#include <syslog.h>
#include <sys/stat.h>

/* Defines */
#define OK 0
#define ERROR -1

/** 
 * @name daemonizar
 *
 * @brief Funcion para demonizar a un servicio, crear proceso, unirlo al init e inhabilitar la terminal
 * 
 * @param char * servicio: Nombre del servidor para dejarlo en segundo plano
 *  
 * @return OK si todo ha salido bien, ERROR si hay algun fallo
 *
 */
int daemonizar (char * servicio);

#endif