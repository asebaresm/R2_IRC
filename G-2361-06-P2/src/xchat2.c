/**
 * @file xchat2.c
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 20/03/17
 * @brief Fichero con principalmente la implementación de los callbacks de xchat2.
 *
 */

#include "../includes/xchat2.h"

pthread_mutex_t loglock;

/**
* Globales útiles sobre la conexión del cliente con el servidor
*/
//como global para que el hilo de interfaz pueda matar al hilo de recibir mensajes
pthread_t recv_tid;
pthread_t sendf_tid;

int sockfd_user = -1;
char nick_user[MAXDATA] = {0};
char* hostname;
char* active_channel = NULL;
char* stream;

char host_name[128]; //aux, uso en libreria TCP

//int 1024B_flag = 0;
char last_command[256] = ""; 		//almacena el ultimo 'message' pasado a unpipe()
int check_next_unpipe = 0;		//0 no check, 1 check
int last_test = OK; 			//0 OK, -1 ERR

/**
* @brief Parsea los mensajes y respuestas que recibe del servidor
* @param massage mensaje recibido para procesar
* @return OK si todo fue correcto y ERR si ocurrio un error
*/
int command_query(char *message){

	long ret = -1;
	int retorno = -1;
	char *prefix = NULL;
	int n = -1;
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

	//265
	char *substring = NULL;
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

	//GENERAL
	char **params;
	int n_params;
	int unknw_type;
	const char space_delim[2] = " ";
	char *message_cp = NULL;

	//g_print("Mesaje recibido en command_query: %s", message);

	/*
	if(message == NULL) {
		g_print(RED "ERROR - In command_query: message == NULL al principio\n\n" RESET);
		return ERR;
	}
	*/

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
			IRCInterface_WriteSystemThread_Pretty("*", msg);
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
			IRCInterface_WriteSystemThread_Pretty("*",msg);
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
			IRCInterface_WriteSystemThread_Pretty("*",msg);
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
			IRCInterface_WriteSystemThread_Pretty("*",mensaje);		
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
			IRCInterface_WriteSystemThread_Pretty("*",msg);		
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

			/*ret_strstr = strstr(mensaje,"nicknick");
			IRCInterface_WriteSystem("*",ret_strstr);*/ 

			sprintf(mensaje,"There are %d users and %d invisibles on %d servers ",nusers,ninvisibles,nservers);
			//sprintf(mensaje,"There are 13 users and 0 services on 1 servers");

			IRCInterface_WriteSystemThread_Pretty("*",mensaje);													
			break;
			
		case RPL_LUSERCHANNELS: //254
			g_print("\t message: %s \n",message);
			/*Coger el primer token*/
			token = strtok(message,s);
			/*Ir por el resto*/
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread_Pretty("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);	
			break;

		case RPL_LUSERME : //255
			g_print("\t message: %s \n",message);
			/*Coger el primer token*/
			token = strtok(message,s);
			/*Ir por el resto*/
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread_Pretty("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);	
			break;

		case RPL_LOCALUSERS: //265
			substring = strnext(message, ':');
			if (substring){
				substring = strnext(substring, ':');
			}
			IRCInterface_WriteSystemThread_Pretty("*",substring);

			substring = NULL;
			break;

		case RPL_GLOBALUSERS: //266
			g_print(GRN "\n>> [server command] RPL_GLOBALUSERS - message = %s\n" RESET, message);
			substring = strnext(message, ':');
			if (substring){
				substring = strnext(substring, ':');
			}
			IRCInterface_WriteSystemThread_Pretty("*",substring);

			substring = NULL;		
			break;

		case RPL_LISTEND: //323
			g_print("\t message: %s \n",message);
			/*Coger el primer token*/
			token = strtok(message,s);
			/*Ir por el resto*/
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread_Pretty("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);			
			break;

		case RPL_MOTDSTART: //375
			g_print("\t message: %s \n",message);
			/*Coger el primer token*/
			token = strtok(message,s);
			/*Ir por el resto*/
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread_Pretty("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);			
			break;

		case RPL_MOTD: //372
			g_print("\t message: %s \n",message);
			/*Coger el primer token*/
			token = strtok(message,s);
			/*Ir por el resto*/
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread_Pretty("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);		
			break;

		case RPL_ENDOFMOTD: //376
			g_print("\t message: %s \n",message);
			/*Coger el primer token*/
			token = strtok(message,s);
			/*Ir por el resto*/
			if(token != NULL){
				token = strtok(NULL,s);	
			}
			IRCInterface_WriteSystemThread_Pretty("*",token);

			token = NULL;
			//IRCInterface_WriteSystem("*",message);	
			return 19;//cambiar por un define
			break;

		case RPL_TOPIC: //332
			g_print(GRN "\n>> [server command] RPL_TOPIC - message = %s\n" RESET, message);
			//g_print("\n=======CASE RPL_TOPIC=======\n");
			//IRCParse_RplTopic (char *strin, char **prefix, char **nick, char **nick2, char **channel, char **msg)
			ret = IRCParse_RplTopic(message, &prefix, &nick, &channel, &topic);
			if(ret != IRC_OK){
				g_print(RED "ERROR - In command_query: case RPL_TOPIC - IRCParse_RplTopic devolvio != IRC_OK\n" RESET);
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

			IRCInterface_WriteChannelThread_Pretty(channel,"*",mensaje);	
			break;

		case RPL_AWAY: //306
			g_print(GRN "\n>> [server command] RPL_AWAY - message = %s\n" RESET, message);
			//>> :irc.eps.net 306 gomupo :You have been marked as being away
			IRCParse_RplAway (message, &prefix, &nick, &nick2, &msg);
			IRCInterface_WriteSystemThread_Pretty("*",msg);
			break;

		case TOPIC: //332
			g_print(GRN "\n>> [server command] TOPIC - message = %s\n" RESET, message);
			//   long IRCParse_Topic (char *strin, char **prefix, char **channel, char **topic)
			ret = IRCParse_Topic (message, &prefix, &channel, &topic);
			if(ret != IRC_OK){
				g_print(RED "ERROR - In command_query: case TOPIC - IRCParse_Topic devolvio != IRC_OK\n" RESET);
				return ERR;
			}

			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t channel: %s \n",channel);
			g_print("\t topic: %s \n\n",topic);
			sprintf(mensaje,"El topic para %s es %s \n",channel,topic);
			g_print("Mensaje: %s \n",mensaje);
			g_print("Existe canal: %d \n", IRCInterface_QueryChannelExistThread(channel));

			IRCInterface_WriteChannelThread_Pretty(channel,"*",mensaje);
			break;

		case RPL_NOTOPIC:
			break;

		case RPL_TOPICWHOTIME: //333
			break;	

		case RPL_NAMREPLY: //353 - reply del servidor de de punames()
			g_print(GRN "\n>> [server command] RPL_NAMREPLY - message = %s\n" RESET, message);
			//long IRCParse_RplNamReply (char *strin, char **prefix, char **nick, char **type, char **channel, char **msg)
			ret = IRCParse_RplNamReply(message, &prefix, &nick, &type, &channel, &msg);
			if(ret != IRC_OK){
				g_print(RED "ERROR - In command_query: case RPL_NAMREPLY - IRCParse_RplNamReply devolvio != IRC_OK\n" RESET);
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
				g_print(RED "ERROR - In command_query: case RPL_ENDOFNAMES - IRCParse_RplEndOfNames devolvio != IRC_OK\n" RESET);
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
			g_print(GRN "\n>> [server command] JOIN - message = %s\n" RESET, message);
			ret = IRCParse_Join (message, &prefix, &channel, &key, &msg);
			if(ret != IRC_OK){
				g_print(RED "\nERROR - In command_query: JOIN - IRCParse_Join devolvio error\n" RESET);
				return ERR;
			}
			g_print("Comandos recibidos en el IRCParse_Join: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t channel: %s \n",channel);
			g_print("\t key: %s \n",key);
			g_print("\t msg: %s \n\n",msg);

			IRCInterface_AddNewChannelThread(msg, 0);
			IRCParse_ComplexUser(prefix, &nick_part, &username_part, &host_part, &server_part);
			sprintf(mensaje, "%s (%s) se ha unido al canal", nick_part, prefix);
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
			g_print(GRN "\n>> [server command] NAMES - message = %s\n" RESET, message);
			g_print(GRN "\nNo hay nada aquí, revisar (?)" RESET);
			break;

		case PRIVMSG:
			g_print(GRN "\n>> [server command] PRIVMSG - message = %s\n" RESET, message);
			ret = IRCParse_Privmsg(message, &prefix, &msgtarget, &msg);
			if(ret != IRC_OK){
				g_print(RED "\nERROR - In command_query: IRCParse_Privmsg devolvio != IRC_OK" RESET);
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

				if(IRCInterface_ReceiveDialogThread(nick_user, filename) == TRUE){
					g_print("Lanzamos el hilo que guarda el archivo\n");
					//pthread_t tid;

					File_args args;
					args.hostname = hostname_destino;
					args.filename = filename;
					args.port = port;
					args.length = length;

					if(pthread_create( &sendf_tid, NULL, (void*) save_file, (void*) &args) < 0){
				    	g_print("Error en la llamada a save_file\n");
						return ERR;
					}
					break;
				}
				/*
				if (sscanf(msg, "\001AUDIOCHAT %ms %li", &hostname_destino, &port) > 0) {
					g_print("host=%s, port=%li\n", hostname_destino, port);
				} else{
					g_print("Formato incorrecto en envio de fichero\n");
				}*/
				ret = enviarDatos(sockfd_user, command, strlen(command));
				if(ret < 0){
					g_print(RED "ERROR - In command_query: case PRIVMSG - enviarDatos() devolvio error (ver secuencia en .log)\n\t\tEl cliente se cerrará.\n" RESET);
					exit(1);
				}
				if(ret == 0){ //timeout 
					g_print(RED "ERROR - In command_query: case PRIVMSG - enviarDatos() mandó 0 Bytes(ver secuencia en .log)\n\t\t(Timeout de conexión probablemente)\n" RESET);
					exit(1);
				}
			}

			if(msgtarget[0] != '#'){//no grupo
				IRCInterface_AddNewChannelThread(nick_privmsg, 0);				
				IRCInterface_WriteChannelThread(nick_privmsg, nick_privmsg, msg);
				return OK;
			}

			//IRCInterface_WriteChannelThread(msgtarget, nick_privmsg, msg);
			IRCInterface_WriteChannelThread_Pretty(msgtarget, nick_privmsg, msg);
			break;

		case PART:
			g_print(GRN "\n>> [server command] PART - message = %s\n" RESET, message);
			ret = IRCParse_Part (message, &prefix, &channel, &msg);
			if(ret != IRC_OK){
				g_print(RED "\nERROR - In command_query: case PART - IRCParse_Part devolvio != IRC_OK" RESET);
				return ERR;
			}
			g_print("Comandos recibidos en el IRCParse_PART: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t channel: %s \n",channel);
			g_print("\t msg: %s \n\n",msg);

			IRCParse_ComplexUser(prefix, &nick_part, &username_part, &host_part, &server_part);
			IRCInterface_DeleteNickChannelThread(channel, nick_part);
			sprintf(mensaje, "El usuario %s ha salido del grupo (%s)",nick_part, msg);
			IRCInterface_WriteChannelThread_Pretty(channel,"*",mensaje);
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
			//g_print("\n=======CASE NOTICE=======\n");
			g_print(GRN "\n>> [server command] NOTICE - message = %s\n" RESET, message);

			IRCParse_Notice(message, &prefix, &msgtarget, &msg);

			g_print("prefix = %s\n", prefix);
			g_print("msgtarget = %s\n", msgtarget);
			g_print("msg = %s\n", msg);

			//comentado de momento, se asume que es 'NOTICE AUTH'
			/*
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
			*/
			//IRCInterface_WriteSystemThread("*",msg);
			break;

		case PING:
			g_print(GRN "\n>> [server command] PING - message = %s\n" RESET, message);
			//:irc.eps.net PONG irc.eps.net :LAG1460877705692323
			IRCParse_Ping (message, &prefix, &server, &server2, &msg);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t server: %s \n",server);
			g_print("\t server2: %s \n",server2);
			g_print("\t msg: %s \n\n",msg);
			IRCMsg_Pong(&command_pong, prefix?prefix:hostname, server?server:"", server2?server2:"", msg?msg:"");
			enviarDatos(sockfd_user, command_pong, strlen(command_pong));
			g_print(GRN "Pong enviado\n" RESET);
			IRCInterface_PlaneRegisterOutMessageThread(command_pong);
			break;

		case KICK:
			g_print(GRN "\n>> [server command] KICK - message = %s\n" RESET, message);
			//long IRCParse_Kick (char *strin, char **prefix, char **channel, char **user, char **comment)

			ret = IRCParse_Kick(message, &prefix, &channel, &user, &comment);
			if(ret != IRC_OK){
				g_print(RED "\nERROR - In command_query: case KICK - IRCParse_Kick devolvio != IRC_OK" RESET);
				return ERR;
			}
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t channel: %s \n",channel);
			g_print("\t user: %s \n",user);
			g_print("\t comment: %s \n\n",comment);

			sprintf(mensaje,"%s Ha sido echado de %s con mensaje/motivo(%s)",user, channel, comment);
			
			g_print("Mensaje: %s \n",mensaje);
			IRCInterface_WriteChannelThread_Pretty(channel,"*",mensaje);

			//Actualizar al lista de usuarios
			IRCInterface_DeleteNickChannelThread (channel, user);
			if(!strcmp(nick_user,user)){
				IRCInterface_RemoveChannelThread(channel);
			}
			/*memset(mensaje,0,MAXDATA);
			sprintf(mensaje,"/names %s",channel);
			retorno = punames(mensaje);
			if(retorno == ERR){
				g_print("ERROR - MODE - punames");
				return ERR;				
			} */
			break;

		case ERR_UNKNOWNMODE: //472
			g_print(GRN "\n>> [server command] ERR_UNKNOWNMODE - message = %s\n" RESET, message);
			//      long IRCParse_ErrUnknownMode (char *strin, char **prefix, char **nick, char **modechar, char **channel, char **msg)
			ret = IRCParse_ErrUnknownMode(message, &prefix, &nick, &modechar, &channel, &msg);
			if(ret != IRC_OK){
				g_print(RED "\nERROR - In command_query: case ERR_UNKNOWNMODE - IRCParse_ErrUnknownMode != IRC_OK" RESET);
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
			IRCInterface_WriteChannelThread_Pretty(channel,"*",mensaje);
			break;

		case MODE: //
			g_print(GRN "\n>> [server command] MODE - message = %s\n" RESET, message);
			//    long IRCParse_Mode (char *strin, char **prefix, char **channeluser, char **mode, char **user)
			ret = IRCParse_Mode(message, &prefix, &channeluser, &mode, &user);
			if(ret != IRC_OK){
				g_print(RED "\nERROR - In command_query: case MODE - IRCParse_Mode != IRC_OK" RESET);
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
					IRCInterface_WriteChannelThread_Pretty(channeluser,"*",mensaje);
					break;		
				}else if(strcasecmp(mode,"+l") == 0){
					sprintf(mensaje,"%s establece límite del canal %s a %s usuarios",nick, channeluser, user);
					g_print("Mensaje: %s \n",mensaje);
					IRCInterface_WriteChannelThread_Pretty(channeluser,"*",mensaje);
					break;						
				}

				sprintf(mensaje,"%s establece el modo %s al usuario %s en el canal %s ",nick, mode, user, channeluser);
				g_print("Mensaje: %s \n",mensaje);
				IRCInterface_WriteChannelThread_Pretty(channeluser,"*",mensaje);

				/* No funciona IRCInterface_ChangeNickStateChannel :(
				if(strcasecmp(mode,"+o") == 0){
					g_print("modo op\n");
					IRCInterface_ChangeNickStateChannel (channeluser, nick, OPERATOR);
				}else if(strcasecmp(mode,"+v") == 0){
					g_print("modo voice\n");
					IRCInterface_ChangeNickStateChannel (channeluser, nick, VOICE);
				}else{
					g_print("else otro modo\n");*/
					//Actualizar al lista de usuarios
					
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
				IRCInterface_WriteChannelThread_Pretty(channeluser,"*",mensaje);
			}			
			break;

		case INVITE:			
			g_print(GRN "\n>> [server command] INVITE - message = %s\n" RESET, message);
			IRCInterface_WriteSystemThread_Pretty("*",message);
			break;

		case NICK:
			g_print(GRN "\n>> [server command] NICK - message = %s\n" RESET, message);
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
			IRCInterface_WriteSystemThread_Pretty("*",mensaje);	
			g_print("new nick: %s\n", msg);

			break;

		case QUIT:
			/*
			* Enfoque 1: Cuando alguien sale (QUIT del servidor)
			*	Obtener la lista de canales abiertos -> '/names' en cada canal -> Capturar reply del servidor:
			*	353 -> interface_mostrar_nicks(channel,msg); (se actualizan todas las listas de chats abiertos)
			*
			*/
			g_print(GRN "\n>> [server command] QUIT - message = %s\n" RESET, message);
			
			IRCParse_Quit (message, &prefix, &msg);
			IRCParse_ComplexUser(prefix, &nick_part, &username_part, &host_part, &server_part);
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t msg: %s \n",msg);
			sprintf(mensaje, "%s se ha desconectado (%s)", nick_part, msg);
			IRCInterface_WriteSystemThread("*",mensaje);

			IRCInterface_ListAllChannelsThread(&channelsQuit, &numChannelsQuit);

			for(i=0; i<numChannelsQuit; i++){
				sprintf(mensaje,"/names %s",channelsQuit[i]);
				retorno = punames(mensaje);
				if(retorno == ERR){
					g_print("ERROR - JOIN - punames");
					return ERR;				
				}
			}
			break;

		case RPL_CHANNELMODEIS: //324 	
			g_print(GRN "\n>> [server command] RPL_CHANNELMODEIS - message = %s\n" RESET, message);
			//long IRCParse_RplChannelModeIs(char *strin, char **prefix, char **nick, char **channel, char **modetxt)
			/*ret = IRCParse_RplChannelModeIs(message, &prefix, &channeluser, &mode, &user);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCParse_RplTopic\n");
				return ERR;
			}
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t channeluser: %s \n",channeluser);
			g_print("\t mode: %s \n",mode);
			g_print("\t user: %s \n\n",user);

			sprintf(mensaje,"%s establece modo %s %s ", user, channeluser, mode);
			
			g_print("Mensaje: %s \n",mensaje);
			IRCInterface_WriteChannelThread(channeluser,"*",mensaje);*/

			//Actualizar al lista de usuarios
			break;	

		case ERR_CHANOPRIVSNEEDED: //482
			g_print(GRN "\n>> [server command] ERR_CHANOPRIVSNEEDED - message = %s\n" RESET, message);
			//   long IRCParse_ErrChanOPrivsNeeded (char *strin, char **prefix, char **nick, char **channel, char **msg)
			ret = IRCParse_ErrChanOPrivsNeeded(message, &prefix, &nick, &channel, &msg);
			if(ret != IRC_OK){
				g_print(RED "\nERROR - In command_query: case ERR_CHANOPRIVSNEEDED - IRCParse_ErrChanOPrivsNeeded != IRC_OK" RESET);
				//return ERR;
			}
			g_print("Comandos recibidos en el IRCParse_ErrChanOPrivsNeeded: \n");
			g_print("\t message: %s \n",message);
			g_print("\t prefix: %s \n",prefix);
			g_print("\t nick: %s \n",nick);			
			g_print("\t channel: %s \n",channel);
			g_print("\t msg: %s \n\n",msg);		

			sprintf(mensaje,"%s %s\n",channel,msg);
			IRCInterface_WriteChannelThread_Pretty(channel,"*",mensaje);
			break;

		/*TRATAMIENTO DE ERRORES*/
		case IRCERR_NOCOMMAND:
			g_print(RED "\nERROR - In command_query: Mensaje de error recibido en switch():\n\tIRCERR_NOCOMMAND -  no hay ningún comando en la cadena de caracteres\n" RESET);
			IRCInterface_WriteSystemThread("*",message);
			return ERR;

		case IRCERR_NOPARAMS:
			g_print(RED "\nERROR - In command_query: Mensaje de error recibido en switch():\n\tIRCERR_NOPARAMS -  se ha introducido una cadena de caracteres nula\n" RESET);
			IRCInterface_WriteSystemThread("*",message);
			return ERR;

		case IRCERR_UNKNOWNCOMMAND:
			//g_print(MAG"\n>>>>>>>>>>>>>>>>>>>>>>>>>>\n"RESET);
			//ret =  IRCParse_GeneralCommand (message, &prefix, &type, &params, &n_params, &msg);
			/*Intenta hacer un parseo apra un type no soportado por IRC_CommandQuery()*/
			unknw_type = parse_type(message);
			if(unknw_type != ERR){
				switch(unknw_type){ //Intentar tratar un mensaje no reconocido por IRC_CommandQuery() 
					case 250:
						g_print(GRN "\n>> [server command] unknw_type = 250 - message = %s\n" RESET, message);
						substring = strnext(message, ':');
						if (substring){
							substring = strnext(substring, ':');
						}
						IRCInterface_WriteSystemThread_Pretty("*",substring);

						substring = NULL;
					break;

					default:
					g_print(YEL "WARN - In command_query: case IRCERR_UNKNOWNCOMMAND:\n\tNo se ha definido un tratamiento para un mensaje desconocido con type = %d\n" RESET, unknw_type);
					IRCInterface_WriteSystemThread("*",message);
					break;
				}
				return OK;
			}

			g_print(YEL "WARN - In command_query: Mensaje de error recibido en switch():\n\tIRCERR_UNKNOWNCOMMAND -  l comando que contiene no es reconocido por esta función.\n" RESET);
			IRCInterface_WriteSystemThread("*",message);
			return OK;

		default:
			g_print(YEL "WARN - In command_query: Alcanzado default del switch()(TO_DO):\n\t %s\n" RESET, message);
			//sprintf(aux,"[%s] %s",gmtime(time(NULL)), nick)
			IRCInterface_WriteSystemThread("*",message);
			return OK;
	}

	//Liberamos memoria
	//mfree(33,msg,nick,nick2,servername,versionname,timedate,version,availableusermodes,availablechannelmodes,addedg,type,visible,topic,channel,key,
	//msgtarget,origin_nick,filename,hostname_destino,nick_part,username_part,host_part,server_part,server,server2,command_pong,token,modechar,channeluser,
	//mode,user,comment,realname,host);
	return OK;

}

void glueAndQuery(char* command, char* last_command){
	char* glued_command = (char*) malloc((2 + strlen(command) + strlen(last_command)) * sizeof(char));
	
	strcpy(glued_command, last_command);
	strcat(glued_command, command);

	g_print(BLU "\nglued_command = %s\n" RESET, glued_command);
	command_query(glued_command);

	free(command);
	free(glued_command);
}

/**
* @brief Funcion para dividir en comandos la cadena "message"
* @param message cadena recibida, puede incluir mas de un comando
* @param MAXDATA_flag flag que marca que se ha leído el maximo del buffer y han podido cortarse comandos
* @return void
*/
void unpipe(char* message, int MAXDATA_flag){

	char *q = message, *command;
	char *glued_command = NULL;
	int i = 0, test = 0;

	while (q != NULL){
		q = IRC_UnPipelineCommands(q, &command);
		test = testIRC_CommandQuery(command);

		if(MAXDATA_flag == 1){	//caso 1.)riesgo: se ha llamado a unpipe() con un bloque de tam MAXDATA
			if((i == 0) && (check_next_unpipe == 1)){	//caso 2.1)primera iter unpipe, no MAXDATA PERO 'check...' activado
				if(test == OK && last_test == ERR){			//caso 2.1.1)además no da error
					glueAndQuery(command, last_command);
				}else if(test == ERR && last_test == OK){	//caso 2.1.2)o bien es la segunda mitad del comando o bien
															//           da la casualidad de que es un comando mal formado independiente
					glueAndQuery(command, last_command);
				}else if(test == ERR && last_test == ERR){	//caso 2.1.3)ambos test petan. Concatenar y rezar
					glued_command =  (char*) malloc((2 + strlen(command) + strlen(last_command)) * sizeof(char));
					strcpy(glued_command, last_command);
					strcat(glued_command, command);

					g_print(BLU "\nglued_command = %s\n" RESET, glued_command);
					if (testIRC_CommandQuery(glued_command) == OK){
						command_query(glued_command);
						free(glued_command);
					}else{
						command_query(last_command);
						command_query(command);
					}

					free(command);
				}else{										//caso 2.1.4)ambos comandos están bien formados
					command_query(command);
					free(command);
				}
				//resetear flags:
				check_next_unpipe = 0;
				strcpy(last_command, "0");

			}else if(q == NULL){ //q es 'resto', comprobar que sea el último comando parseado del bloque
				if (test == ERR){							//caso 1.1)error en test con MAXDATA_f
					g_print(YEL "\nWARN - In unpipe: testIRC_CommandQuery(command) devolvio ERR con MAXDATA_flag activada\n\tSe almacena el trozo de comando=%s\n" RESET, command);
					last_test = ERR;
					strcpy(last_command, command);
					free(command);
					check_next_unpipe = 1;
					break; //(!) en principio no tiene sentido seguir iterando, es el último mensaje (trozo) del bloque
				}else{										//caso 1.2)no error en test con MAXDATA_f, 2 posibilidades:
															//a)justo ha cogido el comando entero, b)ha cogido parte del comando y lo reconoce como valido
					g_print(YEL "\nWARN - In unpipe: testIRC_CommandQuery(command) devolvio OK con MAXDATA_flag activada\n\tSe almacena el trozo de comando=%s\n" RESET, command);
					last_test = OK;
					strcpy(last_command, command);
					free(command);
				}
				check_next_unpipe = 1;
			}else{		//si no es el último comando parseado, command_query normal
				if(command_query(command) == ERR){
					g_print(RED "ERROR - In unpipe: command_query() devolvio ERR (MAXDATA_flag == 1 pero no es el último comando)\n\tcommand = %s" RESET, command);
					free(command);
				}
			}

		}else{					//caso 2)no se ha llamado a unpipe() con un bloque de tam MAXDATA
			if((i == 0) && (check_next_unpipe == 1)){	//caso 2.1)primera iter unpipe, no MAXDATA PERO 'check...' activado
				if(test == OK && last_test == ERR){			//caso 2.1.1)además no da error
					glueAndQuery(command, last_command);
				}else if(test == ERR && last_test == OK){	//caso 2.1.2)o bien es la segunda mitad del comando o bien
															//           da la casualidad de que es un comando mal formado independiente
					glueAndQuery(command, last_command);
				}else if(test == ERR && last_test == ERR){	//caso 2.1.3)ambos test petan. No se puede concatenar
					glued_command =  (char*) malloc((2 + strlen(command) + strlen(last_command)) * sizeof(char));
					strcpy(glued_command, last_command);
					strcat(glued_command, command);
					g_print(BLU "\nglued_command = %s\n" RESET, glued_command);

					if (testIRC_CommandQuery(glued_command) == OK){
						command_query(glued_command);
						free(glued_command);
					}else{
						command_query(last_command);
						command_query(command);
					}

					free(command);
				}else{										//caso 2.1.4)ambos comandos están bien formados
					command_query(command);
					free(command);
				}
				//resetear flags:
				check_next_unpipe = 0;
				strcpy(last_command, "0");
			}else{										//caso 2.2)no MAXDATA, no i=0: command_query normal
				if(command_query(command) == ERR){
					g_print(RED "ERROR - In unpipe: command_query() devolvio ERR (i != 0, check_... False, MAXDATA_flag False)\n\tcommand = %s" RESET, command);
					free(command);
				}
			}
		}
		i++;
	}

    if(command == NULL){
        g_print(RED "ERROR - In unpipe: Se ha solicitado buscar un comando en una cadena vacia\n\n" RESET);
    }   
}

/**
* @brief Funcion ejecutada por un hilo, que recibe mensajes del servidor y los procesa segun el tipo de respuesta
* @param no_arg estructura de parametros (vacia)
* @return OK si todo es correcto, ERR si se produce un error
*/
void receive_messages(void* no_arg){

	char message[MAXDATA];
	int ret;
	g_print(GRN "Hilo Preparado para recibir mensajes\n" RESET);

	int oldtype;
	/*No es necesario el pthread_cancel asíncrono aparentemente
	* +INFO: https://www.securecoding.cert.org/confluence/display/c/POS44-C.+Do+not+use+signals+to+terminate+threads
	*		 https://www.securecoding.cert.org/confluence/display/c/POS47-C.+Do+not+use+threads+that+can+be+canceled+asynchronously
	*/
	//pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
	
	while(1){
		//printf(BLU "\nwhile en receive_messages\n" RESET);
		//sem_wait(&recepcionTCP);
		memset(message, 0, sizeof(message));
		ret = recvDatos(sockfd_user, message, MAXDATA, host_name);
		//sem_post(&recepcionTCP);
		if(ret == ERR){
			g_print(RED "ERROR - In receive_messages: recvDatos() devolvio error (ver .log)\n\t\tEl cliente se cerrará.\n" RESET);
			exit(1);
		}
		if(ret == 0){ //timeout seguramente
			g_print(RED "ERROR - In receive_messages: recvDatos() leyó 0 Bytes(ver .log)\n\t\t(Timeout)El cliente se cerrará.\n" RESET);
			exit(1);
		}
		if(ret == MAXDATA)
			unpipe(message, 1);
		else
			unpipe(message, 0);
	}
}

/** 
 * @defgroup IRCInterface Interface
 *
 */

/** 
 * @defgroup IRCInterfaceCallbacks Callbaks
 * @ingroup IRCInterface
 *
 */

/** 
 * @addtogroup IRCInterfaceCallbacks
 * Funciones que van a ser llamadas desde el interface y que deben ser implementadas por el usuario.
 * Todas estas funciones pertenecen al hilo del interfaz.
 *
 * El programador puede, por supuesto, separar todas estas funciones en múltiples ficheros a
 * efectos de desarrollo y modularización.
 *
 * <hr>
 */

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateChannelKey IRCInterface_ActivateChannelKey
 *
 * @brief Llamada por el botón de activación de la clave del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateChannelKey (char *channel, char * key)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de activación de la clave del canal. El segundo parámetro es
 * la clave del canal que se desea poner. Si es NULL deberá impedirse la activación
 * con la función implementada a tal efecto. En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se va a activar la clave.
 * @param[in] key clave para el canal indicado.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_ActivateChannelKey(char *channel, char *key)
{
	char modo[MAXDATA] = {0};

	g_print(BLU "\nIRCInterface_ActivateChannelKey(char *channel, char *key) call\n" RESET);

	sprintf(modo,"+k %s",key);
	if (changeMode(channel, NULL, modo) != OK)
		g_print(RED "ERROR - In IRCInterface_ActivateChannelKey: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateExternalMessages IRCInterface_ActivateExternalMessages
 *
 * @brief Llamada por el botón de activación de la recepción de mensajes externos.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateExternalMessages (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de activación de la recepción de mensajes externos.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se activará la recepción de mensajes externos.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_ActivateExternalMessages(char *channel)
{
	g_print(BLU "\nIRCInterface_ActivateExternalMessages(char *channel) call\n" RESET);
	if (changeMode(channel, NULL, "+n") != OK)
		g_print(RED "ERROR - In IRCInterface_ActivateExternalMessages: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateInvite IRCInterface_ActivateInvite
 *
 * @brief Llamada por el botón de activación de canal de sólo invitación.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateInvite (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de activación de canal de sólo invitación.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se activará la invitación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_ActivateInvite(char *channel)
{
	g_print(BLU "\nIRCInterface_ActivateInvite(char *channel) call\n" RESET);
	if (changeMode(channel, NULL, "+i") != OK)
		g_print(RED "ERROR - In IRCInterface_ActivateInvite: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateModerated IRCInterface_ActivateModerated
 *
 * @brief Llamada por el botón de activación de la moderación del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateModerated (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de activación de la moderación del canal.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se activará la moderación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_ActivateModerated(char *channel)
{
	g_print(BLU "\nIRCInterface_ActivateModerated(char *channel) call\n" RESET);

	if (changeMode(channel, NULL, "+m") != OK)
		g_print(RED "ERROR - In IRCInterface_ActivateModerated: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateNicksLimit IRCInterface_ActivateNicksLimit
 *
 * @brief Llamada por el botón de activación del límite de usuarios en el canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateNicksLimit (char *channel, int * limit)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de activación del límite de usuarios en el canal. El segundo es el
 * límite de usuarios que se desea poner. Si el valor es 0 se sobrentiende que se desea eliminar
 * este límite.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se activará el límite de usuarios.
 * @param[in] limit límite de usuarios en el canal indicado.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_ActivateNicksLimit(char *channel, int limit)
{
	char mode[MAXDATA] = {0};

	g_print(BLU "\nIRCInterface_ActivateNicksLimit(char *channel, int limit) call\n" RESET);

	sprintf(mode,"+l %d",limit);
	if (changeMode(channel, NULL, mode) != OK)
		g_print(RED "ERROR - In IRCInterface_ActivateNicksLimit: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivatePrivate IRCInterface_ActivatePrivate
 *
 * @brief Llamada por el botón de activación del modo privado.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivatePrivate (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de activación del modo privado.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a activar la privacidad.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_ActivatePrivate(char *channel)
{
	g_print(BLU "\nIRCInterface_ActivatePrivate(char *channel) call\n" RESET);
	if (changeMode(channel, NULL, "+p") != OK)
		g_print(RED "ERROR - In IRCInterface_ActivatePrivate: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateProtectTopic IRCInterface_ActivateProtectTopic
 *
 * @brief Llamada por el botón de activación de la protección de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateProtectTopic (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de activación de la protección de tópico.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a activar la protección de tópico.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateProtectTopic(char *channel)
{
	g_print(BLU "\nIRCInterface_ActivateProtectTopic(char *channel) call\n" RESET);
	if (changeMode(channel, NULL, "+t") != OK)
		g_print(RED "ERROR - In IRCInterface_ActivateProtectTopic: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateSecret IRCInterface_ActivateSecret
 *
 * @brief Llamada por el botón de activación de canal secreto.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateSecret (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de activación de canal secreto.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a activar el estado de secreto.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_ActivateSecret(char *channel)
{
	g_print(BLU "\nIRCInterface_ActivateSecret(char *channel) call\n" RESET);

	if (changeMode(channel, NULL, "+s") != OK)
		g_print(RED "ERROR - In IRCInterface_ActivateSecret: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_BanNick IRCInterface_BanNick
 *
 * @brief Llamada por el botón "Banear".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_BanNick (char *channel, char *nick)
 * @endcode
 * 
 * @description 
 * Llamada por el botón "Banear". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se va a realizar el baneo. En principio es un valor innecesario.
 * @param[in] nick nick del usuario que va a ser baneado
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_BanNick(char *channel, char *nick)
{
	g_print(BLU "\nIRCInterface_BanNick(char *channel, char *nick) call\n" RESET);

	if (changeMode(channel, nick, "+b") != OK)
		g_print(RED "ERROR - In IRCInterface_BanNick: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_Connect IRCInterface_Connect
 *
 * @brief Llamada por los distintos botones de conexión.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	long IRCInterface_Connect (char *nick, char * user, char * realname, char * password, char * server, int port, boolean ssl)
 * @endcode
 * 
 * @description 
 * Función a implementar por el programador.
 * Llamada por los distintos botones de conexión. Si implementará la comunicación completa, incluido
 * el registro del usuario en el servidor.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leída.
 *
 *
 * @param[in] nick nick con el que se va a realizar la conexíón.
 * @param[in] user usuario con el que se va a realizar la conexión.
 * @param[in] realname nombre real con el que se va a realizar la conexión.
 * @param[in] password password del usuario si es necesaria, puede valer NULL.
 * @param[in] server nombre o ip del servidor con el que se va a realizar la conexión.
 * @param[in] port puerto del servidor con el que se va a realizar la conexión.
 * @param[in] ssl puede ser TRUE si la conexión tiene que ser segura y FALSE si no es así.
 *
 * @retval IRC_OK si todo ha sido correcto (debe devolverlo).
 * @retval IRCERR_NOSSL si el valor de SSL es TRUE y no se puede activar la conexión SSL pero sí una 
 * conexión no protegida (debe devolverlo).
 * @retval IRCERR_NOCONNECT en caso de que no se pueda realizar la comunicación (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
long IRCInterface_Connect(char *nick, char *user, char *realname, char *password, char *server, int port, boolean ssl)
{
	int optval = 1;
	long ret = -1;
	int retorno = -1;
	char *command = NULL;
	char *prefix = NULL;
	int sockfd = -1;

	struct hostent *he;
    struct in_addr **addr_list;
    int i=0;
    char ip_addr[INET_ADDRSTRLEN] = {0};

    char *msgNick = NULL;
    char mode[MAXDATA] = {0};

	g_print("IRCInterface_Connect - Datos introducidos por el usuario \n");
	g_print("\t nick: %s \n",nick);
	g_print("\t user: %s \n",user);
	g_print("\t realname: %s \n",realname);
	g_print("\t password: %s \n",password);
	g_print("\t server: %s \n",server);
	g_print("\t port: %d \n",port);
	g_print("\t ssl: %d \n",ssl);

    if ((he = gethostbyname(server)) == NULL) {  // get the host info
        g_print(RED "ERROR - In IRCInterface_Connect: gethostbyname() devol NULL\n" RESET);
        return IRCERR_NOCONNECT;
    }

    // print information about this host:
    g_print("Official name is: %s\n", he->h_name);
    g_print("    IP addresses: ");

    addr_list = (struct in_addr **)he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) {
    	strcat(ip_addr,inet_ntoa(*addr_list[i]));
        g_print("%s ", inet_ntoa(*addr_list[i]));
    }
    g_print("\n");
    g_print("ip_addr: %s \n",ip_addr);

    struct sockaddr_in server_struct;

    /*Socket*/
    sockfd = socket(AF_INET,SOCK_STREAM,0/*TCP*/);
    if (sockfd == -1){
        g_print(RED "ERROR - In IRCInterface_Connect: Error creando socket, devolvió < 0\n" RESET);
        return IRCERR_NOCONNECT;
    }

    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int)); //reutilizar socket sucesivas ejecuciones

    memset(&server_struct, '0', sizeof(server_struct)); 
    server_struct.sin_family = AF_INET;
    server_struct.sin_port = htons(port); 
    memset(&(server_struct.sin_zero), '\0', 8);
    server_struct.sin_addr = **addr_list; 

    /*Connect*/
    if (connect(sockfd, (struct sockaddr*)&server_struct, sizeof(server_struct)) < 0){
        g_print(RED "ERROR - In IRCInterface_Connect: Error en connect(), devolivó < 0\n" RESET);
        return IRCERR_NOCONNECT;
    }

	//Conectarse al socket
	//sockfd = clienteConexionTCP(ip_addr,port);
	if(sockfd == -1){
		g_print(RED "ERROR - In IRCInterface_Connect: sockfd == -1\n" RESET);
		return IRCERR_NOCONNECT;
	}else{
		g_print(GRN "Conexión establecida con host: %s y puerto: %d \n" RESET, server, port);
	}

	//Copiar los parametros a las variables globales
	strcpy(host_name, he->h_name);
    strcpy(nick_user,nick);
    sockfd_user = sockfd;

	if(ssl == FALSE){
		g_print(GRN "Lanzamos el hilo que recibe mensajes\n" RESET);
		if((ret = pthread_create( &recv_tid, NULL, (void*) receive_messages, (void*) "no_arg")) != 0){
	    	g_print(RED "ERROR - In IRCInterface_Connect: pthread_create() devolvio != 0, error = %d\n" RESET, (int)ret);
	    	//logERR("pthread_detach() devolvio error");
			return ERR;
		}

		if (pthread_detach(recv_tid) != OK){
			g_print(RED "ERROR - In IRCInterface_Connect: pthread_detach() devolvio error\n" RESET);
			 //logERR("pthread_detach() devolvio error");
			 return ERR;
		}
		//sleep(5);
		//Prueba CAP
		/*
		retorno = enviarDatos(sockfd, "CAP LS 302", strlen("CAP LS 302")+1);
		if(retorno <= 0){
			g_print(RED "ERROR - In IRCInterface_Connect: enviarDatos() envió %d Bytes - NICK\n" RESET, retorno);
			return IRCERR_NOCONNECT;
		}
		IRCInterface_PlaneRegisterOutMessage(command);
		*/

		//Enviar pass si existe
		if((password != NULL) && (strlen(password) > 0)){ //Comprobar si hay password
			//Enviar mensaje al servidor con la pass
			ret = IRCMsg_Pass (&command, prefix, password);
			if(ret != IRC_OK){
				g_print(RED "ERROR - In IRCInterface_Connect: IRCMsg_Pass devolvio IRCERR_NOPASSWORD: No se ha introducido una clave.\n" RESET);
				return IRCERR_NOCONNECT;
			}
			g_print("IRCMsg_Pass: %s",command);
			retorno = enviarDatos(sockfd, command, strlen(command));
			if(retorno <= 0){
				g_print(RED "ERROR - In IRCInterface_Connect: enviarDatos() envió %d Bytes - PASS\n" RESET, retorno);
				return IRCERR_NOCONNECT;
			}
			IRCInterface_PlaneRegisterOutMessage(command); //Mandar a registro plano
			free(command);		

		}

		//Enviar mensaje con el nick
		command = NULL;
		ret = IRCMsg_Nick (&command, prefix, nick, msgNick);
		
		if(ret != IRC_OK){
			g_print(RED "ERROR - In IRCInterface_Connect: IRCMsg_Nick() devolvio codigo de error\n" RESET);
			return IRCERR_NOCONNECT;
		}
		g_print("IRCMsg_Nick: %s",command);

		retorno = enviarDatos(sockfd, command, strlen(command));
		//retorno = enviarDatos(sockfd, "NICK test_\r\n\0", strlen("NICK test_\r\n\0")+1);
		if(retorno <= 0){
			g_print(RED "ERROR - In IRCInterface_Connect: enviarDatos() envió %d Bytes - NICK\n" RESET, retorno);
			return IRCERR_NOCONNECT;
		}
		IRCInterface_PlaneRegisterOutMessage(command);
		free(command);
		
		//mode
		//Enviar mensaje con el user
		strcpy(mode,"0");
		command = NULL;

		ret = IRCMsg_User (&command, prefix, user, mode , realname);
		if(ret != IRC_OK){
			g_print(RED "ERROR - In IRCInterface_Connect: IRCMsg_User() devolvio codigo de error\n" RESET);
			return IRCERR_NOCONNECT;
		}
		g_print("IRCMsg_User: %s",command);

		retorno = enviarDatos(sockfd, command, strlen(command));
		//retorno = enviarDatos(sockfd, "USER test_ 0 * :test_\r\n\0", strlen("USER test_ 0 * :test_\r\n\0")+1);
		if(retorno <= 0){
			g_print(RED "ERROR - In IRCInterface_Connect: enviarDatos() envió %d Bytes - USER\n" RESET, retorno);
			return IRCERR_NOCONNECT;
		}
		IRCInterface_PlaneRegisterOutMessage(command);
		free(command);
		
	}else{ //Fin del if de ssl = false
		return IRCERR_NOSSL;
	}

	return IRC_OK;
}


/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateChannelKey IRCInterface_DeactivateChannelKey
 *
 * @brief Llamada por el botón de desactivación de la clave del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateChannelKey (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de desactivación de la clave del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la clave.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_DeactivateChannelKey(char *channel)
{
	g_print(BLU "\nIRCInterface_DeactivateChannelKey(char *channel) call\n" RESET);

	if (changeMode(channel, NULL, "-k") != OK)
		g_print(RED "ERROR - In IRCInterface_DeactivateChannelKey: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateExternalMessages IRCInterface_DeactivateExternalMessages
 *
 * @brief Llamada por el botón de desactivación de la recepción de mensajes externos.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateExternalMessages (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de desactivación de la recepción de mensajes externos.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a deactivar la recepción de mensajes externos.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_DeactivateExternalMessages(char *channel)
{
	g_print(BLU "\nIRCInterface_DeactivateExternalMessages(char *channel) call\n" RESET);

	if (changeMode(channel, NULL, "-n") != OK)
		g_print(RED "ERROR - In IRCInterface_DeactivateExternalMessages: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateInvite IRCInterface_DeactivateInvite
 *
 * @brief Llamada por el botón de desactivación de canal de sólo invitación.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateInvite (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de desactivación de canal de sólo invitación.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la invitación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_DeactivateInvite(char *channel)
{
	g_print(BLU "\nIRCInterface_DeactivateInvite(char *channel) call\n" RESET);

	if (changeMode(channel, NULL, "-i") != OK)
		g_print(RED "ERROR - In IRCInterface_DeactivateInvite: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateModerated IRCInterface_DeactivateModerated
 *
 * @brief Llamada por el botón de desactivación  de la moderación del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateModerated (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de desactivación  de la moderación del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la moderación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_DeactivateModerated(char *channel)
{
	g_print(BLU "\nIRCInterface_DeactivateModerated(char *channel) call\n" RESET);

	if (changeMode(channel, NULL, "-m") != OK)
		g_print(RED "ERROR - In IRCInterface_DeactivateModerated: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateNicksLimit IRCInterface_DeactivateNicksLimit
 *
 * @brief Llamada por el botón de desactivación de la protección de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateNicksLimit (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de desactivación  del límite de usuarios en el canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar el límite de usuarios.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_DeactivateNicksLimit(char *channel)
{
	g_print(BLU "\nIRCInterface_DeactivateNicksLimit(char *channel) call\n" RESET);

	if (changeMode(channel, NULL, "-l") != OK)
		g_print(RED "ERROR - In IRCInterface_DeactivateNicksLimit: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivatePrivate IRCInterface_DeactivatePrivate
 *
 * @brief Llamada por el botón de desactivación del modo privado.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivatePrivate (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de desactivación del modo privado.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @param[in] channel canal sobre el que se va a desactivar la privacidad.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_DeactivatePrivate(char *channel)
{
	g_print(BLU "\nIRCInterface_DeactivatePrivate(char *channel) call\n" RESET);

	if (changeMode(channel, NULL, "-p") != OK)
		g_print(RED "ERROR - In IRCInterface_DeactivatePrivate: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateProtectTopic IRCInterface_DeactivateProtectTopic
 *
 * @brief Llamada por el botón de desactivación de la protección de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateProtectTopic (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de desactivación de la protección de tópico.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la protección de tópico.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateProtectTopic(char *channel)
{
	g_print(BLU "\nIRCInterface_DeactivateProtectTopic(char *channel) call\n" RESET);

	if (changeMode(channel, NULL, "-t") != OK)
		g_print(RED "ERROR - In IRCInterface_DeactivateProtectTopic: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateSecret IRCInterface_DeactivateSecret
 *
 * @brief Llamada por el botón de desactivación de canal secreto.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateSecret (char *channel)
 * @endcode
 * 
 * @description 
 * Llamada por el botón de desactivación de canal secreto.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la propiedad de canal secreto.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_DeactivateSecret(char *channel)
{
	g_print(BLU "\nIRCInterface_DeactivateSecret(char *channel) call\n" RESET);

	if (changeMode(channel, NULL, "-s") != OK)
		g_print(RED "ERROR - In IRCInterface_DeactivateSecret: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DisconnectServer IRCInterface_DisconnectServer
 *
 * @brief Llamada por los distintos botones de desconexión.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	boolean IRCInterface_DisconnectServer (char * server, int port)
 * @endcode
 * 
 * @description 
 * Llamada por los distintos botones de desconexión. Debe cerrar la conexión con el servidor.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.

 * @param[in] server nombre o ip del servidor del que se va a realizar la desconexión.
 * @param[in] port puerto sobre el que se va a realizar la desconexión.
 *
 * @retval TRUE si se ha cerrado la conexión (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
boolean IRCInterface_DisconnectServer(char *server, int port)
{
	g_print(BLU "\nIRCInterface_DisconnectServer(char *server, int port) call\n" RESET);

	puquit(NULL);

	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ExitAudioChat IRCInterface_ExitAudioChat
 *
 * @brief Llamada por el botón "Cancelar" del diálogo de chat de voz.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ExitAudioChat (char *nick)
 * @endcode
 * 
 * @description 
 * Llamada por el botón "Parar" del diálogo de chat de voz. Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función cierrala comunicación. Evidentemente tiene que
 * actuar sobre el hilo de chat de voz.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] nick nick del usuario que solicita la parada del chat de audio.
 *
 * @retval TRUE si se ha cerrado la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
boolean IRCInterface_ExitAudioChat(char *nick)
{
	g_print(BLU "\nIRCInterface_ExitAudioChat(char *nick) call\n" RESET);
	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_GiveOp IRCInterface_GiveOp
 *
 * @brief Llamada por el botón "Op".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_GiveOp (char *channel, char *nick)
 * @endcode
 * 
 * @description 
 * Llamada por el botón "Op". Previamente debe seleccionarse un nick del
 * canal para darle "op" a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se va dar op al usuario.
 * @param[in] nick nick al que se le va a dar el nivel de op.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_GiveOp(char *channel, char *nick)
{
	g_print(BLU "\nIRCInterface_GiveOp(char *channel, char *nick) call\n" RESET);

	if (changeMode(channel, nick, "+o") != OK)
		g_print(RED "ERROR - In IRCInterface_GiveOp: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_GiveVoice IRCInterface_GiveVoice
 *
 * @brief Llamada por el botón "Dar voz".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_GiveVoice (char *channel, char *nick)
 * @endcode
 * 
 * @description 
 * Llamada por el botón "Dar voz". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se va dar voz al usuario.
 * @param[in] nick nick al que se le va a dar voz.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_GiveVoice(char *channel, char *nick)
{
	g_print(BLU "\nIRCInterface_GiveVoice(char *channel, char *nick) call\n" RESET);

	if (changeMode(channel, nick, "+v") != OK)
		g_print(RED "ERROR - In IRCInterface_GiveOp: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_KickNick IRCInterface_KickNick
 *
 * @brief Llamada por el botón "Echar".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_KickNick (char *channel, char *nick)
 * @endcode
 * 
 * @description 
 * Llamada por el botón "Echar". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se va a expulsar al usuario.
 * @param[in] nick nick del usuario que va a ser expulsado.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_KickNick(char *channel, char *nick)
{
	g_print(BLU "\nIRCInterface_KickNick(char *channel, char *nick) call\n" RESET);

	int ret = -1;
	char *command=NULL;

	//long IRCMsg_Kick (char **command, char *prefix, char * channel, char *user, char *comment)
	ret = IRCMsg_Kick (&command, NULL, channel, nick, "Has sido kickeado del servidor");
	if(ret != IRC_OK){
		g_print(RED "ERROR - In IRCInterface_KickNick : Error en IRCMsg_Kick, no devolvio IRC_OK\n" RESET);
		return;
	}
	ret = enviarDatos(sockfd_user, command, strlen(command));
	if(ret == ERR){
		g_print(RED "ERROR - In IRCInterface_KickNick: enviarDatos devolvio ERR\n" RESET);
		return;
	}
	IRCInterface_PlaneRegisterOutMessage(command);
	free(command);	
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_NewCommandText IRCInterface_NewCommandText
 *
 * @brief Llamada la tecla ENTER en el campo de texto y comandos.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_NewCommandText (char *command)
 * @endcode
 * 
 * @description
 * Llamada de la tecla ENTER en el campo de texto y comandos. El texto deberá ser
 * enviado y el comando procesado por las funciones de "parseo" de comandos de
 * usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] comando introducido por el usuario.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_NewCommandText(char *command)
{
	g_print(BLU "\n[callback] IRCInterface_NewCommandText(char *command) call\n" RESET);

	int num_comando = -1, ret;
	char *comando = NULL;
	char *prefix = NULL;
	//pthread_t tid;
	//IRCInterface_PlaneRegisterInMessage(command);
	//IRCInterface_WriteSystem(nick_user, command);

	if(command[0] != '/'){//mensaje a grupo o nick directamente
		active_channel = IRCInterface_ActiveChannelName();
		g_print("Active_channel: %s\n", active_channel);

		//   long IRCMsg_Privmsg (char **command, char *prefix, char * msgtarget, char *msg)
		ret = IRCMsg_Privmsg (&comando, prefix, active_channel, command);
		if(ret == ERR){
			g_print(RED "ERROR - In IRCInterface_NewCommandText: IRCMsg_Privmsg devolvio error\n" RESET);
			return;
		}
		g_print("\t Mensaje a enviar IRCMsg_Privmsg: %s \n",comando);

		ret = enviarDatos(sockfd_user,comando, strlen(comando));
		if(ret < 0){
			g_print(RED "ERROR - In IRCInterface_NewCommandText: enviarDatos() devolvio error (ver secuencia en .log)\n\t\tEl cliente se cerrará.\n" RESET);
			exit(1);
		}
		if(ret == 0){ //timeout 
			g_print(RED "ERROR - In IRCInterface_NewCommandText: enviarDatos() mandó 0 Bytes(ver secuencia en .log)\n\t\t(Timeout de conexión probablemente)\n" RESET);
			exit(1);
		}
		IRCInterface_PlaneRegisterOutMessage(comando);
		//No recibimos nada en este comando, los mensajes de otros usuarios los recibimos por otro hilo
		IRCInterface_WriteChannel(active_channel, nick_user, command);
		return;
	}

	num_comando = IRCUser_CommandQuery (command);	
	g_print("num_comando: %d \n",num_comando);
	if (p_array_funciones[num_comando](command) == -1){
		g_print(RED "ERROR - In IRCInterface_NewCommandText: Error en p_array_funciones num: %d \n" RESET,num_comando);
	}
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_NewTopicEnter IRCInterface_NewTopicEnter
 *
 * @brief Llamada cuando se pulsa la tecla ENTER en el campo de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_NewTopicEnter (char * topicdata)
 * @endcode
 * 
 * @description 
 * Llamada cuando se pulsa la tecla ENTER en el campo de tópico.
 * Deberá intentarse cambiar el tópico del canal.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * param[in] topicdata string con el tópico que se desea poner en el canal.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_NewTopicEnter(char *topicdata)
{
	g_print(BLU "\nIRCInterface_NewTopicEnter(char *topicdata) call\n" RESET);

	int ret = -1;
	char *command=NULL;

	//long IRCMsg_Topic (char **command, char *prefix, char * channel, char *topic)
	active_channel = IRCInterface_ActiveChannelName();
	ret = IRCMsg_Topic (&command, NULL, active_channel, topicdata);
	if(ret != IRC_OK){
		g_print(RED "ERROR - In IRCInterface_NewTopicEnter: IRCMsg_Topic no devolvio IRC_OK\n" RESET);
		return;
	}
	ret = enviarDatos(sockfd_user, command, strlen(command));
	if(ret == ERR){
		g_print(RED "ERROR - In IRCInterface_NewTopicEnter: enviarDatos devolvio ERR\n" RESET);
		return;
	}
	IRCInterface_PlaneRegisterOutMessage(command);
	free(command);	
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_SendFile IRCInterface_SendFile
 *
 * @brief Llamada por el botón "Enviar Archivo".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_SendFile (char * filename, char *nick, char *data, long unsigned int length)
 * @endcode
 * 
 * @description 
 * Llamada por el botón "Enviar Archivo". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función como todos los demás callbacks bloquea el interface
 * y por tanto es el programador el que debe determinar si crea un nuevo hilo para enviar el archivo o
 * no lo hace.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] filename nombre del fichero a enviar.
 * @param[in] nick nick del usuario que enviará el fichero.
 * @param[in] data datos a ser enviados.
 * @param[in] length longitud de los datos a ser enviados.
 *
 * @retval TRUE si se ha establecido la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
boolean IRCInterface_SendFile(char *filename, char *nick, char *data, long unsigned int length)
{
	g_print(BLU "\nIRCInterface_SendFile(char *filename, char *nick, char *data, long unsigned int length) call\n" RESET);
	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_StartAudioChat IRCInterface_StartAudioChat
 *
 * @brief Llamada por el botón "Iniciar" del diálogo de chat de voz.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_StartAudioChat (char *nick)
 * @endcode
 * 
 * @description 
 * Llamada por el botón "Iniciar" del diálogo de chat de voz. Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función como todos los demás callbacks bloquea el interface
 * y por tanto para mantener la funcionalidad del chat de voz es imprescindible crear un hilo a efectos
 * de comunicación de voz.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] nick nick del usuario con el que se desea conectar.
 *
 * @retval TRUE si se ha establecido la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
boolean IRCInterface_StartAudioChat(char *nick)
{
	g_print(BLU "\nIRCInterface_StartAudioChat(char *nick) call\n" RESET);
	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_StopAudioChat IRCInterface_StopAudioChat
 *
 * @brief Llamada por el botón "Parar" del diálogo de chat de voz.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_StopAudioChat (char *nick)
 * @endcode
 * 
 * @description 
 * Llamada por el botón "Parar" del diálogo de chat de voz. Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función sólo para la comunicación que puede ser reiniciada. 
 * Evidentemente tiene que actuar sobre el hilo de chat de voz.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] nick nick del usuario con el que se quiere parar el chat de voz.
 *
 * @retval TRUE si se ha parado la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
boolean IRCInterface_StopAudioChat(char *nick)
{
	g_print(BLU "\nIRCInterface_StopAudioChat(char *nick) call\n" RESET);
	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_TakeOp IRCInterface_TakeOp
 *
 * @brief Llamada por el botón "Quitar Op". 
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_TakeOp (char *channel, char *nick)
 * @endcode
 * 
 * @description 
 * Llamada por el botón "Quitar Op". Previamente debe seleccionarse un nick del
 * canal para quitarle "op" a dicho usuario.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se va a quitar op al usuario.
 * @param[in] nick nick del usuario al que se le va a quitar op.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_TakeOp(char *channel, char *nick)
{
	g_print(BLU "\nIRCInterface_TakeOp(char *channel, char *nick) call\n" RESET);

	if (changeMode(channel, nick, "-o") != OK)
		g_print(RED "ERROR - In IRCInterface_TakeOp: Error en changeMode, devolvió ERR\n" RESET);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_TakeVoice IRCInterface_TakeVoice
 *
 * @brief Llamada por el botón "Quitar voz". 
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_TakeVoice (char *channel, char *nick)
 * @endcode
 * 
 * @description 
 * Llamada por el botón "Quitar voz". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se le va a quitar voz al usuario.
 * @param[in] nick nick del usuario al que se va a quitar la voz.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/
 
void IRCInterface_TakeVoice(char *channel, char *nick)
{
	g_print(BLU "\nIRCInterface_TakeVoice(char *channel, char *nick) call\n" RESET);

	if (changeMode(channel, nick, "-v") != OK)
		g_print(RED "ERROR - In IRCInterface_TakeVoice: Error en changeMode, devolvió ERR\n" RESET);	
}


/***************************************************************************************************/
/***************************************************************************************************/
/**                                                                                               **/
/** MMMMMMMMMM               MMMMM           AAAAAAA           IIIIIII NNNNNNNNNN          NNNNNN **/
/**  MMMMMMMMMM             MMMMM            AAAAAAAA           IIIII   NNNNNNNNNN          NNNN  **/
/**   MMMMM MMMM           MM MM            AAAAA   AA           III     NNNNN NNNN          NN   **/
/**   MMMMM  MMMM         MM  MM            AAAAA   AA           III     NNNNN  NNNN         NN   **/
/**   MMMMM   MMMM       MM   MM           AAAAA     AA          III     NNNNN   NNNN        NN   **/
/**   MMMMM    MMMM     MM    MM           AAAAA     AA          III     NNNNN    NNNN       NN   **/
/**   MMMMM     MMMM   MM     MM          AAAAA       AA         III     NNNNN     NNNN      NN   **/
/**   MMMMM      MMMM MM      MM          AAAAAAAAAAAAAA         III     NNNNN      NNNN     NN   **/
/**   MMMMM       MMMMM       MM         AAAAA         AA        III     NNNNN       NNNN    NN   **/
/**   MMMMM        MMM        MM         AAAAA         AA        III     NNNNN        NNNN   NN   **/
/**   MMMMM                   MM        AAAAA           AA       III     NNNNN         NNNN  NN   **/
/**   MMMMM                   MM        AAAAA           AA       III     NNNNN          NNNN NN   **/
/**  MMMMMMM                 MMMM     AAAAAA            AAAA    IIIII   NNNNNN           NNNNNNN  **/
/** MMMMMMMMM               MMMMMM  AAAAAAAA           AAAAAA  IIIIIII NNNNNNN            NNNNNNN **/
/**                                                                                               **/
/***************************************************************************************************/
/***************************************************************************************************/

int main (int argc, char *argv[])
{
	FILE* log_fp = NULL;

	//init de las funciones de log
	log_fp = initLog();
	if (log_fp == NULL){
		perror("error en initLog()");
		return EXIT_FAILURE;
	}

	if (pthread_mutex_init(&loglock, NULL) != OK){
		perror("\n mutex init ha devuelto error\n");
		return EXIT_FAILURE;
	}

	/* La función IRCInterface_Run debe ser llamada al final      */
	/* del main y es la que activa el interfaz gráfico quedándose */
	/* en esta función hasta que se pulsa alguna salida del       */
	/* interfaz gráfico.                                          */
	IRCInterface_Run(argc, argv);

	pthread_mutex_destroy(&loglock);

	return 0;
}
