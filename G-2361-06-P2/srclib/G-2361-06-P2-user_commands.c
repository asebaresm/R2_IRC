/**
 * @file user_commands.c
 * @brief Implementacion de las funciones IRC de usuario de xchat2
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 13/02/17
 *
 */

#include "../includes/G-2361-06-P2-user_commands.h"


p_funcion p_array_funciones[] = {
	pdefault, punames, puhelp, pulist, pujoin, pupart, puleave, puquit,
	punick, puaway, puwhois, puinvite, pukick, putopic, pdefault, pumsg, puquery,
	punotice, pdefault, pdefault, pdefault, puwho, pdefault, pdefault,
	pucycle, pumotd, pdefault, pulusers, pdefault, pdefault, pdefault,
	pdefault, pdefault, pumode, pdefault, pdefault, pdefault, pdefault,
	pdefault, pdefault, pdefault, pdefault, pdefault, pdefault, pdefault,
	pdefault, pdefault, pupartall, pdefault, puback, puunaway, pdefault,
	puoper, puban, pufsend, pufaccept, pufclose //0-56
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
//int pdefault(char* command){ return -1; }

//int puhelp(char* command){ return -1; }
//int pulist(char* command){ return -1; }
//int pupart(char* command){ return -1; }
//int puquit(char* command){ return -1; }
//int punick(char* command){ return -1; }
//int puaway(char* command){ return -1; }
//int puwhois(char* command){ return -1; }
//int puinvite(char* command){ return -1; }
//int pumsg(char* command){ return -1; }
//int puquery(char* command){ return -1; }
//int punotice(char* command){ return -1; }
//int pucycle(char* command){ return -1; }
//int pumotd(char* command){ return -1; }
//int pulusers(char* command){ return -1; }

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

/**
* @brief Comando de usuario QUIT
* En principio solo se llama con:
* - Callback: Boton 'Desconectar'     -> IRCInterface_DisconnectServer(char *server, int port)
* - Callback: '/QUIT en chat + ENTER' -> IRCInterface_NewCommandText("/QUIT ")
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int puquit(char* command){

	char* command_enviar;
	char* reason;
	//char command[] = "QUIT :Leaving";

	char** channelsQuit;
	int numChannelsQuit;
	int i, ret;

	//kill hilo 'receive_messages()'
	ret = pthread_cancel(recv_tid);
	if (ret != 0){
		g_print(MAG "\npthread_cancel() return = %d\n" RESET, ret);
	}

   	IRCUserParse_Quit (command, &reason);
   	IRCMsg_Quit (&command_enviar, NULL, reason ? reason : "Desconectando...");
	g_print("\t Mensaje a enviar command_enviar en QUIT: %s \n",command_enviar);
	enviarDatos(sockfd_user, command_enviar, strlen(command_enviar));
	IRCInterface_PlaneRegisterOutMessage(command_enviar);
	cerrarConexion(sockfd_user, NULL);
	mfree(2, command_enviar, reason);

	/*IRCInterface_RemoveAllChannels da segmentation fault por alguna razón*/
	IRCInterface_ListAllChannels(&channelsQuit, &numChannelsQuit);
	for(i = 0; i<numChannelsQuit; i++){
		//IRCInterface_WriteChannelThread(channelsQuit[i],"*", "Desconectado.");
		IRCInterface_RemoveChannel(channelsQuit[i]);
	}
	IRCInterface_WriteSystem_Pretty("*", "Desconectado.");

	return OK;
}

