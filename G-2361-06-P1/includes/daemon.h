/** 
 * @brief Funcion para poder demonizar un servidor
 * 
 * @file daemon.h
 * @author Alfonso Sebares y Beatriz de Pablo
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