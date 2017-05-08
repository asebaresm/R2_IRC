/**
 * @file G-2361-06-P1-Functions.h
 * @brief Declaracion de funciones, definición de tipos: implementacion de ciertas funciones IRC que implementa el servidor IRC.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 13/02/17
 *
 */
#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

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
#include "../includes/G-2361-06-P1-Server.h"

/**
 * @page liberaLista liberaLista
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Libera la memoria de una lista.
 *
 * @description 
 * Libera una lista que ha sido reservada.
 *
 * @parameters
 * @param[in] lista: lista que se quiere liberar.
 * @param[in] nElems: numero de elementos que componen la lista.
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
void liberaLista(char** lista, long nElems);

/**
 * @page funcionNick funcionNick
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Realiza toda la funcionalidad correspondiente al nick de un usuario.
 *
 * @description 
 * Esta funcion se encarga de modificar el nick de un usuario, modificando el 
 * campo nick_name por el campo nick. Se modifica y este seria el nuevo nick
 * del usuario actual.
 *
 * @parameters
 * @param[in] id: identificador del usuario.
 * @param[in] usuario: nombre del usuario.
 * @param[in] nick_name: nick actual del usuario.
 * @param[in] real: realname del usuario.
 * @param[in] nick: nick nuevo.  
 * @param[in] IDsocket: el numero del identificador del socket.
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
void funcionNick(long id, char* usuario, char* nick_name, char* real, char* nick, int IDsocket);

/**
 * @page funcionUser funcionUser
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Realiza toda la funcionalidad correspondiente a crear un 
 *		  nuevo usuario.
 *
 * @description 
 * Imprime toda la informacion al conectarse un nuevo usuario.
 *
 * @parameters
 * @param[in] user: nombre del nuevo usuario.
 * @param[in] nick: nick del nuevo usuario.
 * @param[in] realname: realname del nuevo usuario.
 * @param[in] modehost: 
 * @param[in] IDsocket: el numero del identificador del socket.
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
void funcionUser(char *user, char *nick, char *realname, char *modehost, int IDsocket);

/**
 * @page funcionPing funcionPing
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Realiza toda la funcionalidad correspondiente con el comando ping.
 *
 * @description 
 * Esta funcion se encarga de cuando se realiza el comando ping, devuelva un pong.
 * Este mensaje se mostraria cada 30 segundos.
 *
 * @parameters
 * @param[in] server1: Servidor 1.
 * @param[in] server2: servidor 2.
 * @param[in] IDsocket: el numero del identificador del socket.
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
void funcionPing(char *server1, char *server2, int IDsocket);

/**
 * @page funcionAway funcionAway
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Realiza toda la funcionalidad correspondiente con el comando away.
 *
 * @description 
 * Esta funcion se encarga de dejar ausente a un usuario.
 *
 * @parameters
 * @param[in] id: identificador del usuario.
 * @param[in] usuario: nombre del usuario.
 * @param[in] nick_name: nick actual del usuario.
 * @param[in] real: realname del usuario.
 * @param[in] away: away del usuario.
 * @param[in] msg: mensaje que se quiere mostrar.  
 * @param[in] IDsocket: el numero del identificador del socket.
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
void funcionAway(long id, char *usuario, char *nick_name, char *real, char *away, char *msg, int IDsocket);

/**
 * @page funcionPart funcionPart
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Realiza toda la funcionalidad correspondiente a abandonar un canal.
 *
 * @description 
 * Esta funcion se encarga de que un usuario abandone un canal y en el caso
 * de que no queden mas usuarios eliminar el canal. 
 *
 * @parameters
 * @param[in] channel: canal del que se quiere salir el usuario.
 * @param[in] nick_name: nick del usuario que quiere abandonar el canal.
 * @param[in] msg: mensaje que se va a mandar.
 * @param[in] IDsocket: el numero del identificador del socket.
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
void funcionPart(char *channel, char *nick_name, char *msg, int IDsocket);

/**
 * @page funcionList funcionList
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Realiza toda la funcionalidad correspondiente a listar los canales.
 *
 * @description 
 * Esta funcion se encarga de listar los canales creados. 
 *
 * @parameters
 * @param[in] nick: nick del usuario actual.
 * @param[in] target: mascara.
 * @param[in] IDsocket: el numero del identificador del socket.
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
void funcionList(char *nick, char *target, int IDsocket);

/**
 * @page funcionMotd funcionMotd
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Realiza toda la funcionalidad correspondiente al mensaje del dia.
 *
 * @description 
 * Esta funcion se encarga de abrir un fichero y mostrar por pantalla el 
 * mensaje del dia. 
 *
 * @parameters
 * @param[in] nick_name: nick del usuario actual.
 * @param[in] target: 
 * @param[in] IDsocket: el numero del identificador del socket.
 * @param[in] fich: el nombre del fichero que se va a leer.
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
void funcionMotd(char *nick_name, char *target, int IDsocket, char *fich);

#endif