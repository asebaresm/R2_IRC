/**
 * @file xchat2.h
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 20/03/17
 * @brief Declaraciones de funciones, definición de tipos: implementación de los callbacks de xchat2.
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

#include "types.h"
#include "conexion_tcp.h"
#include "logger.h"
#include "user_commands.h"
#include "aux_functions.h"

extern p_funcion p_array_funciones[57];

/*Funciones auxiliares generales*/
//void interface_mostrar_nicks(char* channel, char* list);
int command_query(char *message);
void unpipe(char* message, int MAXDATA_flag);
void receive_messages(void* no_arg);
void glueAndQuery(char* command, char* last_command);

#endif