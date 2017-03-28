/**
 * @file aux_functions.h
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 20/03/17
 * @brief Declaraciones de funciones, definición de tipos: implementación de funciones auxiliares de xchat2.
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
 
void mfree(int n, ...);
int save_file(void* args);
void interface_mostrar_nicks(char* channel, char* list);
char *strnext(char* haystack, int ch);
int parse_type(const char* message);
void IRCInterface_WriteSystemThread_Pretty(char *nick, char *msg);
#endif
