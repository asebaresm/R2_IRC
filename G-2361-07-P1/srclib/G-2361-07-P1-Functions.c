#include "../includes/G-2361-07-P1-Functions.h"


/**
 * @page funcionUser funcionUser
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Realiza toda la funcionalidad correspondiente a crear un 
 *		  nuevo usuario.
 *
 * @description 
 * Imprime por stdout la string a. Este comentario debería ser mucho más explicativo.
 *
 * @parameters
 * @param[in] user: nombre del nuevo usuario.
 * @param[in] nick: nick del nuevo usuario.
 * @param[in] realname: realname del nuevo usuario.
 * @param[in] modehost: 
 * @param[in] IDsocket: el numero del identificador del socket.
 *
 * @return
 * No devuelve nada
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha ()
 *
 *
 * @date 13 de abril de 2017
 *
 *<hr>
*/
void funcionUser(char *user, char *nick, char *realname, char *modehost, int IDsocket){
	char *welcome_msj, *host_msj, *create_msj, *info_msj, *respuesta;

	/*Mensaje 001*/
	IRCMsg_RplWelcome (&welcome_msj, PREFIJO, nick, nick, user, modehost);
	/*Mensaje 002*/
	IRCMsg_RplYourHost (&host_msj, PREFIJO, nick, "Practica 1", "1.0");
	/*Mensaje 003*/
	IRCMsg_RplCreated (&create_msj, PREFIJO, nick, 0);
	/*Mensaje 004*/
	IRCMsg_RplMyInfo(&info_msj, PREFIJO, nick, "Practica 1", "1.0", "adDcCioqrRswx", "abehiIklmMnoOPqQrRstvVz");

	/*Juntamos todos los mensajes en uno solo*/
	IRC_PipelineCommands(&respuesta, welcome_msj, host_msj, create_msj, info_msj, NULL);

	send(IDsocket, respuesta, strlen(respuesta), 0);
	/*syslog(LOG_INFO, "SEND USER");*/
	free(welcome_msj); free(host_msj), free(create_msj); free(info_msj); free(respuesta);
}

/**
 * @page funcionNick funcionNick
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Realiza toda la funcionalidad correspondiente al nick de un usuario.
 *
 * @description 
 * Esta funcion se encarga de modificar el nick de un usuario, modificando el 
 * campo nick_name por el campo nick. Se modifica y este seria el nuevo nick
 * del usuario actual.
 *
 * @parameters
 * @param[in] id: identificador del usuario.
 * @param[in] usuario: nombre del usuario.
 * @param[in] nick_name: nick actual del usuario.
 * @param[in] real: realname del usuario.
 * @param[in] nick: nick nuevo.  
 * @param[in] IDsocket: el numero del identificador del socket.
 *
 * @return
 * No devuelve nada
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha ()
 *
 *
 * @date 13 de abril de 2017
 *
 *<hr>
*/
void funcionNick(long id, char* usuario, char* nick_name, char* real, char* nick, int IDsocket){
	char *complex = NULL, *msj_nick = NULL;
	
	/*Crea el array por ejemplo : :celia!~celia@66.red-83-63-217.staticip.rima-tde.net*/
	syslog(LOG_INFO, "COMPLEX USER");
	IRC_ComplexUser(&complex, nick_name, usuario, NULL, NULL);

	IRCTADUser_Set(id, usuario, nick_name, real, usuario, nick, real);

	/*Mensaje de vuelta del comando NICK*/
	IRCMsg_Nick(&msj_nick, complex, NULL, nick);
	send(IDsocket, msj_nick, strlen(msj_nick), 0);

	free(complex); free(msj_nick); 
}


void funcionAway(long id, char *usuario, char *nick_name, char *real, char *away, char *msg, int IDsocket){
	char *away_msj;

	if(away == NULL){
		/*syslog(LOG_INFO, "AWAY: No hay away");*/
		IRCTADUser_SetAway (id, usuario, nick_name, real, msg);
		IRCMsg_RplNowAway (&away_msj, PREFIJO, nick_name);
	}else{
		/*syslog(LOG_INFO, "AWAY: Hay away");*/
		/*Se borra*/
		IRCTADUser_SetAway (id, usuario, nick_name, real, NULL);
		IRCMsg_RplUnaway (&away_msj, PREFIJO, nick_name);
	}
	send(IDsocket,away_msj,strlen(away_msj),0);
	free(away_msj);
}


/**
 * @page funcionPart funcionPart
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Realiza toda la funcionalidad correspondiente a abandonar un canal.
 *
 * @description 
 * Esta funcion se encarga de que un usuario abandone un canal y en el caso
 * de que no queden mas usuarios eliminar el canal. 
 *
 * @parameters
 * @param[in] channel: canal del que se quiere salir el usuario.
 * @param[in] nick_name: nick del usuario que quiere abandonar el canal.
 * @param[in] msg: mensaje que se va a mandar.
 * @param[in] IDsocket: el numero del identificador del socket.
 *
 * @return
 * No devuelve nada
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha ()
 *
 *
 * @date 13 de abril de 2017
 *
 *<hr>
*/
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
			IRCTADChan_Delete(channel); /*Borramos el canal*/
		}

		IRCMsg_Part (&part_msj, PREFIJO, channel, msg);
		send(IDsocket,part_msj,strlen(part_msj),0);
	}

	free(part_msj); 
}

/**
 * @page funcionList funcionList
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Realiza toda la funcionalidad correspondiente a listar los canales.
 *
 * @description 
 * Esta funcion se encarga de listar los canales creados. 
 *
 * @parameters
 * @param[in] nick: nick del usuario actual.
 * @param[in] target: 
 * @param[in] IDsocket: el numero del identificador del socket.
 *
 * @return
 * No devuelve nada
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha ()
 *
 *
 * @date 13 de abril de 2017
 *
 *<hr>
*/
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


void funcionMode(char *usuario, char *nick_name, char *channeluser, char *modo, char *key, int IDsocket){
	char *mode_msj;
	long modeUsuChannel, modeValUsu;

	/*Modo usuario en un canal*/
	modeUsuChannel = IRCTAD_GetUserModeOnChannel(channeluser, nick_name);
	modeValUsu = modeUsuChannel & IRCUMODE_OPERATOR;

	if(modeValUsu == IRCUMODE_OPERATOR){
		/*Cambia modo de un canal*/
		IRCTAD_Mode (channeluser, nick_name, modo);

		if(strstr(modo,"k")!=NULL){
			IRCTADChan_SetPassword (channeluser,key);
			free(key);
		}
		IRCMsg_Mode (&mode_msj, PREFIJO, channeluser, modo, usuario);
		send(IDsocket,mode_msj,strlen(mode_msj),0);
		free(mode_msj);
	}
}


/**
 * @page funcionMotd funcionMotd
 *
 * @synopsis
 * @code
 * 	#include <redes2/irc.h>
 *
 * @endcode
 * 
 * @brief Realiza toda la funcionalidad correspondiente al mensaje del dia.
 *
 * @description 
 * Esta funcion se encarga de abrir un fichero y mostrar por pantalla el 
 * mensaje del dia. 
 *
 * @parameters
 * @param[in] nick_name: nick del usuario actual.
 * @param[in] target: 
 * @param[in] IDsocket: el numero del identificador del socket.
 * @param[in] fich: el nombre del fichero que se va a leer.
 *
 * @return
 * No devuelve nada
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha ()
 *
 *
 * @date 13 de abril de 2017
 *
 *<hr>
*/
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

