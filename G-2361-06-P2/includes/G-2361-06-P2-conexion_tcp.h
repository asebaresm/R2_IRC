/**
 * @file conexion_tcp.h
 * @brief Declaraciones de funciones, definición de tipos: liberia para conexiones TCP.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 13/02/17
 *
 */

#ifndef conexion_tcp_h
#define conexion_tcp_h 

/* -- Includes -- */
#include "G-2361-06-P2-types.h"
#include "G-2361-06-P2-logger.h"

#include <unistd.h> /*write*/
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> /*inet_addr*/

#define BUFSIZE 1024    /**< Tam. max. del buffer que se lee */
#define AUX_SBUF 1152   /**< Tam. max. del buffer auxiliar para logeo de eventos (1024 + 128 extra)*/
#define MAX_CON_REQ 10  /**< Max. de peticiones de conexion activas (e.g. la 11 falla si puesto a 10) */

/**
 * @page crearConexion crearConexion
 *
 * @synopsis
 * @code
 *	#include "conexion_tcp.h"
 *
 *	int crearConexion(int portno, struct sockaddr_in* server)
 *	
 * @endcode
 * 
 * @brief Crea un socket para una conexion a partir de un puerto e IP
 *
 * @parameters
 * @param[in] portno: Numero de puerto
 * @param[in] server: Direccion IP
 *
 * @return
 * int descriptor de socket si todo ha ido bien o ERR
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
int crearConexion(int portno, struct sockaddr_in* server);

/**
 * @page recvDatos recvDatos
 *
 * @synopsis
 * @code
 *	#include "conexion_tcp.h"
 *
 *	int recvDatos(int client_sock, char* client_message, int cm_size, char *hostaddrp)
 *	
 * @endcode
 * 
 * @brief Funcion que lee datos recibidos en un socket con conexion
 *
 * @parameters
 * @param[in] client_sock: Descriptor del socket
 * @param[in] client_message: Mensaje leido
 * @param[in] cm_size: Tamaño del mensaje leido
 * @param[in] server: Hostname
 *
 * @return
 * int tamaño de datos leidos o ERR si no se ha podido leer (problema de conexion)
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int recvDatos(int client_sock, char* client_message, int cm_size, char *hostaddrp);

/**
 * @page enviarDatos enviarDatos
 *
 * @synopsis
 * @code
 *	#include "conexion_tcp.h"
 *
 *	int enviarDatos(int client_sock, char* message, int message_size)
 *	
 * @endcode
 * 
 * @brief Funcion que envia datos a un socket con conexion
 *
 * @parameters
 * @param[in] client_sock: Descriptor del socket
 * @param[in] message: Mensaje a enviar
 * @param[in] message_size: Tamaño del mensaje a enviar
 *
 * @return
 * int tamaño de datos enviados o ERR si no se ha podido enviar nada
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
int enviarDatos(int client_sock, char* message, int message_size);

/**
 * @page crearConexion crearConexion
 *
 * @synopsis
 * @code
 *	#include "conexion_tcp.h"
 *
 *	int crearConexion(int portno, struct sockaddr_in* server)
 *	
 * @endcode
 * 
 * @brief Funcion que cierra la conexion con un socket
 *
 * @parameters
 * @param[in] client_sock: Descriptor del socket
 * @param[in] hostaddrp: Hostname con el que cerramos al conexion (opcional)
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
int cerrarConexion(int client_sock, char* hostaddrp);
#endif