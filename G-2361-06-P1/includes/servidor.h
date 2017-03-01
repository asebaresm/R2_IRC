/** 
 * @file servidor.h
 * @brief Prototipos de funciones que implementan el servidor IRC.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * 
 */

#ifndef _SERVIDOR_H_
#define _SERVIDOR_H_

#include "../includes/daemon.h"
#include "../includes/conexion_tcp.h"

#include <pthread.h>   /*for threading , link with lpthread*/

#define PORT_LIMIT_FLOOR 1024	/**< Puerto por debajo de 1024 requiere 'sudo' */
#define PORT 8888				/**< Puerto en el que se abre el servidor */
#define MAX_NICK_LEN 10			/**< Longitud maxima de nick. Ver: https://tools.ietf.org/html/rfc2813#section-2.2.1 */
#define MAX_CLIENTS 100			/**< Maximo de conexiones que acepta el servidor (orientativo) */
#define STR_LEN 256				/**< Maximo de caracteres para hostname */
#define IP_LEN 16				/**< Maximo de caracteres para una IP en formato string */
#define MSG_LEN 8192			/**< Maximo de caracteres para un mensaje recibido de un cliente por socket (estimado).
									 Por defecto se recibe el máximo de caracteres de una trama TCP (comandos en pipeline).*/


/**
 * @brief Estructura-registro de conexiones
 *
 */
/*
typedef struct cliente_struct{
    int sockfd;
} Cliente;
*/

/**
 * @brief Estructura de Cliente para llevar un registro de conexiones
 *
 */
/*
typedef struct conexiones_struct{
    Cliente clientes[MAX_CLIENTS]; 
    int num; 
} Conexiones;
*/

/**
* @brief Estructura que representa un usuario, identificado por nick y descriptor de socket
*/
typedef struct str_usock {
	char ip[IP_LEN];			/**< IP en formato string */
	char hostname[STR_LEN];		/**< Hostname en formato string */
	char nick[256]; 			/**< Array de nicks */
	int sockfd; 				/**< Id del sock */
} usock;

/**
* @brief Estructura que representa un conjunto de usuarios
*/
typedef struct str_users{
	usock users[200]; 	/**< Array de usuarios */
	int num; 			/**< numero de usuarios */
} Users;

/**
  @brief Estructura de parametros para enviarle información al hilo (a cada usuario y conexion)
*/
typedef struct str_thdata
{
	int thread_no;
	int new_fd;
	char* prefix;
	char* passw;
	char* nick;
	char* nick2;
	char* user;
	//char* host;
	char* modehost;
	char* serverother;
	char* realname;
	char* name;
	char* quit_message;
	char* channel;
	char* key;
	char* join_msg;
	char* topic;
	char* target;
	int nick_sent;
	int passw_sent;
	int user_sent;
	//WHOIS
	char* maskarray;
	//NAMES
	char* msgtarget;
	//PING
	char* server;
	char* server2;
	//mas info
	char ip[IP_LEN];			
	char hostname[STR_LEN];
} thdata;

/** 
 * @name main
 *
 * @brief Funcion para ejecutar el servidor
 * 
 * @param int argc: numero de parametros de entrada
 * @param char * argv[]: parametros de entrada
 *  
 *
 * @return OK si todo ha salido bien, ERROR si hay algun fallo
 *
 */
int main (int argc, char * argv[]);

/**
 * @brief Elimina una entrada del registro de sockets-usuarios del servidor a partir del nick
 * @param Users* usrs: puntero a array de estructuras usuario.
 * @param char* nick: nick del usuario a eliminar
 * @return OK si todo es correcto y ERR si hay algun fallo
 */
void remove_entry(Users* usrs, char* nick);

/**
 * @brief Procesa la conexion de un cliente que se conecte al servidor
 * @param int argc: puntero a estructura Users con todos los usuarios en el momento de la llamada.
 * @return OK si todo es correcto y ERR si hay algun fallo
 */
int procesar_conexion (void * ptr);

/**
 * @brief Procesa un mensaje recibido por un cliente conectado al servidor
 * @param int new_fd: descriptor de fichero del socket abierto con ese cliente
 * @param char *message: mensaje recibido en el socket
 * @param thdata* data: estructura de parametros propia de la conexion IRC
 * @param Users* usrs: puntero a estructura Users con todos los usuarios en el momento de la llamada.
 * @return OK si todo es correcto y ERR si hay algun fallo
 */
int procesar_mensaje (int new_fd, char *message, thdata* data, Users* usrs);

/**
 * @brief Funcionc core de procesar_mensaje().
 * Procesa el mensaje o mensajes (pipeline) leídos del cliente y procesa los comandos IRC.
 * @param char *message: mensaje recibido en el socket
 * @param int new_fd: descriptor de fichero del socket abierto con ese cliente
 * @param thdata* data: estructura de parametros propia de la conexion IRC
 * @param Users* usrs: puntero a estructura Users con todos los usuarios en el momento de la llamada.
 * @return OK si todo es correcto y ERR si hay algun fallo
 */
int command_query(char *message, int new_fd, thdata* data, Users* usrs);

#endif