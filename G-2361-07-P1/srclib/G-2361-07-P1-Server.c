
#include "../includes/G-2361-07-P1-Server.h"


char *fich, *prefix, *nick, *msg, *user, *modehost, *serverother, *realname, *server1, *server2, *channel, *key, *target;
char *maskarray, *msgtarget, *topic, *channeluser, *modo;
char *real = NULL, *ip = NULL, *away = NULL, *host = NULL, *nick_name = NULL, *usuario = NULL;
char listaNicks[NUM_SOCKETS][10];
long accion = 0, creacion = 0, id = 0;
int socketAlrm = 0, numeroUsuarios = 0;
int sockets[NUM_SOCKETS];




void procesar(char* entrada, int IDsocket, fd_set readset){
	char* next=NULL;
	char* comando=NULL;
	long int query;

	/*Coger el primer comando y en next se guarda el resto de la entrada*/
	next = IRC_UnPipelineCommands(entrada, &comando);

	/*Identificar comando*/
	query = IRC_CommandQuery(comando);
	parseCommand(query, comando, IDsocket);

	free(comando);
	comando = NULL;

	/*Mientras que siga habiendo comandos*/
	while(next != NULL){
		next = IRC_UnPipelineCommands(next, &comando);
		/*Identificar comando*/
		query = IRC_CommandQuery(comando);
		parseCommand(query, comando, IDsocket); 

		free(comando);
		comando = NULL;
	}
	free(comando);


}


void servidor(int puerto, char* path){
	int IDsocket, maxfd;
	fd_set readset, tempset;
	int descriptor, j, select_estado;
	char entrada[SIZE + 1];	

	/*Abrir socket*/
	IDsocket = abrir_socketTCP(puerto);

	FD_ZERO(&readset);
	FD_SET(IDsocket, &readset);
	maxfd = IDsocket;
	socketAlrm = IDsocket;
	fich = path;


	do{
		memcpy(&tempset, &readset, sizeof(tempset));
	  
	   	select_estado = select(maxfd + 1, &tempset, NULL, NULL, NULL);
	   	syslog(LOG_INFO, "SELECT");

	   	if (select_estado == 0) {
	   	   	
	   	}else{
	  		/*Accept*/
			if (FD_ISSET(IDsocket, &tempset)) {
		         descriptor = acepta_conexion(IDsocket);
		         syslog(LOG_INFO, "ACEPTAR CONEXION");
		         if (descriptor < 0) {
		            //syslog(LOG_INFO,"error in accept ");
		         }
		         else {
		            FD_SET(descriptor, &readset);
		            maxfd = (maxfd < descriptor)?descriptor:maxfd;
		         }
	      	}
	      	/*Read and process*/
	      	for (j = 0; j < maxfd+1; j++) {
	         	if (FD_ISSET(j, &tempset) && j != IDsocket ) {
		            do {
		               bzero(entrada, sizeof(entrada));
		               select_estado = read(j,entrada,SIZE);
		               syslog(LOG_INFO,"LEIDO: %s",entrada);

		               if(select_estado==-1){
		               		close(j);
		               		FD_CLR(j, &readset);
		               }
		            } while (select_estado == -1 && errno == EINTR);

		            if (select_estado > 0) {		            	
			            entrada[select_estado] = 0;
			            procesar(entrada,j,readset);
		            }
		            else if (select_estado == 0 ) {
		                //syslog(LOG_INFO,"ESTOY EN CLOSEEEEEEEE 1 %s j:%d",buscaUsuPorSocket(j),j);
		                //IRCTADUser_DeleteByNick (buscaUsuPorSocket(j));
		            	///////delUsuPorSocket(j);
		                //syslog(LOG_INFO,"ESTOY EN CLOSEEEEEEEE 2%s j:%d ",buscaUsuPorSocket(j),j);
		                //close(j);
		                FD_CLR(j, &readset);
		            }
		            else {
		               //syslog(LOG_INFO,"error in recv ");
		            }
		        }      
	     	}     
		}
	}while(1);
}


void liberaLista(char** lista, long nElems){
	int i;
	for(i=0;i<nElems;i++)
		free(lista[i]);
}

