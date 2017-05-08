/**
 * @file xchat2.h
 * @brief Declaraciones de funciones, definición de tipos: implementación de los callbacks de xchat2.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 20/03/17
 *
 */
#ifndef xchat_h
#define xchat_h

#include <redes2/ircxchat.h>
#include <redes2/irc.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include "G-2361-06-P2-types.h"
#include "G-2361-06-P2-conexion_tcp.h"
#include "G-2361-06-P2-logger.h"
#include "G-2361-06-P2-user_commands.h"
#include "G-2361-06-P2-aux_functions.h"

extern p_funcion p_array_funciones[57];	/**< Array de funciones de usuario definido en user_commands.c */

/*
typedef struct User{
	char* nick;
	char* channels;
} User;
*/

/*Funciones auxiliares generales*/
//void interface_mostrar_nicks(char* channel, char* list);

/**
 * @page command_query command_query
 *
 * @synopsis
 * @code
 *	#include "xchat2.h"
 *
 *	int command_query(char *message);
 *	
 * @endcode
 * 
 * @brief Parsea los mensajes y respuestas que recibe del servidor
 *
 * @parameters
 * @param[in] massage: Mensaje recibido para procesar
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
int command_query(char *message);

/**
 * @page unpipe unpipe
 *
 * @synopsis
 * @code
 *	#include "xchat2.h"
 *
 *	void unpipe(char* message, int MAXDATA_flag);
 *	
 * @endcode
 * 
 * @brief Funcion para dividir en comandos la cadena "message"
 * Tiene implementada una logica de concatenacion de trozos de comandos de cara a cuando se lee el tamaño maximo
 * del buffer de lectura en el socket y se corta un comando en 2 lecturas.
 * @parameters
 * @param[in] massage: Mensaje recibido para procesar
 * @param[in] MAXDATA_flag: Flag que viene activada si se alcanzó el máximo de Bytes leidos por el buffer del socket
 *
 * @return
 * Void
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
void unpipe(char* message, int MAXDATA_flag);

/**
 * @page receive_messages receive_messages
 *
 * @synopsis
 * @code
 *	#include "xchat2.h"
 *
 *	void receive_messages(void* no_arg);
 *	
 * @endcode
 * 
 * @brief Funcion ejecutada por un hilo, que recibe mensajes del servidor y los pasa a la funcion unpipe().
 *
 * @parameters
 * @param[in] no_arg: estructura de parametros (vacia, experimental)
 *
 * @return
 * Void
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
void receive_messages(void* no_arg);

/**
 * @page glueAndQuery glueAndQuery
 *
 * @synopsis
 * @code
 *	#include "xchat2.h"
 *
 *	void glueAndQuery(char* command, char* last_command);
 *	
 * @endcode
 * 
 * @brief Concatena 2 trozos de comandos cortados y llama a command_query() para procesarlo como un único comando.
 * Concatena el (trozo de) comando actual en unpipe() y el (trozo de) comando procesado anterior
 * @parameters
 * @param[in] command: comando actual procesando en unpipe()
 * @param[in] last_command: comando anterior procesado en unpipe()
 *
 * @return
 * Void
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
void glueAndQuery(char* command, char* last_command);

#endif