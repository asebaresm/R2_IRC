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

void liberaLista(char** lista, long nElems);
void funcionNick(long id, char* usuario, char* nick_name, char* real, char* nick, int IDsocket);
void funcionUser(char *user, char *nick, char *realname, char *modehost, int IDsocket);
void funcionPing(char *server1, char *server2, int IDsocket);
void funcionAway(long id, char *usuario, char *nick_name, char *real, char *away, char *msg, int IDsocket);
void funcionPart(char *channel, char *nick_name, char *msg, int IDsocket);
void funcionList(char *nick, char *target, int IDsocket);
void funcionMode(char *usuario, char *nick_name, char *channeluser, char *modo, char *key, int IDsocket);
void funcionMotd(char *nick_name, char *target, int IDsocket, char *fich);