void parseCommand(long int query, char* comando, int IDsocket){

	long result;
	

	switch(query){

		case USER:

			result = IRCParse_User (comando, &prefix, &user, &modehost, &serverother, &realname);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "USER: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "USER: No se encuentran todos los parámetros obligatorios.");
			}else{
				/*Para ver si existe el usuario*/
				result = IRCTADUser_GetData (&id, &user, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				if(result!=IRC_OK){ /*Si el usuario no existe*/
					char *welcome_msj, *host_msj, *create_msj, *info_msj, *respuesta;

					strcpy(listaNicks[numeroUsuarios], user);
					sockets[numeroUsuarios] = IDsocket; /*Numero del socket del usuario*/
					numeroUsuarios++;


					/*Annadir un nuevo usuario*/
					IRCTADUser_New (user, nick, realname, NULL, modehost, "localhost", IDsocket);
					/*long nelements;
					char **nicklist;
					IRCTADUser_GetUserList(&nicklist, &nelements);
					syslog(LOG_INFO, "nusus %ld",nelements);*/

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
					syslog(LOG_INFO, "send USER");
					free(welcome_msj); free(host_msj), free(create_msj); free(info_msj); free(respuesta);
					

				}else{ /*Si existe, liberamos memoria*/
					free(user); free(nick_name); free(host); free(prefix);
					break;
				}


			}
			free(host);id = 0;free(real);free(ip);free(away);
			free(prefix); free(user);free(modehost);free(serverother);free(realname);
			break;

		case NICK:

			result = IRCParse_Nick (comando, &prefix, &nick, &msg);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "NICK: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "NICK: No se encuentran todos los parámetros obligatorios.");
			}else{

				if(!nick){
					syslog(LOG_INFO, "NICK: No hay nick.");
				}else{
					char *complex = NULL, *msj_nick = NULL;

					if(strlen(nick) < 10){ /*Longitud maxima de entrada para el nick*/
						usuario=NULL; nick_name=NULL; real=NULL;id=0;

						IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

						if(!usuario){

						}else{
							/*Crea el array por ejemplo : :celia!~celia@66.red-83-63-217.staticip.rima-tde.net*/
							syslog(LOG_INFO, "COMPLEX USER");
							IRC_ComplexUser(&complex, nick_name, usuario, host, NULL);

							IRCTADUser_Set(id, usuario, nick_name, NULL, usuario, nick, real);

							/*Mensaje de vuelta del comando NICK*/
							IRCMsg_Nick(&msj_nick, complex, NULL, nick);
							send(IDsocket, msj_nick, strlen(msj_nick), 0);

							free(complex); free(msj_nick); 
							free(nick_name); free(usuario); id = 0; free(real); free(host); 
						}

					}
				}

			}
			free(prefix); free(msg); 	

			break;

		case PING:
			result = IRCParse_Ping (comando, &prefix, &server1, &server2, &msg);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "PING: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "PING: No se encuentran todos los parámetros obligatorios.");
			}else{	/*Si todo ha salido bien*/
				char *pong_msj;

				if(!server2){ /*En el caso de que no se especifique el server2*/
					IRCMsg_Pong (&pong_msj, PREFIJO, PREFIJO, server2, server1);
					send(IDsocket,pong_msj,strlen(pong_msj),0);
				}else{
					IRCMsg_Pong (&pong_msj, PREFIJO, PREFIJO, server1, server2);
					send(IDsocket,pong_msj,strlen(pong_msj),0);
				}

				free(server1); free(server2); free(pong_msj); free(prefix);
			}

			break;

		case JOIN:
			result = IRCParse_Join(comando, &prefix, &channel, &key, &msg);
	        syslog (LOG_INFO, "Llega join para unirse al canal: %s", channel);

	        /*Cogemos la informacion del usuario actual*/
			usuario=NULL; nick_name=NULL; real=NULL;id=0;
			IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

	        if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "JOIN: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){ /*JOIN sin argumentos*/
				char *paramsErr_msj;
				syslog(LOG_INFO, "JOIN: No se encuentran todos los parámetros obligatorios.");

				IRCMsg_ErrNeedMoreParams (&paramsErr_msj, PREFIJO, nick_name, "JOIN");
				send(IDsocket,paramsErr_msj,strlen(paramsErr_msj),0);

				free(paramsErr_msj);

			}else{				
				if(!channel){

				}else{

					char **lista = NULL, **listaNicksOnChannel = NULL, **listaUsuarios = NULL;
					long nChannels = 0, nUsuarios = 0;
					char *join_msj = NULL, *complex = NULL, *endOfNames_msj = NULL, channel_aux[strlen(channel)+1], **uCanales, *nameMsg, *reply;
					int i, flag = 0, nstrings;

					/*Obtenemos la lista con los nombres de todos los canales*/
					IRCTADChan_GetList (&lista, &nChannels, NULL);
					for(i = 0; i < nChannels; i++){
						if(strcmp(lista[i], channel) == 0){
							flag = 1; /*El canal existe*/
							IRCTADChan_FreeList (lista, nChannels);
						}
					}

					/*Si no hay canal*/
					if(flag == 0){
						syslog(LOG_INFO, "Nuevo canal creado");
						/*Creamos un canal. Es 'o' de operator. key clave del canal*/
						result = IRCTAD_Join (channel, nick_name, "o", key);
					}else{/*Si hay canal*/
						syslog(LOG_INFO, "Ya existe el canal");
						result = IRCTAD_Join (channel, nick_name, "", key);

					}

					sprintf(channel_aux, ":%s", channel);
					syslog(LOG_INFO, "COMPLEX USER /JOIN");
					IRC_ComplexUser(&complex, nick_name, usuario, host, NULL);
					IRCMsg_Join(&join_msj, complex, channel_aux, key, msg);

					/*Listamos usuarios de un canal*/
					IRCTAD_ListNicksOnChannelArray(channel, &listaUsuarios, &nUsuarios);
					listaNicksOnChannel = listaUsuarios;

					IRCMsg_RplEndOfNames (&endOfNames_msj, PREFIJO, nick_name, channel);

					for(i = 0; i < nUsuarios; i++){
						syslog(LOG_INFO,"EN USUARIOS for %s",listaUsuarios[i]);
						if((IRCTAD_GetUserModeOnChannel (channel, listaUsuarios[i]) & IRCUMODE_OPERATOR)==IRCUMODE_OPERATOR)
							sprintf(listaNicksOnChannel[i],"@%s",listaUsuarios[i]);
						else
							sprintf(listaNicksOnChannel[i],"%s",listaUsuarios[i]);
					}


					IRC_BuildStringsFromList(&uCanales, 512, ' ', &nstrings, listaNicksOnChannel, nUsuarios);
					
					for(i = 0; i < nstrings; i++){
						IRCMsg_RplNamReply(&nameMsg,PREFIJO, nick_name, "=", channel, uCanales[i]);
						IRC_PipelineCommands(&reply,join_msj,nameMsg,endOfNames_msj,NULL);
						send(IDsocket,reply,strlen(reply),0);
					}					

					free(join_msj); free(complex); free(endOfNames_msj);free(uCanales); 
					liberaLista(listaUsuarios,nUsuarios); free(reply); free(nameMsg);
				}
			}
			free(prefix); free(channel); free(key); free(msg); 

			break;

		case LIST:

			result = IRCParse_List(comando, &prefix, &channel, &target);
	        syslog (LOG_INFO, "List: %s", channel);

	        if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "LIST: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "LIST: No se encuentran todos los parámetros obligatorios.");
			}else{
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

						usuario=NULL; nick_name=NULL; real=NULL; id=0;
						/*Obtenemos datos del usuario*/
						IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);
						/*Mensaje de respuesta*/
						IRCMsg_RplList(&can, PREFIJO, nick_name, listaCanales[i], nUsusCanales, "");
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

			free(channel);free(prefix);free(target);
			break;

		case WHOIS:

			result = IRCParse_Whois (comando, &prefix, &target, &maskarray);
	        syslog (LOG_INFO, "WHOIS");


			usuario=NULL; nick_name=NULL; real=NULL;id=0;
			IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);
			
			if(maskarray == NULL){
				char *nick_no;
				IRCMsg_ErrNoNickNameGiven (&nick_no, PREFIJO, nick_name);
				send(IDsocket,nick_no,strlen(nick_no),0);
				free(nick_no);

			}else{

				char *who_msj, *serv, *ch, *op, *idl, *end_who;
				char **listaCh, **listChannel, **uCanales;
				long numberOfChannels;
				int nstrings, i;

				IRCMsg_RplWhoIsUser (&who_msj, PREFIJO, maskarray, maskarray, usuario, host, maskarray);
				send(IDsocket,who_msj,strlen(who_msj),0);


				IRCTAD_ListChannelsOfUserArray(maskarray, maskarray, &listaCh, &numberOfChannels);
				listChannel = listaCh;

				syslog(LOG_INFO, "Numero de canales %ld", numberOfChannels);
				for(i = 0; i < numberOfChannels; i++){
					char aux[strlen(listaCh[i])+1];
					sprintf(aux,"@%s",listaCh[i]);
					strcpy(listChannel[i],aux);
				}

				IRC_BuildStringsFromList(&uCanales, 512, ' ', &nstrings, listChannel, numberOfChannels);

				for(i = 0; i < nstrings; i++){
					IRCMsg_RplWhoIsServer(&serv, PREFIJO, maskarray, maskarray, host, "info who is");
					send(IDsocket,serv,strlen(serv),0);

					IRCMsg_RplWhoIsChannels(&ch, PREFIJO, maskarray, maskarray, uCanales[i]);
					send(IDsocket,ch,strlen(ch),0);

					IRCMsg_RplWhoIsOperator(&op, PREFIJO, maskarray, maskarray);
					send(IDsocket,op,strlen(op),0);

					IRCMsg_RplWhoIsIdle(&idl, PREFIJO, maskarray, maskarray, 0, ":seconds idle, signon time");
					send(IDsocket,idl,strlen(idl),0);
				}	


				if(away != NULL){
					char *away_msj;
					/*syslog(LOG_INFO, "Dentro AWAY");*/
					IRCMsg_RplAway (&away_msj, PREFIJO, maskarray, maskarray, away);
					send(IDsocket,away_msj,strlen(away_msj),0);
					free(away_msj);
				}


				IRCMsg_RplEndOfWhoIs (&end_who, PREFIJO, maskarray, maskarray);
				send(IDsocket,end_who,strlen(end_who),0);


				free(who_msj); free(uCanales); free(serv); free(ch); 
				free(op); free(idl); free(end_who);
				IRCTADChan_FreeList (listaCh, numberOfChannels);
			}
			break;

		case NAMES:
			result = IRCParse_Names (comando, &prefix, &channel, &target);
	        syslog (LOG_INFO, "NAMES");

	        if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "NAMES: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "NAMES: No se encuentran todos los parámetros obligatorios.");
			}else{
				char **listaUsuarios = NULL, **listaNicksOnChannel = NULL, **uCanales = NULL;
				char *endOfNames_msj = NULL, *nameMsg = NULL, *reply = NULL;
				long nUsuarios = 0;
				int i, nstrings;

				/*Listamos usuarios de un canal*/
				IRCTAD_ListNicksOnChannelArray(channel, &listaUsuarios, &nUsuarios);

				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				syslog(LOG_INFO, "NUMERO DE USUARIOS %ld", nUsuarios);
				if(nUsuarios > 0){ /*Si no hay usuarios*/
					listaNicksOnChannel = listaUsuarios;
					IRCMsg_RplEndOfNames(&endOfNames_msj, PREFIJO, nick_name, channel);

					for(i = 0; i < nUsuarios; i++){
						if((IRCTAD_GetUserModeOnChannel (channel, listaUsuarios[i]) & IRCUMODE_OPERATOR)==IRCUMODE_OPERATOR)
							sprintf(listaNicksOnChannel[i],"@%s",listaUsuarios[i]);
						else
							sprintf(listaNicksOnChannel[i],"%s",listaUsuarios[i]);
					}


					IRC_BuildStringsFromList(&uCanales, 512, ' ', &nstrings, listaNicksOnChannel, nUsuarios);
					
					for(i = 0; i < nstrings; i++){
						IRCMsg_RplNamReply(&nameMsg,PREFIJO, nick_name, "=", channel, uCanales[i]);
						IRC_PipelineCommands(&reply,nameMsg,endOfNames_msj,NULL);
						send(IDsocket,reply,strlen(reply),0);
					}					

					free(reply); free(nameMsg); free(uCanales);
				}else{
					IRCMsg_RplEndOfNames (&endOfNames_msj, PREFIJO, nick_name, channel);
					send(IDsocket,endOfNames_msj,strlen(endOfNames_msj),0);

				}

				liberaLista(listaUsuarios, nUsuarios);
				free(endOfNames_msj);

			}

			free(channel);free(prefix);free(target);
			break;

		case PRIVMSG:

		 	result = IRCParse_Privmsg (comando, &prefix, &msgtarget, &msg);
	        syslog (LOG_INFO, "PRIVMSG");

	        if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "PRIVMSG: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "PRIVMSG: No se encuentran todos los parámetros obligatorios.");
			}else{
				char *privmsg_msj, *complex;
				char **lista;
				int i, j, socket_aux, flag = 0;
				long nChannels = 0;

				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				/*Obtenemos la lista con los nombres de todos los canales*/
				IRCTADChan_GetList (&lista, &nChannels, NULL);
				for(i = 0; i < nChannels; i++){
					if(strcmp(lista[i], msgtarget) == 0){
						flag = 1; /*El canal existe*/
						IRCTADChan_FreeList (lista, nChannels);
					}
				}


				if(msgtarget != NULL){ /*Si hay usuario*/
					syslog(LOG_INFO, "COMPLEX USER /PRIVMSG");
					IRC_ComplexUser(&complex, nick_name, usuario, NULL, NULL);

					IRCMsg_Privmsg (&privmsg_msj, complex, msgtarget, msg);

					for(i = 0; i < numeroUsuarios; i++){
						if(strncmp(msgtarget, listaNicks[i], strlen(listaNicks[i]))){
							socket_aux = sockets[i];
						}
					}

					if(away != NULL){
						char *away_msj;
						/*syslog(LOG_INFO, "Dentro AWAY");*/
						IRCMsg_RplAway (&away_msj, PREFIJO, usuario, usuario, away);
						send(IDsocket,away_msj,strlen(away_msj),0);
						free(away_msj);
					}

					send(socket_aux, privmsg_msj, strlen(privmsg_msj), 0);
					free(privmsg_msj); free(complex);
				}else if(flag == 1){ /*Si existe el canal*/
					char *can, **listaUsus, *complex;
					long nUsuarios = 0;
					/*Obtenemos la lista con los nombres de todos los canales*/
					IRCTADChan_GetList (&lista, &nChannels, NULL);
					for(i = 0; i < nChannels; i++){
						if(strncmp(lista[i], msgtarget, strlen(msgtarget)) == 0){
							can = (char *) malloc (strlen(lista[i])+1);
							strcpy(can, lista[i]);
						}
					}

					if(can != NULL){
						/*Listamos usuarios de un canal*/
						IRCTAD_ListNicksOnChannelArray(can, &listaUsus, &nUsuarios);	
						for(i = 0; i < nUsuarios; i++){
							if(strncmp(nick_name,listaUsus[i],strlen(nick_name))!=0){
								IRC_ComplexUser(&complex, nick_name, usuario, host, NULL);

								IRCMsg_Privmsg(&privmsg_msj, complex, can, msg);

								for(j = 0; j < numeroUsuarios; j++){
									if(strncmp(listaUsus[i], listaNicks[j], strlen(listaUsus[i])) == 0){
										send(sockets[j], privmsg_msj, strlen(privmsg_msj), 0);
									}
								}

								free(complex); free(privmsg_msj);
							}

						}

						if(away != NULL){
							char *away_msj;
							/*syslog(LOG_INFO, "Dentro AWAY");*/
							IRCMsg_RplAway (&away_msj, PREFIJO, usuario, usuario, away);
							send(IDsocket,away_msj,strlen(away_msj),0);
							free(away_msj);
						}

						free(can); free(msgtarget);
						liberaLista(listaUsus, numeroUsuarios);

					}


				}else{
					char *noDest_msj;
					IRCMsg_ErrNoSuchNick(&noDest_msj, PREFIJO, nick_name, msgtarget);
					send(IDsocket,noDest_msj,strlen(noDest_msj),0);
					free(noDest_msj);
				}

			}

			free(msgtarget); free(prefix); free(msg);

			break;

		case PART: /*Usuario abandona correctamente el canal*/
			result =  IRCParse_Part (comando, &prefix, &channel, &msg);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "PART: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "PART: No se encuentran todos los parámetros obligatorios.");
			}else{
				char *part_msj, **lista = NULL;
				long nChannels = 0;
				int i, flag = 0;

				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				/*Obtenemos la lista con los nombres de todos los canales*/
				IRCTADChan_GetList (&lista, &nChannels, NULL);
				for(i = 0; i < nChannels; i++){
					if(strcmp(lista[i], channel) == 0){
						flag = 1; /*El canal existe*/
						IRCTADChan_FreeList (lista, nChannels);
					}
				}

				if(flag == 0){ /*No existe el canal*/
					IRCMsg_ErrNoSuchChannel(&part_msj, PREFIJO, nick_name, channel);
					send(IDsocket,part_msj,strlen(part_msj),0);
				}else{
					long nUsus = 0;

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

			free(channel); free(prefix); free(msg);

			break;

		case TOPIC: /*Cambia el nombre de un canal*/

			result = IRCParse_Topic (comando, &prefix, &channel, &topic);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "TOPIC: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "TOPIC: No se encuentran todos los parámetros obligatorios.");
			}else{
				char *topic_msj = NULL;
				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				if(topic == NULL){ /*Si no hay topic*/
					char *tpc = NULL;

					/*Obtenemos el topic del canal*/
					IRCTAD_GetTopic (channel, &tpc);

					if(tpc == NULL){
						IRCMsg_RplNoTopic (&topic_msj, PREFIJO, nick_name,channel);
					}else{
						IRCMsg_RplTopic (&topic_msj, PREFIJO, nick_name , channel, tpc);
					}
					send(IDsocket,topic_msj,strlen(topic_msj),0);
					free(tpc); 

				}else{ /*Si hay topic*/
					long modeUsuChannel = 0, modeChannel = 0, modeVal = 0, modeValUsu = 0;
					
					/*Modo usuario en un canal*/
					modeUsuChannel = IRCTAD_GetUserModeOnChannel(channel, nick_name);
					/*Modo del canal*/
					modeChannel = IRCTADChan_GetModeInt(channel);

					modeVal = modeChannel & IRCMODE_TOPICOP;
					modeValUsu = modeUsuChannel & IRCUMODE_OPERATOR;

					if(modeVal != IRCMODE_TOPICOP || modeValUsu == IRCUMODE_OPERATOR){
						IRCMsg_Topic(&topic_msj, PREFIJO, channel, topic);
						/*Se cambia el topic del canal y todos los parametros asociados*/
						IRCTAD_SetTopic (channel, nick_name, topic);
					}else{
						/*"You're not channel operator"*/
						IRCMsg_ErrChanOPrivsNeeded(&topic_msj, PREFIJO, nick_name, channel);
					}

					send(IDsocket,topic_msj,strlen(topic_msj),0);
					free(topic);
				}
				free(topic_msj);
			}

			free(channel); free(prefix);
			break;

		case KICK: /*Expulsion de un usuario*/
			result =  IRCParse_Kick (comando, &prefix, &channel, &user, &msg);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "KICK: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "KICK: No se encuentran todos los parámetros obligatorios.");
			}else{

				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				if(user != NULL){
					if(channel != NULL){
						int i, socket_aux;
						long modeUsuChannel, modeValUsu;
						char *complex, *kick_msj;


						for(i = 0; i < numeroUsuarios; i++){
							if(strncmp(user, listaNicks[i], strlen(listaNicks[i])) == 0){
								socket_aux = sockets[i];
							}
						}

						/*Modo usuario en un canal*/
						modeUsuChannel = IRCTAD_GetUserModeOnChannel(channel, nick_name);
						modeValUsu = modeUsuChannel & IRCUMODE_OPERATOR;

						IRC_ComplexUser(&complex,nick_name,usuario,NULL,NULL);
						if (modeValUsu==IRCUMODE_OPERATOR){
							IRCTAD_KickUserFromChannel (channel, nick_name);
							IRCMsg_Kick (&kick_msj, complex, channel, user, msg);
							send(socket_aux,kick_msj,strlen(kick_msj),0);
						}else{
							/*"You're not channel operator"*/
							IRCMsg_ErrChanOPrivsNeeded(&kick_msj, complex, nick_name, channel);
							send(IDsocket,kick_msj,strlen(kick_msj),0);
						}
						free(kick_msj); free(complex);

					}

				}

			}
			free(prefix); free(channel); free(msg);
			break;

		case AWAY:
			result = IRCParse_Away (comando, &prefix, &msg);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "AWAY: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "AWAY: No se encuentran todos los parámetros obligatorios.");
			}else{
				char *away_msj;

				usuario = NULL; nick_name = NULL; real = NULL; id = 0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				if(away == NULL){
					syslog(LOG_INFO, "AWAY: No hay away");
					IRCTADUser_SetAway (id, usuario, nick_name, real, msg);
					IRCMsg_RplNowAway (&away_msj, PREFIJO, nick_name);
				}else{
					syslog(LOG_INFO, "AWAY: Hay away");
					/*Se borra*/
					IRCTADUser_SetAway (id, usuario, nick_name, real, NULL);
					IRCMsg_RplUnaway (&away_msj, PREFIJO, nick_name);
				}
				send(IDsocket,away_msj,strlen(away_msj),0);
				free(away_msj);

			}

			free(prefix); free(msg);

			break;


		case MODE:
			result = IRCParse_Mode (comando, &prefix, &channeluser, &modo, &key);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "MODE: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "MODE: No se encuentran todos los parámetros obligatorios.");
			}else{
				char *mode_msj;
				long modeUsuChannel, modeValUsu;

				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

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
			free(prefix); free(channeluser); free(modo);
			break;

		case QUIT:
			result = IRCParse_Quit (comando, &prefix, &msg);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "QUIT: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "QUIT: No se encuentran todos los parámetros obligatorios.");
			}else{
				char *quit_msj;
				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				IRCTAD_Quit (nick_name);
				IRCMsg_Quit (&quit_msj, PREFIJO, msg);
				send(IDsocket,quit_msj,strlen(quit_msj),0);
				free(quit_msj);
			}

			/**********ELIMINAR USUARIO???*************/
			free(prefix); free(msg);

			break;

		case MOTD:
			result = IRCParse_Motd (comando, &prefix, &target);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "MOTD: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "MOTD: No se encuentran todos los parámetros obligatorios.");
			}else{
				FILE *f;
				char lectura[250];
				char *motdStart_msj, *motd_msj, *motdEnd_msj;
				f = fopen(fich,"r");

				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				if(!target)
					IRCMsg_RplMotdStart (&motdStart_msj, PREFIJO, usuario, PREFIJO);
				else
					IRCMsg_RplMotdStart (&motdStart_msj, PREFIJO, usuario, target);

				send(IDsocket,motdStart_msj,strlen(motdStart_msj),0);

				while(fgets(lectura, 250, f) != NULL){
					lectura[strlen(lectura)-1]=0;
					IRCMsg_RplMotd(&motd_msj, PREFIJO, usuario, lectura);
					send(IDsocket,motd_msj,strlen(motd_msj),0);

				}

				fclose(f);
				IRCMsg_RplEndOfMotd (&motdEnd_msj, PREFIJO, usuario);
				send(IDsocket,motdEnd_msj,strlen(motdEnd_msj),0);

				free(motd_msj); free(motdStart_msj); free(motdEnd_msj);
			}
			free(prefix); free(target);

			break;


		case IRCERR_UNKNOWNCOMMAND: /*Comando desconocido*/
			if(strstr(comando,"CAP") != NULL){

			}else{
				char *unk_msj;
				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				IRCMsg_ErrUnKnownCommand (&unk_msj, PREFIJO, nick_name, comando);
				//syslog(LOG_INFO,"mens:%s",unkCom);
				send(IDsocket,unk_msj,strlen(unk_msj),0);
				free(unk_msj);
			}

			break;

		default:
			break;



	}





}