/**
* @brief Comando de usuario WHO
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int puwho(char* command){

	long ret = -1;

	char* command_enviar=NULL;
	char* prefix = NULL;
	char* mask;
	
	ret = IRCUserParse_Who (command, &mask);
	if(ret != IRC_OK){
		g_print("ERROR - IRCInterface_NewCommandText - UWHO - IRCUserParse_Who");
		return -1;
	}
	g_print("\t command: %s \n",command);
	g_print("\t mask who: %s \n",mask);
	
	ret = IRCMsg_Who (&command_enviar, prefix, mask, NULL);
	if(ret != IRC_OK){
		g_print(RED "ERROR - puwho - IRCMsg_Who \n" RESET);
		return ERR;
	}

	g_print("\t command_enviar: %s \n",command_enviar);

	if(enviarDatos(sockfd_user,command_enviar, strlen(command_enviar) == ERR)){
		g_print("ERROR: IRCInterface_NewCommandText - enviarDatos - Who\n");
		return ERR;
	}

	IRCInterface_PlaneRegisterOutMessage(command_enviar);	
	mfree(3, command_enviar, prefix, mask);
	return OK;
}

/**
* @brief Comando de usuario MSG y PRIVMSG
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int pumsg(char* command){
	char* nickorchannel;
	char* msg;
	char *command_enviar = NULL;
	char *prefix = NULL;
	int ret;

	//<< privmsg gomupo :probando
	//>> :gomupo!~gonzalo@119.181.218.87.dynamic.jazztel.es PRIVMSG gomupo :probando
	ret = IRCUserParse_Msg(command, &nickorchannel, &msg);
	if(ret != IRC_OK){
		g_print("ERROR - IRCInterface_NewCommandText - UMSG - IRCUserParse_Msg\n");
		return ERR;
	}
	g_print("\t command: %s \n", command);
	g_print("\t nickorchannel: %s \n", nickorchannel);
	g_print("\t msg: %s \n", msg);

	ret = IRCMsg_Privmsg (&command_enviar, prefix, nickorchannel, msg);
	if(ret == ERR){
		g_print("ERROR: IRCInterface_NewCommandText - IRCMsg_Privmsg \n");
		return ERR;
	}

	g_print("\t Mensaje a enviar command_enviar en pumsg: %s \n",command_enviar);
	ret = enviarDatos(sockfd_user, command_enviar, strlen(command_enviar));
	if(ret == ERR){
		g_print("ERROR: IRCInterface_NewCommandText - enviarDatos - Names\n");
		return ERR;
	}
	IRCInterface_PlaneRegisterOutMessage(command_enviar);
	//No recibimos nada en este comando, los mensajes de otros usuarios los recibimos por otro hilo
	IRCInterface_WriteChannel (nickorchannel, nick_user, msg);
	mfree(4, command_enviar, nickorchannel, prefix, msg);
	return OK;
}

/**
* @brief Comando de usuario LIST
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int pulist(char* command){ 
	long ret = -1;
	int retorno = -1;
	char *channel = NULL;
	char *searchstring = NULL;
	char* command_enviar=NULL;
	char* prefix = NULL;

	ret = IRCUserParse_List (command, &channel, &searchstring);
	g_print("\t command: %s \n", command);
	g_print("\t channel: %s \n", channel);
	g_print("\t searchstring: %s \n", searchstring);
	if(ret != IRC_OK){
		g_print("ERROR - pulist - IRCUserParse_List \n");
		return ERR;
	}

	ret = IRCMsg_List (&command_enviar, prefix, channel, searchstring);
	if(ret != IRC_OK){
		g_print("ERROR - pulist - IRCMsg_List \n");
		return ERR;
	}

	g_print("\t Mensaje a enviar command_enviar: %s \n",command_enviar);
	//sem_wait(&recepcionTCP);
	retorno = enviarDatos(sockfd_user,command_enviar, strlen(command_enviar));
	if(retorno == ERR){
		g_print("ERROR: IRCInterface_NewCommandText - enviarDatos - list\n");
		return ERR;
	}

	IRCInterface_PlaneRegisterOutMessage(command_enviar);
	mfree(4, command_enviar, channel, searchstring, prefix);
	return OK;
}


/**
* @brief Comando de usuario HELP
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int puhelp(char* command){
	char* comando;
	char command_enviar[MAXDATA];

	IRCUserParse_Help (command, &comando);
	sprintf(command_enviar, "HELP %s\r\n", comando?comando:"");
	enviarDatos(sockfd_user, command_enviar, strlen(command_enviar));	
	IRCInterface_PlaneRegisterOutMessage(command_enviar);
	mfree(1, comando);
	return OK;
}


/**
* @brief Comando desconocido para el cliente
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int pdefault(char* command){ 
	IRCInterface_WriteSystem(nick_user, "No se ha podido ejecutar el comando: ");
	IRCInterface_WriteSystem(nick_user, command);
	return 0;
}


/**
* @brief Comando de usuario PART
* Used to part (or leave) a channel you currently occupy.
* All those in the channel will be notified of your departure.
* If you specify a reason it will be displayed to the users on the channel
* -
* Syntax:  PART <chan>,<chan2>,<chan3>,<chan4> <reason>
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int pupart(char* command){
	char* channel;
	char command_enviar[MAXDATA];

	IRCUserParse_Part(command, &channel);
	sprintf(command_enviar, "PART %s :Saliendo\r\n", channel?channel:IRCInterface_ActiveChannelName());

	enviarDatos(sockfd_user, command_enviar, strlen(command_enviar));
	IRCInterface_PlaneRegisterOutMessage(command_enviar);
	mfree(1, channel);
	return OK;
}

/**
* @brief Comando de usuario AWAY
* 
* Sets your online status to "Away".
* -
* Syntax:  AWAY <reason> (Sets you Away with the reason given)
*          AWAY (Un-Sets you as Away)
* Example: AWAY Lunch time!
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int puaway(char* command){
	char* command_enviar;
	char *reason;
	int free_f = 0;

   	IRCUserParse_Away (command, &reason);
   	/*
   	if(reason == NULL || strlen(reason) == 0){
   		reason = "afk";
   		free_f = 1;
   	}*/

   	IRCMsg_Away (&command_enviar, NULL, reason);
	g_print("\t Mensaje a enviar command_enviar en AWAY: %s \n",command_enviar);

	enviarDatos(sockfd_user, command_enviar, strlen(command_enviar));
	IRCInterface_PlaneRegisterOutMessage(command_enviar);
	
	if(free_f)
		mfree(1, command_enviar);
	else
		mfree(2, command_enviar, reason);
	return OK;
}


