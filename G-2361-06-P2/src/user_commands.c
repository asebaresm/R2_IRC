/**
 * @file user_commands.c
 * @brief Implementacion de las funciones IRC de usuario de xchat2
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 13/02/17
 *
 */

#include "../includes/user_commands.h"

/*
int (*p_array_funciones[])(char* ) = {
	pdefault, punames, puhelp, pulist, pujoin, pupart, puleave, puquit,
	punick, puaway, puwhois, puinvite, pukick, putopic, pdefault, pumsg, puquery,
	punotice, pdefault, pdefault, pdefault, puwho, pdefault, pdefault,
	pucycle, pumotd, pdefault, pulusers, pdefault, pdefault, pdefault,
	pdefault, pdefault, pumode, pdefault, pdefault, pdefault, pdefault,
	pdefault, pdefault, pdefault, pdefault, pdefault, pdefault, pdefault,
	pdefault, pdefault, pupartall, pdefault, puback, puunaway, pdefault,
	puoper, puban, pufsend, pufaccept, pufclose
};
*/
p_funcion p_array_funciones[] = {
	pdefault, punames, puhelp, pulist, pujoin, pupart, puleave, puquit,
	punick, puaway, puwhois, puinvite, pukick, putopic, pdefault, pumsg, puquery,
	punotice, pdefault, pdefault, pdefault, puwho, pdefault, pdefault,
	pucycle, pumotd, pdefault, pulusers, pdefault, pdefault, pdefault,
	pdefault, pdefault, pumode, pdefault, pdefault, pdefault, pdefault,
	pdefault, pdefault, pdefault, pdefault, pdefault, pdefault, pdefault,
	pdefault, pdefault, pupartall, pdefault, puback, puunaway, pdefault,
	puoper, puban, pufsend, pufaccept, pufclose //57
};


/*Funciones no necesarias para el cliente*/
int pumode(char* command){ return -1; } // ya se envia con los botones
int pupartall(char* command){ return -1; }
int puback(char* command){ return -1; }
int puunaway(char* command){ return -1; } //existe UNAWAY??
int puoper(char* command){ return -1; }
int puban(char* command){ return -1; } // ya se envia con los botones
int pufsend(char* command){ return -1; } //se envia con los ficheros
int pufaccept(char* command){ return -1; } //se envia con los ficheros
int pufclose(char* command){ return -1; } //se envia con los ficheros
int putopic(char* command){ return -1; } // se envia con la barra de topic
int pukick(char* command){ return -1; } //se envia con los botones
int puleave(char* command){ return -1; }

//TO-DO: por implementar
int pdefault(char* command){ return -1; }
int puhelp(char* command){ return -1; }
int pulist(char* command){ return -1; }
int pupart(char* command){ return -1; }
int puquit(char* command){ return -1; }
int punick(char* command){ return -1; }
int puaway(char* command){ return -1; }
int puwhois(char* command){ return -1; }
int puinvite(char* command){ return -1; }
int pumsg(char* command){ return -1; }
int puquery(char* command){ return -1; }
int punotice(char* command){ return -1; }
int puwho(char* command){ return -1; }
int pucycle(char* command){ return -1; }
int pumotd(char* command){ return -1; }
int pulusers(char* command){ return -1; }

