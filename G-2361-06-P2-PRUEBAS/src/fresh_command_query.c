/**
* @brief Parsea los mensajes y respuestas que recibe del servidor
* @param massage mensaje recibido para procesar
* @return OK si todo fue correcto y ERR si ocurrio un error
*/
int command_query(char *message){

	long ret = -1;
	int retorno = -1;
	char *prefix = NULL;
	int n=-1;
	int counter = 1;
    char mensaje[MAXDATA] = {0};
    char mensajeLargo[10*MAXDATA] = {0};
    char *msg = NULL;
    char *nick = NULL;
    char *nick2 = NULL;
    //002
    char *servername = NULL;
	char *versionname = NULL;
	//003
	char *timedate = NULL;
	//004
	char* version = NULL;
	char* availableusermodes = NULL;
	char* availablechannelmodes = NULL;
	char* addedg = NULL;	
	//251
	int nusers = 0;
	int ninvisibles = 0;
	int nservers = 0;
	char *type = NULL;
	//char **parameters = NULL;
	//int numparameters = 0;

	//322
	char *visible = NULL;
	//332
	char *topic = NULL;
	char *channel = NULL;

	//353
	//char* show_nick;

	//JOIN
	char* key = NULL;	
	char command[MAXDATA];

	//PRIVMSG
	char* msgtarget = NULL;
	char* origin_nick = NULL;
	char nick_privmsg[MAXDATA];
	char *filename = NULL, *hostname_destino = NULL;
	unsigned long length, port;

	//PART
	char* nick_part;
	char* username_part;
	char* host_part;
	char* server_part;

	//PING
	char* server, *server2;
	char* command_pong;

	//Strtok
	const char s[2] = ":";
	char *token = NULL;

	//472
	char *modechar = NULL;

	//MODE
	char *channeluser = NULL;
	char *mode = NULL;
	char *user = NULL;

	//KICK
	char *comment = NULL;

	//QUIT
	char **channelsQuit;
	int numChannelsQuit;
	int i;
	//char* realname, *host;

	g_print("Mesaje recibido en command_query: %s \n", message);

	if(message == NULL) {
		g_print("ERROR - Message NULL en command_query \n\n");
		return ERR;
	}

	/*
	IRCInterface_PlaneRegisterInMessage(message);

	switch(IRC_CommandQuery(message)){
		case RPL_WELCOME: //001
			ret = IRCParse_RplWelcome(message, &prefix, &nick2, &msg);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_RplWelcome\n");
				//return IRCERR_NOCONNECT;
			}
			g_print("Comandos recibidos en el IRCParse_RplWelcome: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick2: %s \n",nick2);
			g_print("\t msg: %s \n\n",msg);
			//obtenemos el hostname, util para el envio de ficheros
			hostname = strtok(msg, " ");
			while (((hostname = strtok(NULL, " ")) != NULL) && (counter < 5)){
				counter++;
			}
			hostname = strtok(NULL, " ");
			g_print("\t hostname: %s \n\n", hostname);
			IRCInterface_WriteSystemThread("*", msg);
			break;

		case RPL_YOURHOST:	//002
			//long IRCParse_RplYourHost (char *strin, char **prefix, char **nick, char **msg, char **servername, char **versionname)
			ret = IRCParse_RplYourHost(message, &prefix, &nick2, &msg, &servername, &versionname);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_RplYourHost\n");
				//return IRCERR_NOCONNECT;
			}
			g_print("Comandos recibidos en el IRCParse_RplYourHost: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick2: %s \n",nick2);
			g_print("\t servername: %s \n",servername);
			g_print("\t versionname: %s \n",versionname);		
			g_print("\t msg: %s \n\n",msg);
			IRCInterface_WriteSystemThread("*",msg);
			break;
		case RPL_CREATED://003			
			//long IRCParse_RplCreated (char *strin, char **prefix, char **nick,char **timedate, char **msg)
			ret = IRCParse_RplCreated(message, &prefix, &nick2, &timedate, &msg);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_RplCreated\n");
				//return IRCERR_NOCONNECT;
			}
			g_print("Comandos recibidos en el IRCParse_RplCreated: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick2: %s \n",nick2);
			g_print("\t timedate: %s \n",timedate);	
			g_print("\t msg: %s \n\n",msg);
			IRCInterface_WriteSystemThread("*",msg);
			break;

		case RPL_MYINFO: //004
			//long IRCParse_RplMyInfo (char *strin, char **prefix, char **nick, char **servername, char **version, char **availableusermodes, char **availablechannelmodes, char **addedg)
			ret = IRCParse_RplMyInfo(message, &prefix, &nick2, &servername, &version, &availableusermodes, &availablechannelmodes, &addedg);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_RplMyInfo\n");
				//return IRCERR_NOCONNECT;
			}
			g_print("Comandos recibidos en el IRCParse_RplMyInfo: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick2: %s \n",nick2);
			g_print("\t servername: %s \n",servername);
			g_print("\t version: %s \n",version);
			g_print("\t availableusermodes: %s \n",availableusermodes);	
			g_print("\t availablechannelmodes: %s \n",availablechannelmodes);
			g_print("\t addedg: %s \n\n",addedg);
			n = snprintf(mensaje, sizeof mensaje,"%s %s %s %s %s ",servername,version,availableusermodes,availablechannelmodes,addedg);

			if ( n < 0 || n >= sizeof mensaje ){
				g_print("Error en sprintf \n");
    			return ERR;    // or other error handling
			}
			IRCInterface_WriteSystemThread("*",mensaje);		
			break;

		case RPL_BOUNCE: //005
			//   long IRCParse_RplISupport (char *strin, char **prefix, char **nick, char **msg)						
			ret = IRCParse_RplISupport(message, &prefix, &nick2, &msg);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_RplISupport\n");
				//return IRCERR_NOCONNECT;
			}
			g_print("Comandos recibidos en el IRCParse_RplISupport: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick2: %s \n",nick2);
			g_print("\t msg: %s \n\n",msg);
			IRCInterface_WriteSystemThread("*",msg);		
			break;

		case RPL_LUSERCLIENT: //251
			//long IRCParse_RplLuserClient (char *strin, char **prefix, char **nick, char **msg, int *nusers, int *ninvisibles, int *nservers)
			
			ret = IRCParse_RplLuserClient(message, &prefix, &nick2, &msg, &nusers, &ninvisibles, &nservers);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_RplLuserClient\n");
				//return IRCERR_NOCONNECT;
			}
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick2: %s \n",nick2);
			g_print("\t msg: %s \n",msg);
			g_print("\t nusers: %d \n",nusers);
			g_print("\t ninvisibles: %d \n",ninvisibles);
			g_print("\t nservers: %d \n\n",nservers); 

			//ret_strstr = strstr(mensaje,"nicknick");
			//IRCInterface_WriteSystem("*",ret_strstr);

			sprintf(mensaje,"There are %d users and %d invisibles on %d servers ",nusers,ninvisibles,nservers);
			//sprintf(mensaje,"There are 13 users and 0 services on 1 servers");

			IRCInterface_WriteSystemThread("*",mensaje);													
			break;
			
		case RPL_LUSERCHANNELS: //254
			g_print("\t message: %s \n",message);
			//Coger el primer token
			token = strtok(message,s);
			//Ir por el resto
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);	
			break;

		case RPL_LUSERME : //255
			g_print("\t message: %s \n",message);
			//Coger el primer token
			token = strtok(message,s);
			//Ir por el resto
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);	
			break;

		case RPL_LOCALUSERS: //265
			g_print("\t message: %s \n",message);
			//Coger el primer token
			token = strtok(message,s);
			//Ir por el resto
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);		
			break;

		case RPL_GLOBALUSERS: //266
			g_print("\t message: %s \n",message);
			//Coger el primer token
			token = strtok(message,s);
			//Ir por el resto
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);			
			break;

		//case 250: 250 RPL_STATSDLINE
			//IRCInterface_WriteSystem("*",message);
			//break; 

		case RPL_LIST: //322
			//   long IRCParse_RplList (char *strin, char **prefix, char **nick, char **channel, char **visible, char **topic)
			ret = IRCParse_RplList(message, &prefix, &nick2, &channel, &visible, &topic);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_RplList\n");
				//return IRCERR_NOCONNECT;
			}
			g_print("Comandos recibidos en el IRCParse_RplList: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick2: %s \n",nick2);
			g_print("\t channel: %s \n",channel);
			g_print("\t visible: %s \n",visible);
			g_print("\t topic: %s \n\n",topic);

			sprintf(mensajeLargo,"%s \t %s \t %s",channel,visible,topic);
			g_print("Mensaje creado: %s \n\n",mensajeLargo);

			IRCInterface_WriteSystemThread("*",mensajeLargo);
			mfree(5,prefix,nick2,channel,visible,topic);	
			break;

		case RPL_LISTEND: //323
			g_print("\t message: %s \n",message);
			//Coger el primer token
			token = strtok(message,s);
			//Ir por el resto
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);			
			break;

		case RPL_MOTDSTART: //375
			g_print("\t message: %s \n",message);
			//Coger el primer token
			token = strtok(message,s);
			//Ir por el resto
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);			
			break;
		case RPL_MOTD: //372
			g_print("\t message: %s \n",message);
			//Coger el primer token
			token = strtok(message,s);
			//Ir por el resto
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);		
			break;
		case RPL_ENDOFMOTD: //376
			g_print("\t message: %s \n",message);
			//Coger el primer token
			token = strtok(message,s);
			//Ir por el resto
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);	
			return 19;//cambiar por un define
			break;

		case RPL_TOPIC: //332
			g_print("\n=======CASE RPL_TOPIC=======\n");
			//IRCParse_RplTopic (char *strin, char **prefix, char **nick, char **nick2, char **channel, char **msg)
			ret = IRCParse_RplTopic(message, &prefix, &nick, &channel, &topic);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_RplTopic\n");
				//return IRCERR_NOCONNECT;
			}
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick: %s \n",nick);
			g_print("\t channel: %s \n",channel);
			g_print("\t topic: %s \n\n",topic);
			sprintf(mensaje,"El topic para %s es %s ",channel,topic);
			g_print("Mensaje: %s \n",mensaje);
			g_print("Existe canal: %d \n", IRCInterface_QueryChannelExistThread(channel));

			IRCInterface_WriteChannelThread(channel,"*",mensaje);	
			break;

		case RPL_AWAY: //306
			g_print("\n=======CASE RPL_AWAY=======\n");
			//>> :irc.eps.net 306 gomupo :You have been marked as being away
			IRCParse_RplAway (message, &prefix, &nick, &nick2, &msg);
			IRCInterface_WriteSystemThread("*",msg);
			break;

		case TOPIC: //332
			g_print("\n=======CASE TOPIC=======\n");
			//   long IRCParse_Topic (char *strin, char **prefix, char **channel, char **topic)
			ret = IRCParse_Topic (message, &prefix, &channel, &topic);
			if(ret != IRC_OK){
				g_print("ERROR: IRC_CommandQuery - IRCParse_Topic\n");
				return ERR;
				//return IRCERR_NOCONNECT;
			}
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t channel: %s \n",channel);
			g_print("\t topic: %s \n\n",topic);
			sprintf(mensaje,"El topic para %s es %s \n",channel,topic);
			g_print("Mensaje: %s \n",mensaje);
			g_print("Existe canal: %d \n", IRCInterface_QueryChannelExistThread(channel));

			IRCInterface_WriteChannelThread(channel,"*",mensaje);
			break;

		case RPL_NOTOPIC:
			break;

		case RPL_TOPICWHOTIME: //333
			break;					

		case RPL_NAMREPLY: //353
			//long IRCParse_RplNamReply (char *strin, char **prefix, char **nick, char **type, char **channel, char **msg)
			ret = IRCParse_RplNamReply(message, &prefix, &nick, &type, &channel, &msg);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_RplNamReply\n");
				//return IRCERR_NOCONNECT;
			}
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick: %s \n",nick);
			g_print("\t type: %s \n",type);								
			g_print("\t channel: %s \n",channel);
			g_print("\t msg: %s \n\n",msg);

			//Ojo, que pasa si es names?? sin join
			//Añadir los nicks a la ventana de lad erecha. Pillarlos del WHO que se envía 
			//despues del join.
			//OJO es una prueba del funcionamineto de IRCInterface_AddNickChannel,
			//los nicks deberían de ser partidos mediante uso strtok o algo parecido
			//IRCInterface_AddNickChannel (channel, msg, msg, msg, msg, VOICE);
			//sprintf(mensaje,"Usuarios en %s: %s",channel,msg);
			//IRCInterface_WriteChannelThread(channel,"*",mensaje);
			interface_mostrar_nicks(channel,msg);	
			break;

		case RPL_ENDOFNAMES: //366
			g_print("Mensaje recibido en RPL_ENDOFNAMES: \n");
			//long IRCParse_RplEndOfNames (char *strin, char **prefix, char **nick, char **channel, char **msg)
			ret = IRCParse_RplEndOfNames(message, &prefix, &nick2, &channel, &msg);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_RplEndOfNames\n");
				//return IRCERR_NOCONNECT;
			}
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick: %s \n",nick);							
			g_print("\t channel: %s \n",channel);
			g_print("\t msg: %s \n\n",msg);

			//IRCInterface_WriteChannelThread(channel,"*",msg);
			break;	

		case JOIN:
			g_print("entre en JOIN con message = %s\n", message);
			ret = IRCParse_Join (message, &prefix, &channel, &key, &msg);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_NewCommandText - UJOIN - IRCParse_Join\n");
				return -1;
			}
			g_print("Comandos recibidos en el IRCParse_Join: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t channel: %s \n",channel);
			g_print("\t key: %s \n",key);
			g_print("\t msg: %s \n\n",msg);

			IRCInterface_AddNewChannelThread(msg, 0);
			IRCParse_ComplexUser(prefix, &nick_part, &username_part, &host_part, &server_part);
			sprintf(mensaje, "%s se ha unido al canal", nick_part);
			if(!strcmp(nick_user, nick_part)){
				IRCInterface_WriteChannelThread(msg,"*", "Bienvenido al canal");
			} else {
				IRCInterface_WriteChannelThread(msg,"*", mensaje);
			}
			//Actualizar al lista de usuarios
			sprintf(mensaje,"/names %s",msg);
			retorno = punames(mensaje);
			if(retorno == ERR){
				g_print("ERROR - JOIN - punames");
				return ERR;				
			}
			break;

		case NAMES:
			g_print("Case NAMES\n");
			break;

		case PRIVMSG:
			g_print("he entrado en PRIVMSG:)\n");
			ret = IRCParse_Privmsg(message, &prefix, &msgtarget, &msg);
			if(ret != IRC_OK){
				g_print("ERROR - receive_messages - IRCParse_Privmsg");
				return ERR;
			}
			//>> :gomupo!~gonzalo@119.181.218.87.dynamic.jazztel.es PRIVMSG gon :hola
			if ((origin_nick = strtok(prefix, "!")) != NULL){
				strcpy(nick_privmsg, origin_nick);
			}
			g_print("\t nick_privmsg: %s \n",nick_privmsg);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t msgtarget: %s \n",msgtarget);
			g_print("\t msg: %s \n\n",msg);

			if(msg[0] == 1 || (msg[0] == ':' || msg[1] == 1)) { //envio de ficheros
				g_print("alguien quiere enviarme un fichero\n");
				//char *msg = "\001FSEND gato.jpg ~cgs@cliente168.wlan.uam.es 1234 24422";
				
				filename = strtok(msg, " ");
				filename = strtok(NULL, " ");
				hostname_destino = strtok(NULL, " ");
				port = (unsigned long) atol(strtok(NULL, " "));
				length = (unsigned long) atol(strtok(NULL, " "));
				g_print("filename: %s \n",filename);
				g_print("hostname_destino: %s \n",hostname_destino);
				g_print("length: %ld \n",length);
				g_print("port: %ld \n",port);

				if(IRCInterface_RecibirDialogThread(nick_user, filename) == TRUE){
					g_print("Lanzamos el hilo que guarda el archivo\n");
					pthread_t tid;

					File_args args;
					args.hostname = hostname_destino;
					args.filename = filename;
					args.port = port;
					args.length = length;

					if(pthread_create( &tid, NULL, (void*) save_file, (void*) &args) < 0){
				    	g_print("Error en la llamada a save_file\n");
						return ERR;
					}
					break;
				}
				
				//if (sscanf(msg, "\001AUDIOCHAT %ms %li", &hostname_destino, &port) > 0) {
				//	g_print("host=%s, port=%li\n", hostname_destino, port);
				//} else{
				//	g_print("Formato incorrecto en envio de fichero\n");
				//}
				ret = clienteEnviarDatosTCP(sockfd_user, command, strlen(command));
				if(ret == ERR){
					g_print("ERROR: IRCInterface_NewCommandText - clienteEnviarDatosTCP - Join\n");
					return -1;
				}
			}

			if(msgtarget[0] != '#'){//no grupo
				IRCInterface_AddNewChannelThread(nick_privmsg, 0);				
				IRCInterface_WriteChannelThread(nick_privmsg, nick_privmsg, msg);
				return OK;
			}

			IRCInterface_WriteChannelThread(msgtarget, nick_privmsg, msg);
			break;

		case PART:
			g_print("entre en PART con message = %s\n", message);
			ret = IRCParse_Part (message, &prefix, &channel, &msg);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_NewCommandText - UJOIN - IRCParse_Join\n");
				return -1;
			}
			g_print("Comandos recibidos en el IRCParse_PART: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t channel: %s \n",channel);
			g_print("\t msg: %s \n\n",msg);

			IRCParse_ComplexUser(prefix, &nick_part, &username_part, &host_part, &server_part);
			IRCInterface_DeleteNickChannelThread(channel, nick_part);
			sprintf(mensaje, "El usuario %s ha salido del grupo (%s)",nick_part, msg);
			IRCInterface_WriteChannelThread (channel,"*",mensaje);
			if(strcmp(nick_user, nick_part) == 0){
				IRCInterface_RemoveChannelThread(channel);
			} else {
				sprintf(mensaje,"/names %s",channel);
				retorno = punames(mensaje);
				if(retorno == ERR){
					g_print("ERROR - JOIN - punames");
					return ERR;				
				}
			}

			break;
		case NOTICE:
			g_print("\n=======CASE NOTICE=======\n");
			g_print("entre en NOTICE con message = %s\n", message);

			IRCParse_Notice(message, &prefix, &msgtarget, &msg);

			g_print("prefix = %s\n", prefix);
			g_print("msgtarget = %s\n", msgtarget);
			g_print("msg = %s\n", msg);

			if((!strcmp(msgtarget, nick_user)) && (strcmp(prefix,"irc.eps.net") != 0)) {
				IRCParse_ComplexUser(prefix, &nick_part, &username_part, &host_part, &server_part);				
				sprintf(mensaje, ">%s<", nick_part);
				IRCInterface_AddNewChannelThread(nick_part, 0);
				IRCInterface_WriteChannelThread (nick_part,mensaje,msg);
			} else if(msgtarget[0] == '#'){
				IRCParse_ComplexUser(prefix, &nick_part, &username_part, &host_part, &server_part);
				sprintf(mensaje, ">%s/%s<", nick_part, msgtarget);
				IRCInterface_AddNewChannelThread(msgtarget, 0);
				IRCInterface_WriteChannelThread (msgtarget,mensaje,msg);
			} else {//AUTH			
				IRCInterface_WriteSystemThread ("*",msg);
			}
			//IRCInterface_WriteSystemThread("*",msg);
			break;		
		case PING:
			g_print("entre en PING con message = %s\n", message);
			//:irc.eps.net PONG irc.eps.net :LAG1460877705692323
			IRCParse_Ping (message, &prefix, &server, &server2, &msg);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t server: %s \n",server);
			g_print("\t server2: %s \n",server2);
			g_print("\t msg: %s \n\n",msg);
			IRCMsg_Pong(&command_pong, prefix?prefix:hostname, server?server:"", server2?server2:"", msg?msg:"");
			clienteEnviarDatosTCP(sockfd_user, command_pong, strlen(command_pong));
			g_print("Pong enviado\n");
			IRCInterface_PlaneRegisterOutMessageThread(command_pong);
			break;

		case KICK:
			g_print("\n=======CASE KICK=======\n");
			//long IRCParse_Kick (char *strin, char **prefix, char **channel, char **user, char **comment)

			ret = IRCParse_Kick(message, &prefix, &channel, &user, &comment);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_Kick\n");
				return ERR;
			}
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t channel: %s \n",channel);
			g_print("\t user: %s \n",user);
			g_print("\t comment: %s \n\n",comment);

			sprintf(mensaje,"%s Ha sido echado de %s con mensaje/motivo(%s)",user, channel, comment);
			
			g_print("Mensaje: %s \n",mensaje);
			IRCInterface_WriteChannelThread(channel,"*",mensaje);

			//Actualizar al lista de usuarios
			IRCInterface_DeleteNickChannelThread (channel, user);
			if(!strcmp(nick_user,user)){
				IRCInterface_RemoveChannelThread(channel);
			}
			//memset(mensaje,0,MAXDATA);
			//sprintf(mensaje,"/names %s",channel);
			//retorno = punames(mensaje);
			//if(retorno == ERR){
			//	g_print("ERROR - MODE - punames");
			//	return ERR;				
			//} 
			break;		

		case ERR_UNKNOWNMODE: //472
			g_print("Entre en ERR_UNKNOWNMODE con message = %s\n", message);
			//      long IRCParse_ErrUnknownMode (char *strin, char **prefix, char **nick, char **modechar, char **channel, char **msg)
			ret = IRCParse_ErrUnknownMode(message, &prefix, &nick, &modechar, &channel, &msg);
			if(ret != IRC_OK){
				g_print("ERROR: IRC_CommandQuery - ERR_UNKNOWNMODE - IRCParse_ErrUnknownMode\n");
				//return -1;
			}
			g_print("Comandos recibidos en el IRCParse_ErrUnknownMode: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick: %s \n",nick);
			g_print("\t modechar: %s \n",nick);						
			g_print("\t channel: %s \n",channel);
			g_print("\t msg: %s \n\n",msg);		

			sprintf(mensaje,"%s %s\n",channel,msg);
			IRCInterface_WriteChannelThread(channel,"*",mensaje);
			break;

		case MODE: //
			g_print("\n=======CASE MODE=======\n");
			//    long IRCParse_Mode (char *strin, char **prefix, char **channeluser, char **mode, char **user)
			ret = IRCParse_Mode(message, &prefix, &channeluser, &mode, &user);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_RplTopic\n");
				return ERR;
			}
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t channeluser: %s \n",channeluser);
			g_print("\t mode: %s \n",mode);
			g_print("\t user: %s \n\n",user);

			nick = strtok(prefix,"!"); //Coger el usuario que ha mandando el mode
			if(user != NULL){ //Modo usuario


				//Comprobar si es k o l 
				if(strcasecmp(mode,"+k") == 0){
					sprintf(mensaje,"%s establece contraseña del canal %s como: %s ",nick, channeluser, user);
					g_print("Mensaje: %s \n",mensaje);
					IRCInterface_WriteChannelThread(channeluser,"*",mensaje);
					break;		
				}else if(strcasecmp(mode,"+l") == 0){
					sprintf(mensaje,"%s establece límite del canal %s a %s usuarios",nick, channeluser, user);
					g_print("Mensaje: %s \n",mensaje);
					IRCInterface_WriteChannelThread(channeluser,"*",mensaje);
					break;						
				}

				sprintf(mensaje,"%s establece el modo %s al usuario %s en el canal %s ",nick, mode, user, channeluser);
				g_print("Mensaje: %s \n",mensaje);
				IRCInterface_WriteChannelThread(channeluser,"*",mensaje);

				// No funciona IRCInterface_ChangeNickStateChannel :(
				//if(strcasecmp(mode,"+o") == 0){
				//	g_print("modo op\n");
				//	IRCInterface_ChangeNickStateChannel (channeluser, nick, OPERATOR);
				//}else if(strcasecmp(mode,"+v") == 0){
				//	g_print("modo voice\n");
				//	IRCInterface_ChangeNickStateChannel (channeluser, nick, VOICE);
				//}else{
				//	g_print("else otro modo\n");

					//Actualizar la lista de usuarios
					memset(mensaje,0,MAXDATA);
					sprintf(mensaje,"/names %s",channeluser);
					retorno = punames(mensaje);
					if(retorno == ERR){
						g_print("ERROR - MODE - punames");
						return ERR;				
					}					
				//}				
			}else{
				sprintf(mensaje,"%s establece modo %s %s ", nick, channeluser, mode);
				g_print("Mensaje: %s \n",mensaje);
				IRCInterface_WriteChannelThread(channeluser,"*",mensaje);
			}			
			break;

		case INVITE:			
			g_print("\n=======CASE INVITE=======\n");
			IRCInterface_WriteSystemThread("*",message);
			break;

		case NICK:
			g_print("\n=======CASE NICK=======\n");
			//<< NICK gomupo2
			//>> :gomupo!~gonzalo@cliente020.wlan.uam.es NICK :gomupo2
			IRCParse_Nick (message, &prefix, &nick, &msg);
			IRCParse_ComplexUser(prefix, &nick_part, &username_part, &host_part, &server_part);
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick: %s \n",nick); //esta a null
			g_print("\t msg: %s \n",msg); //el nick viene aqui
			IRCInterface_ChangeNickThread(nick_part, msg);
			sprintf(mensaje, "%s ahora es conocido como %s", nick_part, msg);
			if(!strcmp(nick_part, nick_user)){
				strcpy(nick_user,msg);			
			}
			IRCInterface_WriteSystemThread("*",mensaje);	
			g_print("new nick: %s\n", msg);

			break;

		case QUIT:
			g_print("\n=======CASE QUIT=======\n");
			IRCParse_Quit (message, &prefix, &msg);
			IRCParse_ComplexUser(prefix, &nick_part, &username_part, &host_part, &server_part);
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t msg: %s \n",msg);
			sprintf(mensaje, "El usuario %s se ha desconectado", nick_part);
			IRCInterface_WriteSystemThread("*",mensaje);

			IRCInterface_ListAllChannelsThread(&channelsQuit, &numChannelsQuit);

			for(i=0; i<numChannelsQuit; i++){
				sprintf(mensaje,"/names %s",channelsQuit[i]);
				retorno = punames(mensaje);
				if(retorno == ERR){
					g_print("ERROR - JOIN - punames");
					return ERR;				
				}
				//IRCInterface_GetNickChannelInfoThread(nick_part, channelsQuit[i], &user, &realname, &host);
				//if(nick_part != NULL){
				//	IRCInterface_DeleteNickChannelThread (channelsQuit[i], nick_part);
				//}
			}

			

			break;

		case RPL_CHANNELMODEIS: //324 	
			g_print("\n=======CASE RPL_CHANNELMODEIS=======\n");
			//long IRCParse_RplChannelModeIs(char *strin, char **prefix, char **nick, char **channel, char **modetxt)
			//ret = IRCParse_RplChannelModeIs(message, &prefix, &channeluser, &mode, &user);
			//if(ret != IRC_OK){
			//	g_print("ERROR: IRCInterface_Connect - IRCParse_RplTopic\n");
			//	return ERR;
			//}
			//g_print("\t message: %s \n",message);
			//g_print("\t prefix: %s \n",prefix);
			//g_print("\t channeluser: %s \n",channeluser);
			//g_print("\t mode: %s \n",mode);
			//g_print("\t user: %s \n\n",user);

			//sprintf(mensaje,"%s establece modo %s %s ", user, channeluser, mode);
			
			//g_print("Mensaje: %s \n",mensaje);
			//IRCInterface_WriteChannelThread(channeluser,"*",mensaje);

			//Actualizar al lista de usuarios
			break;	

		case ERR_CHANOPRIVSNEEDED: //482
			g_print("Entre en ERR_CHANOPRIVSNEEDED con message = %s\n", message);
			//   long IRCParse_ErrChanOPrivsNeeded (char *strin, char **prefix, char **nick, char **channel, char **msg)
			ret = IRCParse_ErrChanOPrivsNeeded(message, &prefix, &nick, &channel, &msg);
			if(ret != IRC_OK){
				g_print("ERROR: IRC_CommandQuery - ERR_CHANOPRIVSNEEDED - IRCParse_ErrChanOPrivsNeeded\n");
				//return ERR;
			}
			g_print("Comandos recibidos en el IRCParse_ErrChanOPrivsNeeded: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick: %s \n",nick);			
			g_print("\t channel: %s \n",channel);
			g_print("\t msg: %s \n\n",msg);		

			sprintf(mensaje,"%s %s\n",channel,msg);
			IRCInterface_WriteChannelThread(channel,"*",mensaje);
			break;	

		default:
			g_print("Mensaje desconocido en command_query: %s \n", message);
			IRCInterface_WriteSystemThread("*",message);
			return OK;
	}
	*/
	//Liberamos memoria
	//mfree(33,msg,nick,nick2,servername,versionname,timedate,version,availableusermodes,availablechannelmodes,addedg,type,visible,topic,channel,key,
	//msgtarget,origin_nick,filename,hostname_destino,nick_part,username_part,host_part,server_part,server,server2,command_pong,token,modechar,channeluser,
	//mode,user,comment,realname,host);
	return OK;

}