/**
* @brief Comando de usuario NICK
* Changes your "Online Identity" on a server. All those in the channel you are in will be alerted of your nickname change.
* -
* Syntax:  NICK <new nickname>
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int punick(char* command){

	char* command_enviar;
	char *newnick;

   	IRCUserParse_Nick (command, &newnick);
   	IRCMsg_Nick (&command_enviar, NULL, newnick, NULL);
	g_print("\t Mensaje a enviar command_enviar en NICK: %s \n",command_enviar);

	enviarDatos(sockfd_user, command_enviar, strlen(command_enviar));
	IRCInterface_PlaneRegisterOutMessage(command_enviar);
	mfree(2, command_enviar, newnick);
	return OK;
}


/**
* @brief Comando de usuario INVITE
* Sends a user an Invitation to join a particular channel. If the channel is +i, you must be an Operator to use this
* command, otherwise any user may use the command.
* Invite without parameters lists the channels you have been invited to.
* -
* Syntax:  INVITE <user> <channel>
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int puinvite(char* command){

	char* command_enviar = NULL, *prefix = NULL, *nick = NULL, *channel = NULL;

	g_print("\t Mensaje reciido en UINVITE: %s \n",command);

	IRCParse_Invite (command, &prefix, &nick, &channel); //la parseo con esta funcion del server porque no hay del user
	g_print("\t prefix: %s \n",prefix);
	g_print("\t nick: %s \n",nick);
	g_print("\t channel: %s \n",channel);

	IRCMsg_Invite (&command_enviar, prefix, nick, channel);
	g_print("\t Mensaje a enviar en UINVITE: %s \n", command_enviar);

	enviarDatos(sockfd_user, command_enviar, strlen(command_enviar));
	IRCInterface_PlaneRegisterOutMessage(command_enviar);
	mfree(4, command_enviar, channel, nick, prefix);
	return OK;
}


/**
* @brief Comando de usuario WHOIS
* Shows information about the user in question, such as their "Name", channels they are currently in, their hostmask, etc.
* -
* Syntax:  WHOIS <user>
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int puwhois(char* command){

	char command_enviar[MAXDATA];
	char* nick = NULL;
	char whois[200];

	g_print("\t Mensaje recibido en UWHOIS: %s \n", command);
	IRCUserParse_Whois (command, &nick);

	g_print("\t nick: %s \n", nick);
	sprintf(command_enviar, "WHOIS %s\n\r", nick );
	g_print("\t Mensaje a enviar en UWHOIS: %s \n", command_enviar);

	enviarDatos(sockfd_user, command_enviar, strlen(command_enviar));
	IRCInterface_PlaneRegisterOutMessage(command_enviar);

	strcpy(whois,"WHOIS ");
	strcat(whois, nick);
	IRCInterface_WriteSystem_Pretty("*", "------------------------------");
	IRCInterface_WriteSystem_Pretty("*", whois);
	mfree(1, nick);
	return OK;
}
 
/**
* @brief Comando de usuario LUSERS
* Provides Local and Global user information (Such as Current and Maximum user count).
* -
* Syntax: LUSERS [server]
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int pulusers(char* command){

	char command_enviar[MAXDATA];
	char* server;
	char lusers[200];

	g_print("\t Mensaje recibido en ULUSERS: %s \n", command);
	IRCUserParse_Lusers (command, &server);

	g_print("\t server: %s \n", server);
	sprintf(command_enviar, "LUSERS %s\n\r", server );
	g_print("\t Mensaje a enviar en ULUSERS: %s \n", command_enviar);

	enviarDatos(sockfd_user, command_enviar, strlen(command_enviar));
	IRCInterface_PlaneRegisterOutMessage(command_enviar);

	strcpy(lusers,"LUSERS ");
	strcat(lusers, server);
	IRCInterface_WriteSystem_Pretty("*", "------------------------------");
	IRCInterface_WriteSystem_Pretty("*", lusers);
	mfree(1, server);
	return OK;
}


/**
* @brief Comando de usuario MOTD
* Displays the Message Of The Day of the IRC Server you are logged onto.
* -
* Syntax: MOTD
*         MOTD <server>
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int pumotd(char* command){

	char command_enviar[MAXDATA];
	char* server;

	g_print("\t Mensaje recibido en UMOTD: %s \n", command);
	IRCUserParse_Motd (command, &server);
	g_print("\t server: %s \n", server);
	sprintf(command_enviar, "MOTD %s\n\r", server );
	g_print("\t Mensaje a enviar en UMOTD: %s \n", command_enviar);

	enviarDatos(sockfd_user, command_enviar, strlen(command_enviar));
	IRCInterface_PlaneRegisterOutMessage(command_enviar);
	mfree(1, server);
	return OK;
}


/**
* @brief Comando de usuario NOTICE
* Send a notice to a user, channel or server.
* -
* NOTICE <nick> <text>
* Send a notice to a user.
* Ex: /NOTICE Blah hi, how are you?
* -
* NOTICE <#channel> <text>
* Send a notice to a channel.
* Ex: /NOTICE #room Hi all, this is annoying
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int punotice(char* command){

	char *command_enviar, mensaje[MAXDATA];
	char* target, *msg;

	g_print("\t Mensaje recibido en UNOTICE: %s \n", command);
	IRCUserParse_Notice (command, &target, &msg);

	IRCMsg_Notice (&command_enviar, NULL, target, msg);

	g_print("\t Mensaje a enviar en UNOTICE: %s \n", command_enviar);
	enviarDatos(sockfd_user, command_enviar, strlen(command_enviar));
	IRCInterface_PlaneRegisterOutMessage(command_enviar);

	if(target[0] == '#'){
		sprintf(mensaje, ">%s/%s<", nick_user, target);
		IRCInterface_WriteChannel (target, mensaje, msg);
	} else {
		sprintf(mensaje, ">%s<", nick_user);		
		IRCInterface_AddNewChannel (target, 0);
		IRCInterface_WriteChannel (target, mensaje, msg);
	}
	mfree(3, command_enviar, target, msg);
	return OK;
}


/**
* @brief Comando de usuario CYCLE
* Cycles the given channel(s). This command is equivilent to sending a PART then a JOIN command.
* -
* Syntax:  CYCLE <chan1>,<chan2>,<chan3> ...
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int pucycle(char* command){

   	char* respuesta = NULL;
    char** target;
    int numchannels=0;
    int i ;

    IRCUserParse_Cycle (command, &target, &numchannels);
    for (i = 0; i< numchannels; i++){
    	IRCMsg_Part (&respuesta, NULL, target[i], "Saliendo");
		enviarDatos(sockfd_user, respuesta, strlen(respuesta));
    	IRCInterface_PlaneRegisterOutMessage (respuesta);
    	free(respuesta);
    	IRCMsg_Join(&respuesta, NULL, target[i], NULL, NULL);
		enviarDatos(sockfd_user, respuesta, strlen(respuesta));
    	IRCInterface_PlaneRegisterOutMessage (respuesta);
    	free(respuesta);
    }

    return OK;   
}


/**
* @brief Comando de usuario QUERY
* Use the "/query <user>" command to specify that every message you type should be directed to a single user. 
* @param command cadena introducida por el usuario en el campo de texto
* @return OK si todo es correcto, ERR si se produce un error
*/
int puquery(char* command){

	char* nickorchannel, *msg;

   	IRCUserParse_Query (command, &nickorchannel, &msg);
	g_print("\t Mensaje recibido en UQUERY: %s \n", command);
	g_print("\t nickorchannel: %s \n", nickorchannel);
	g_print("\t msg: %s \n", msg);

	if(nickorchannel != NULL){
		IRCInterface_AddNewChannel (nickorchannel, 0);
	}
	mfree(2, nickorchannel, msg);
    return OK;
}