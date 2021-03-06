/**
 * @file G-2361-06-P1-Functions.c
 * @brief Implementacion de ciertas funciones IRC que implementa el servidor IRC.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 13/02/17
 *
 */
#include "../includes/G-2361-06-P1-Functions.h"


void liberaLista(char** lista, long nElems){
	int i;
	for(i=0;i<nElems;i++)
		free(lista[i]);
}


void funcionUser(char *user, char *nick, char *realname, char *modehost, int IDsocket){
	char *welcome_msj, *host_msj, *create_msj, *info_msj, *respuesta;

	/*Mensaje 001*/
	IRCMsg_RplWelcome (&welcome_msj, PREFIJO, nick, nick, user, modehost);
	/*Mensaje 002*/
	IRCMsg_RplYourHost (&host_msj, PREFIJO, nick, "localhost", "1.0");
	/*Mensaje 003*/
	IRCMsg_RplCreated (&create_msj, PREFIJO, nick, 0);
	/*Mensaje 004*/
	IRCMsg_RplMyInfo(&info_msj, PREFIJO, nick, "Practica 1", "1.0", "abBcCFiIoqrRswx", "abehiIklmMnoOPqQrRstvVz");

	/*Todos los mensajes en uno solo*/
	IRC_PipelineCommands(&respuesta, welcome_msj, host_msj, create_msj, info_msj, NULL);

	send(IDsocket, respuesta, strlen(respuesta), 0);
	free(welcome_msj); free(host_msj), free(create_msj); free(info_msj); free(respuesta);
}

void funcionNick(long id, char* usuario, char* nick_name, char* real, char* nick, int IDsocket){
	char *complex = NULL, *msj_nick = NULL;
	
	/*Crea el array por ejemplo : :celia!~celia@66.red-83-63-217.staticip.rima-tde.net*/
	IRC_ComplexUser(&complex, nick_name, usuario, NULL, NULL);

	IRCTADUser_Set(id, usuario, nick_name, real, usuario, nick, real);

	/*Mensaje de vuelta del comando NICK*/
	IRCMsg_Nick(&msj_nick, complex, NULL, nick);
	send(IDsocket, msj_nick, strlen(msj_nick), 0);

	free(complex); free(msj_nick); 
}


void funcionAway(long id, char *usuario, char *nick_name, char *real, char *away, char *msg, int IDsocket){
	char *away_msj;

	if(away == NULL){ /*No hay away*/
		IRCTADUser_SetAway (id, usuario, nick_name, real, msg);
		IRCMsg_RplNowAway (&away_msj, PREFIJO, nick_name);
	}else{ /*Hay away*/
		/*Se borra*/
		IRCTADUser_SetAway (id, usuario, nick_name, real, NULL);
		IRCMsg_RplUnaway (&away_msj, PREFIJO, nick_name);
	}
	send(IDsocket,away_msj,strlen(away_msj),0);
	free(away_msj);
}


void funcionPing(char *server1, char *server2, int IDsocket){
	char *pong_msj;

	if(!server2){ /*En el caso de que no se especifique el server2*/
		IRCMsg_Pong (&pong_msj, PREFIJO, PREFIJO, server2, server1);
		send(IDsocket,pong_msj,strlen(pong_msj),0);
	}else{
		IRCMsg_Pong (&pong_msj, PREFIJO, PREFIJO, server1, server2);
		send(IDsocket,pong_msj,strlen(pong_msj),0);
	}
 	free(pong_msj);
}

void funcionPart(char *channel, char *nick_name, char *msg, int IDsocket){
	char *part_msj, **lista = NULL;
	long nChannels = 0, nUsus = 0;
	int i, flag = 0;

	/*Obtenemos la lista con los nombres de todos los canales*/
	IRCTADChan_GetList (&lista, &nChannels, NULL);
	for(i = 0; i < nChannels; i++){
		if(strcmp(lista[i], channel) == 0){
			flag = 1; /*El canal existe*/
		}
	}
	IRCTADChan_FreeList (lista, nChannels);

	if(flag == 0){ /*No existe el canal*/
		IRCMsg_ErrNoSuchChannel(&part_msj, PREFIJO, nick_name, channel);
		send(IDsocket,part_msj,strlen(part_msj),0);
	}else{

		IRCTAD_Part (channel, nick_name);
		nUsus = IRCTADChan_GetNumberOfUsers(channel); /*Obtenemos el numero de usuarios del canal*/

		if(nUsus <= 0){ /*Si no hay usuarios*/
			IRCTADChan_Delete(channel); /*Se borra el canal*/
		}

		IRCMsg_Part (&part_msj, PREFIJO, channel, msg);
		send(IDsocket,part_msj,strlen(part_msj),0);
	}
	free(part_msj); 
}

void funcionList(char *nick, char *target, int IDsocket){
	char **listaCanales, *can, *listaEnd;
	long numCanales, mode, numUsuariosCanal, modeVal;
	char nUsusCanales[4];
	int i;

	/*Obtiene una lista con el nombre de los canales existentes*/
	IRCTADChan_GetList(&listaCanales, &numCanales, target);

	for(i = 0; i < numCanales; i++){
		/*Modo del canal en formato entero*/
		mode = IRCTADChan_GetModeInt (listaCanales[i]);
		modeVal = mode & IRCMODE_SECRET;

		if(modeVal != IRCMODE_SECRET){
			/*Obtiene numero de usuarios de un canal*/
			numUsuariosCanal = IRCTADChan_GetNumberOfUsers (listaCanales[i]);
			sprintf(nUsusCanales,"%ld",numUsuariosCanal);
			/*Mensaje de respuesta*/
			IRCMsg_RplList(&can, PREFIJO, nick, listaCanales[i], nUsusCanales, "");
			send(IDsocket, can, strlen(can), 0);
		}
	}

	if(i > 0){
		/*Mensaje de respuesta*/
		IRCMsg_RplListEnd(&listaEnd,PREFIJO,nick);
		send(IDsocket,listaEnd,strlen(listaEnd),0);
		free(listaEnd);free(can);
		IRCTADChan_FreeList (listaCanales, numCanales);
	}
}

void funcionMotd(char *nick_name, char *target, int IDsocket, char *fich){
	char lectura[80];
	char *motdStart_msj, *motd_msj, *motdEnd_msj;
	FILE *f;
	f = fopen(fich,"r");
	
	if(!target)
		IRCMsg_RplMotdStart (&motdStart_msj, PREFIJO, nick_name, PREFIJO);
	else
		IRCMsg_RplMotdStart (&motdStart_msj, PREFIJO, nick_name, target);

	send(IDsocket,motdStart_msj,strlen(motdStart_msj),0);
	
	while(fgets(lectura, 80, f) != NULL){
		lectura[strlen(lectura)-1] = 0;
		IRCMsg_RplMotd(&motd_msj, PREFIJO, nick_name, lectura);
		send(IDsocket,motd_msj,strlen(motd_msj),0);		
	}

	fclose(f);
	IRCMsg_RplEndOfMotd (&motdEnd_msj, PREFIJO, nick_name);
	send(IDsocket,motdEnd_msj,strlen(motdEnd_msj),0);

	free(motd_msj); free(motdStart_msj); free(motdEnd_msj);
}