//Implementadas
/**
* @brief Comando de usuario NAMES
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int punames(char* command){ 
	//<< NAMES #redes2
	//>> :irc.eps.net 353 gomupo = #redes2 :flowey cgs gomupo Mamo_1 qwerttyue asdfgh alpeh ArcaFacts BotGram
	char* channels;
	char* passwords;
	char* command_enviar;
	char* prefix = NULL;
	char* target = NULL;
	char channels_passwords [MAXDATA] = {0};
	int ret;

	g_print(MAG "\n<< [user command] NOTICE - command = %s\n" RESET, command);
	/*Comprobar si es un comando names sin argumentos, en caso afirmativo no utilizar la función de Eloy ya que
	parece que falla, y mandarlo tal cual*/
	//Usamos strcasecmp para que den igual minusculas
	if((0 == strcasecmp(command,"/names")) && (strlen(command) == strlen("/names"))){
		g_print("\t Command names sin argumentos: %s \n",command);
		ret = IRCMsg_Names (&command_enviar, prefix, channels_passwords, target);
		if(ret != IRC_OK){
			g_print(RED "ERROR - In punames: IRCMsg_Names no devolvio IRC_OK\n" RESET);
			return ERR;
		}
		g_print("\t command_enviar names sin argumentos: %s \n",command_enviar);
		ret = enviarDatos(sockfd_user,command_enviar, strlen(command_enviar));
		if(ret == ERR){
			g_print(RED "ERROR - In punames: enviarDatos() devolvio error (ver .log)\n\t\tEl cliente se cerrará.\n" RESET);
			exit(1);
		}
		if(ret == 0){ //timeout seguramente
			g_print(RED "ERROR - In punames: enviarDatos() mandó 0 Bytes(ver .log)\n\t\t(Timeout)El cliente se cerrará.\n" RESET);
			exit(1);
		}
		IRCInterface_PlaneRegisterOutMessage(command);
		free(command_enviar);
		return OK;
	}

	ret = IRCUserParse_Names(command, &channels, &passwords);
	if(ret != IRC_OK){
		g_print(RED "ERROR - In punames: IRCUserParse_Names no devolvio IRC_OK\n" RESET);
		return ERR;
	}
	g_print("\t command: %s \n",command);
	g_print("\t channels : %s \n",channels);
	g_print("\t passwords : %s \n",passwords);

	sprintf(channels_passwords,"%s %s",channels,passwords?passwords:"");

	ret = IRCMsg_Names (&command_enviar, prefix, channels_passwords, target);
	if(ret != IRC_OK){
		g_print(RED "ERROR - In punames: IRCMsg_Names no devolvio IRC_OK\n" RESET);
		return ERR;
	}
	g_print("\t Mensaje a enviar command_enviar: %s \n",command_enviar);

	enviarDatos(sockfd_user, command_enviar, strlen(command_enviar));

	IRCInterface_PlaneRegisterOutMessage(command_enviar);
	mfree(5, command_enviar, channels, passwords, prefix, target);
	return OK;
}

/**
* @brief Comando de usuario JOIN
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int pujoin(char* command){ 

	long ret = -1;
	int retorno = -1;

	char* key = NULL;
	char* msg = NULL;
	char* channels = NULL;
	char* passwords = NULL;
	char* command_enviar = NULL;
	char* prefix = NULL;
	char canales_y_passwords [MAXDATA] = {0};

	g_print(MAG "\n<< [user command] UJOIN - command = %s\n" RESET, command);

	ret = IRCUserParse_Join(command, &channels, &passwords);
	if(ret != IRC_OK){
		g_print(RED "ERROR - In pujoin: IRCUserParse_Join no devolvio IRC_OK\n" RESET);
		return -1;
	}
	g_print("\t command: %s \n",command);
	g_print("\t channels : %s \n",channels);
	g_print("\t passwords : %s \n",passwords);

	sprintf(canales_y_passwords,"%s %s",channels,passwords?passwords:"");

	//enviar varios canales
	ret = IRCMsg_Join (&command_enviar, prefix, canales_y_passwords, key, msg);
	if(ret != IRC_OK){
		g_print(RED "ERROR - In pujoin: IRCMsg_Join no devolvio IRC_OK\n" RESET);
		return -1;
	}
	g_print("\t Mensaje a enviar command_enviar: %s \n",command_enviar);
	
	retorno = enviarDatos(sockfd_user,command_enviar, strlen(command_enviar));
	if(retorno < 0){
		g_print(RED "ERROR - In pujoin: enviarDatos() devolvio error (ver secuencia en .log)\n\t\tEl cliente se cerrará.\n" RESET);
		exit(1);
	}
	if(retorno == 0){ //timeout 
		g_print(RED "ERROR - In pujoin: enviarDatos() envió 0 Bytes(ver secuencia en .log)\n\t\t(Timeout de conexión probablemente)\n" RESET);
		exit(1);
	}

	IRCInterface_PlaneRegisterOutMessage(command_enviar);	
	mfree(6, command_enviar, channels, passwords, prefix, key, msg);
	return OK; 
}