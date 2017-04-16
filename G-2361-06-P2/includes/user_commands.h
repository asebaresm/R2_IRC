/**
 * @file user_commands.h
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 20/03/17
 * @brief Declaraciones de funciones, definición de tipos: implementación de funciones IRC de usuario.
 *
 */
#ifndef user_commands_h
#define user_commands_h

#include <redes2/ircxchat.h>
#include <redes2/irc.h>
#include <pthread.h>

#include "conexion_tcp.h"
#include "aux_functions.h"

typedef int (*p_funcion)(char* );

extern int sockfd_user;
extern pthread_t recv_tid;

int punotice(char* command);
int pucycle(char* command);
int pumotd(char* command);
int pulusers(char* command);
int pumode(char* command);
int pupartall(char* command);
int puback(char* command);
int puunaway(char* command);
int puoper(char* command);
int puban(char* command);
int pufsend(char* command);
int pufaccept(char* command);
int pufclose(char* command);
int putopic(char* command);
int pukick(char* command);
int puinvite(char* command);
int puwhois(char* command);
int puaway(char* command);
int punick(char* command);
int puquit(char* command);
int puleave(char* command);
int pupart(char* command);
int pujoin(char* command);
int puwho(char* command);
int punames(char* command);
int pumsg(char* command);
int pulist(char* command);
int puhelp(char* command);
int pdefault(char* command);
int puquery(char* command);

#endif