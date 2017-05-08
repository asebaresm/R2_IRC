/**
 * @file aux_functions.h
 * @brief Declaraciones de funciones, definición de tipos: implementación de funciones auxiliares de xchat2.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 20/03/17
 *
 */
#ifndef aux_functions_h
#define aux_functions_h

#include <redes2/ircxchat.h>
#include <redes2/irc.h>

#include "conexion_tcp.h"

#define EXTRA 20				/**< espacio extra para pruebas de formato*/
#define SNAP_SIZE 9				/**< XX:XX:XX\0 */		
#define MAX_NICK_FIELD 22+EXTRA	/**< maximo del campo 'nick' en el cliente: 11([snap de tiempo]) + 2(espacio y \0) + 9(nick IRC) */

extern int sockfd_user;			/**< global con el socket que tiene abierto el cliente con el servidor IRC **/

/**
* Estructura util para el envio de ficheros
*/
typedef struct {
	char* hostname;
	char* filename;
	int port;
	long unsigned int length;
	char *data;
} File_args;

/**
 * @page mfree mfree
 *
 * @synopsis
 * @code
 * 	#include "aux_functions.h"
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
void mfree(int n, ...);
int save_file(void* args);
void interface_mostrar_nicks(char* channel, char* list);
char *strnext(char* haystack, int ch);
int parse_type(const char* message);
int parse_type2(const char* message);
void IRCInterface_WriteSystem_Pretty(char *nick, char *msg);
void IRCInterface_WriteSystemThread_Pretty(char *nick, char *msg);
void IRCInterface_WriteChannelThread_Pretty(char *chan, char *nick, char *msg);
int testIRC_CommandQuery(char* message);
int changeMode(char *channel, char *nick, char *mode);

#endif
