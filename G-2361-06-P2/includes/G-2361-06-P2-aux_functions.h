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

#include "G-2361-06-P2-conexion_tcp.h"

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
 *	#include "aux_functions.h"
 *
 *	void mfree(int n, ...);
 *	
 * @endcode
 * 
 * @brief Libera punteros si estos no estan a NULL
 *
 * @description 
 * Libera punteros si estos no estan a NULL 
 *
 * @parameters
 * @param[in] n: Puntero/s a liberar
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

/**
 * @page save_file save_file
 *
 * @synopsis
 * @code
 *	#include "aux_functions.h"
 *
 *	int save_file(void* args);
 *	
 * @endcode
 * 
 * @brief Establece la conexíon con el servidor de archivos para recibir los datos y escribirlos en un fichero
 *
 * @parameters
 * @param[in] args: Estructura que contiene los parametros necesarios para establacer la conexion
 *
 * @return
 * int OK si todo fue correcto y ERR si ocurrio un error
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
int save_file(void* args);

/**
 * @page interface_mostrar_nicks interface_mostrar_nicks
 *
 * @synopsis
 * @code
 *	#include "aux_functions.h"
 *
 *	void interface_mostrar_nicks(char* channel, char* list);
 *	
 * @endcode
 * 
 * @brief Actualiza la lista de nicks de la interfaz y sus estados
 *
 * @parameters
 * @param[in] channel: Canal que se quiere actualizar
 * @param[in] list:	Lista de nicks obtenida del comando names, separados por espacios

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
void interface_mostrar_nicks(char* channel, char* list);

/**
 * @page strnext strnext
 *
 * @synopsis
 * @code
 *	#include "aux_functions.h"
 *
 *	char *strnext(char* haystack, int ch);
 *	
 * @endcode
 * 
 * @brief Devuelve una cadena que empieza inmediatamente después de la cadena 'haystack' tras la primera aparición de 'ch'
 *
 * @parameters
 * @param[in] haystack: string original donde hacer la busqueda
 * @param[in] ch: delimitador

 * @return
 * char* substring con la cadena generada, NULL si no se ha encontrado 'ch'
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
char *strnext(char* haystack, int ch);

/**
 * @page parse_type parse_type
 *
 * @synopsis
 * @code
 *	#include "aux_functions.h"
 *
 *	int parse_type(const char* message);
 *	
 * @endcode
 * 
 * @brief Devuelve el tipo de comando (código 3 digitos) de un mensaje no reconocido por IRC_CommandQuery()
 *
 * @parameters
 * @param[in] message: mensaje original
 *
 * @return
 * int codigo de comando si es un codigo valido, ERR si no lo es o comando invalido
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
int parse_type(const char* message);

/**
 * @page parse_type2 parse_type2
 *
 * @synopsis
 * @code
 *	#include "aux_functions.h"
 *
 *	int parse_type2(const char* message);
 *	
 * @endcode
 * 
 * @brief Version experimetnal de parse_type(). 
 * Devuelve el tipo de comando (código 3 digitos) de un mensaje no reconocido por IRC_CommandQuery()
 *
 * @parameters
 * @param[in] message: mensaje original
 *
 * @return
 * int codigo de comando si es un codigo valido, ERR si no lo es o comando invalido
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
int parse_type2(const char* message);

/**
 * @page IRCInterface_WriteSystem_Pretty IRCInterface_WriteSystem_Pretty
 *
 * @synopsis
 * @code
 *	#include "aux_functions.h"
 *
 *	void IRCInterface_WriteSystem_Pretty(char *nick, char *msg);
 *	
 * @endcode
 * 
 * @brief Wrapper de IRCInterface_WriteSystem para mostrar el mensaje con timestamp
 *
 * @parameters
 * @param[in] nick: Nick con el que se muestra el mensaje
 * @param[in] msg: Mensaje original
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
void IRCInterface_WriteSystem_Pretty(char *nick, char *msg);

/**
 * @page IRCInterface_WriteSystemThread_Pretty IRCInterface_WriteSystemThread_Pretty
 *
 * @synopsis
 * @code
 *	#include "aux_functions.h"
 *
 *	void IRCInterface_WriteSystemThread_Pretty(char *nick, char *msg);
 *	
 * @endcode
 * 
 * @brief (Threadsafe) Wrapper de IRCInterface_WriteSystemThread para mostrar el mensaje con timestamp.
 *
 * @parameters
 * @parameters
 * @param[in] nick: Nick con el que se muestra el mensaje
 * @param[in] msg: Mensaje original
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
void IRCInterface_WriteSystemThread_Pretty(char *nick, char *msg);

/**
 * @page IRCInterface_WriteChannelThread_Pretty IRCInterface_WriteChannelThread_Pretty
 *
 * @synopsis
 * @code
 *	#include "aux_functions.h"
 *
 *	void IRCInterface_WriteChannelThread_Pretty(char *chan, char *nick, char *msg);
 *	
 * @endcode
 * 
 * @brief (Threadsafe) Wrapper de IRCInterface_WriteChannelThread para mostrar el mensaje con timestamp.
 *
 * @parameters
 * @param[in] chan: Canal donde escribir el mensaje
 * @param[in] nick: Nick con el que se muestra el mensaje
 * @param[in] msg: Mensaje original
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
void IRCInterface_WriteChannelThread_Pretty(char *chan, char *nick, char *msg);

/**
 * @page testIRC_CommandQuery testIRC_CommandQuery
 *
 * @synopsis
 * @code
 *	#include "aux_functions.h"
 *
 *	int testIRC_CommandQuery(char* message);
 *	
 * @endcode
 * 
 * @brief Para un mensaje IRC, comprueba si es un comando reconocido por IRC_CommandQuery
 *
 * @parameters
 * @param[in] message: Mensaje original
 *
 * @return
 * int OK si es un comando reconocido y ERR si es IRCERR_NOCOMMAND o IRCERR_UNKNOWNCOMMAND
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
int testIRC_CommandQuery(char* message);

/**
 * @page changeMode changeMode
 *
 * @synopsis
 * @code
 *	#include "aux_functions.h"
 *
 *	int changeMode(char *channel, char *nick, char *mode);
 *	
 * @endcode
 * 
 * @brief Funcion que reduce el proceso de cambiar el mode de un nick en un channel dado
 *
 * @parameters
 * @param[in] channel: Canal donde se produce el cambio de mode
 * @param[in] nick: Nick del usuario a modificar
 * @param[in] mode: Nuevo mode
 *
 * @return
 * int OK si es un comando reconocido y ERR si es IRCERR_NOCOMMAND o IRCERR_UNKNOWNCOMMAND
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
int changeMode(char *channel, char *nick, char *mode);

#endif
