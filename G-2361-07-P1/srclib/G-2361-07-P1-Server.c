
#include "../includes/G-2361-07-P1-Server.h"
#include "../includes/G-2361-07-P1-Functions.h"


pthread_mutex_t user_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t canal_mutex = PTHREAD_MUTEX_INITIALIZER;

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

		         }else {
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
		               syslog(LOG_INFO,"LEIDO: %s, %d",entrada, select_estado);

		               if(select_estado ==-1){
		               		close(j);
		               		FD_CLR(j, &readset);
		               }
		            } while (select_estado == -1 && errno == EINTR);

		            if (select_estado > 0) {		            	
			            entrada[select_estado] = 0;
			            procesar(entrada,j,readset);
		            }else if (select_estado == 0) {
		            	int z, k;
						for(z = 0; z < numeroUsuarios; z++){
							if(sockets[z] == j){
								id = 0; usuario = NULL; real = NULL;
								IRCTADUser_Delete (id, usuario, listaNicks[z], real);

								for(k = z; k < numeroUsuarios; k++){
									strcpy(listaNicks[k], "");
									sockets[k] = sockets[k+1];
									strcpy(listaNicks[k], listaNicks[k+1]);
								}
								strcpy(listaNicks[numeroUsuarios], "");
								sockets[numeroUsuarios] = -1;
								numeroUsuarios--;
							}
						}
						
						/*syslog(LOG_INFO, "Numero de usuarios %d", numeroUsuarios);
						for(z = 0; z < numeroUsuarios; z++){
							syslog(LOG_INFO, "Usuario %d : %s", z, listaNicks[z]);
						}*/
						//shutdown(j, SHUT_RDWR);
						close(j);
		                /*syslog(LOG_INFO,"ESTOY EN CLOSEEEEEEEE j:%d ",j);*/
		                FD_CLR(j, &readset);
		            }else {
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
			syslog(LOG_INFO, "%s, %s, %s, %s, %s", prefix, user, modehost, serverother, realname);
			pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "USER: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "USER: No se encuentran todos los parámetros obligatorios.");
			}else{
				/*Para ver si existe el usuario*/
				nick_name = NULL; real = NULL; id = 0;
				result = IRCTADUser_GetData (&id, &user, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				if(!nick_name){ /*Si el usuario no existe*/
					/*Annadir un nuevo usuario*/
					IRCTADUser_New (user, nick, realname, NULL, modehost, serverother, IDsocket);
					IRCTADUser_SetAway (id, user, nick, realname, NULL);
					strcpy(listaNicks[numeroUsuarios], "");
					strcpy(listaNicks[numeroUsuarios], nick);
					sockets[numeroUsuarios] = IDsocket; /*Numero del socket del usuario*/
					numeroUsuarios++;

					funcionUser(user, nick, realname, modehost, IDsocket);


				}else{ /*Si existe, liberamos memoria*/
					free(user); free(modehost); free(serverother); free(realname);
					pthread_mutex_unlock(&canal_mutex);
					pthread_mutex_unlock(&user_mutex);
					break;
				}
			}

			free(user);free(modehost);free(serverother);free(realname);
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;

		case NICK:

			result = IRCParse_Nick (comando, &prefix, &nick, &msg);
			pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "NICK: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "NICK: No se encuentran todos los parámetros obligatorios.");
			}else{

				if(!nick){
					syslog(LOG_INFO, "NICK: No hay nick.");
				}else if(strlen(nick) < 10){ /*Longitud maxima de entrada para el nick*/
					usuario=NULL; nick_name=NULL; real=NULL;id=0;
					IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);
					if(!usuario){

					}else{
						funcionNick(id,usuario, nick_name, real, nick, IDsocket);
						free(nick);	
					}
				}		
			}
			free(prefix); free(msg); 
			pthread_mutex_unlock(&user_mutex);
			pthread_mutex_unlock(&canal_mutex);

			break;

		case PING:
			result = IRCParse_Ping (comando, &prefix, &server1, &server2, &msg);
			pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "PING: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "PING: No se encuentran todos los parámetros obligatorios.");
			}else{	/*Si todo ha salido bien*/
				funcionPing(server1, server2, IDsocket);
				free(server1); free(server2); free(prefix); free(msg);
			}
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;

		case JOIN:
			result = IRCParse_Join(comando, &prefix, &channel, &key, &msg);
	        pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);
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
				pthread_mutex_unlock(&canal_mutex);
				pthread_mutex_unlock(&user_mutex);
				break;

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
						/*syslog(LOG_INFO, "Nuevo canal creado");*/
						/*Creamos un canal. Es 'o' de operator. key clave del canal*/
						result = IRCTAD_Join (channel, nick_name, "o", key);
					}else{/*Si hay canal*/
						/*syslog(LOG_INFO, "Ya existe el canal");*/
						result = IRCTAD_Join (channel, nick_name, "", key);

						if(result != IRC_OK){ /*Canal protegido con clave*/
							IRCMsg_ErrBadChannelKey (&join_msj, PREFIJO, nick_name, channel);
							send(IDsocket,join_msj,strlen(join_msj),0);
							free(join_msj); free(prefix); free(channel); free(key); free(msg); 
							pthread_mutex_unlock(&canal_mutex);
							pthread_mutex_unlock(&user_mutex);
							break;
						}
					}

					sprintf(channel_aux, ":%s", channel);
					/*syslog(LOG_INFO, "COMPLEX USER /JOIN");*/
					IRC_ComplexUser(&complex, nick_name, usuario, host, NULL);
					IRCMsg_Join(&join_msj, complex, channel_aux, key, msg);

					/*Listamos usuarios de un canal*/
					IRCTAD_ListNicksOnChannelArray(channel, &listaUsuarios, &nUsuarios);
					listaNicksOnChannel = listaUsuarios;

					IRCMsg_RplEndOfNames (&endOfNames_msj, PREFIJO, nick_name, channel);

					for(i = 0; i < nUsuarios; i++){
						/*syslog(LOG_INFO,"EN USUARIOS for %s",listaUsuarios[i]);*/
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
					free(channel);
				}
			}
			free(prefix); free(msg); free(key);
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;

		case LIST:

			result = IRCParse_List(comando, &prefix, &channel, &target);
	        pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);
	        /*syslog (LOG_INFO, "List: %s", channel);*/

	        if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "LIST: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "LIST: No se encuentran todos los parámetros obligatorios.");
			}else{
				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);
				funcionList(nick_name, target, IDsocket);
			}

			free(channel);free(prefix);free(target);
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;

		case WHOIS:

			result = IRCParse_Whois (comando, &prefix, &target, &maskarray);
	        pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);
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

				IRCMsg_RplWhoIsUser (&who_msj, PREFIJO, maskarray, maskarray, nick_name, host, maskarray);
				send(IDsocket,who_msj,strlen(who_msj),0);


				IRCTAD_ListChannelsOfUserArray(maskarray, maskarray, &listaCh, &numberOfChannels);
				listChannel = listaCh;

				syslog(LOG_INFO, "Numero de canales %ld", numberOfChannels);
				for(i = 0; i < numberOfChannels; i++){
					char aux[strlen(listaCh[i])+1];
					sprintf(aux,"@%s",listaCh[i]);
					strcpy(listChannel[i],aux);
				}


				
				IRCMsg_RplWhoIsServer(&serv, PREFIJO, maskarray, maskarray, host, "info who is");
				send(IDsocket,serv,strlen(serv),0);

				if(numberOfChannels > 0){
					IRC_BuildStringsFromList(&uCanales, 512, ' ', &nstrings, listChannel, numberOfChannels);

					for(i = 0; i < nstrings; i++){
						IRCMsg_RplWhoIsChannels(&ch, PREFIJO, maskarray, maskarray, uCanales[i]);
						send(IDsocket,ch,strlen(ch),0);
					}
					free(ch); free(uCanales);
					IRCTADChan_FreeList (listaCh, numberOfChannels);
				}

				
				IRCMsg_RplWhoIsOperator(&op, PREFIJO, maskarray, maskarray);
				send(IDsocket,op,strlen(op),0);

				IRCMsg_RplWhoIsIdle(&idl, PREFIJO, maskarray, maskarray, 0, ":seconds idle, signon time");
				send(IDsocket,idl,strlen(idl),0);

				if(away != NULL){
					char *away_msj;
					/*syslog(LOG_INFO, "Dentro AWAY");*/
					IRCMsg_RplAway (&away_msj, PREFIJO, maskarray, maskarray, away);
					send(IDsocket,away_msj,strlen(away_msj),0);
					free(away_msj);
				}


				IRCMsg_RplEndOfWhoIs (&end_who, PREFIJO, maskarray, maskarray);
				send(IDsocket,end_who,strlen(end_who),0);


				free(who_msj); free(serv); free(maskarray);
				free(op); free(idl); free(end_who);
				
			}
			free(prefix); free(target); 
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;

		case NAMES:
			result = IRCParse_Names (comando, &prefix, &channel, &target);
	        pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);
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
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;

		case PRIVMSG:

			/*msgtarget es el nick del usuario que va a recibir el mensaje*/
		 	result = IRCParse_Privmsg (comando, &prefix, &msgtarget, &msg);
	        pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);
	        syslog (LOG_INFO, "PRIVMSG");

	        if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "PRIVMSG: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "PRIVMSG: No se encuentran todos los parámetros obligatorios.");
			}else{
				char *privmsg_msj, *complex, *can, *away_msj;
				char **lista, **listaDeNicks;
				int i, j, socket_aux, flagCanal = 0, flagNicks = 0;
				long nChannels = 0, nelementsNicks = 0;

				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario que va a enviar el mensaje*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				/*Obtenemos la lista con los nombres de todos los canales*/
				IRCTADChan_GetList (&lista, &nChannels, NULL);
				for(i = 0; i < nChannels; i++){
					printf("\ncanal %d = \"%s\" y msgtarget = \"%s\"\n", i, lista[i], msgtarget);
					if(strcmp(lista[i], msgtarget) == 0){
						flagCanal = 1; /*El canal existe*/
						can = (char *) malloc (strlen(lista[i])+1);
						strcpy(can, lista[i]);
						IRCTADChan_FreeList (lista, nChannels);
					}
				}
				

				IRCTADUser_GetNickList (&listaDeNicks, &nelementsNicks);
				for(i = 0; i < nelementsNicks; i++){
					if(strcmp(listaDeNicks[i], msgtarget) == 0){
						flagNicks = 1; /*El canal existe*/
						liberaLista(listaDeNicks, nelementsNicks);
					}
				}
				

				if(flagCanal == 1){ /*Si existe el canal*/
					char **listaUsus;
					long nUsuarios = 0;

					/*syslog(LOG_INFO, "HAY CANAL");*/
					if(can != NULL){
						/*Listamos usuarios de un canal*/
						IRCTAD_ListNicksOnChannelArray(can, &listaUsus, &nUsuarios);	
						for(i = 0; i < nUsuarios; i++){
							if(strncmp(nick_name,listaUsus[i],strlen(nick_name))!=0){
								/*syslog(LOG_INFO, "COMPLEX/CANAL");*/
								IRC_ComplexUser(&complex, nick_name, listaUsus[i], NULL, NULL);

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
							/*syslog(LOG_INFO, "Dentro AWAY");*/
							IRCMsg_RplAway (&away_msj, PREFIJO, usuario, usuario, away);
							send(IDsocket,away_msj,strlen(away_msj),0);
							free(away_msj);
						}

						free(can); 
						liberaLista(listaUsus, nUsuarios);
					}


				}else if(flagNicks == 1){ /*Si el receptor existe*/
					/*syslog(LOG_INFO, "COMPLEX USER /PRIVMSG");*/
					IRC_ComplexUser(&complex, nick_name, usuario, NULL, NULL);

					IRCMsg_Privmsg (&privmsg_msj, complex, msgtarget, msg);

					for(i = 0; i < numeroUsuarios; i++){
						if(strncmp(msgtarget, listaNicks[i], strlen(listaNicks[i])) == 0){
							socket_aux = sockets[i];
						}
					}

					if(away != NULL){
						/*syslog(LOG_INFO, "Dentro AWAY");*/
						IRCMsg_RplAway (&away_msj, PREFIJO, nick_name, usuario, away);
						send(IDsocket,away_msj,strlen(away_msj),0);
						free(away_msj);
					}

					send(socket_aux, privmsg_msj, strlen(privmsg_msj), 0);
					free(privmsg_msj); free(complex);
				}else{
					char *noDest_msj;
					/*syslog(LOG_INFO, "NOOOOO HAY DESTINATARIO");*/
					IRCMsg_ErrNoSuchNick(&noDest_msj, PREFIJO, nick_name, msgtarget);
					send(IDsocket,noDest_msj,strlen(noDest_msj),0);
					free(noDest_msj); 
				}
				

			}
			free(msgtarget); free(prefix); free(msg); 
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;

		case PART: /*Usuario abandona correctamente el canal*/
			result =  IRCParse_Part (comando, &prefix, &channel, &msg);
			pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "PART: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "PART: No se encuentran todos los parámetros obligatorios.");
			}else{
				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				funcionPart(channel, nick_name, msg, IDsocket);
			}

			free(channel); free(prefix); free(msg);
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;

		case TOPIC: /*Cambia el nombre de un canal*/

			result = IRCParse_Topic (comando, &prefix, &channel, &topic);
			pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);

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
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;

		case KICK: /*Expulsion de un usuario*/
			result =  IRCParse_Kick (comando, &prefix, &channel, &user, &msg);
			pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);

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

						IRC_ComplexUser(&complex,usuario,usuario,NULL,NULL);
						if (modeValUsu==IRCUMODE_OPERATOR){
							IRCTAD_KickUserFromChannel (channel, user);
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
			free(prefix); free(channel); free(msg);free(user);
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;

		case AWAY:
			result = IRCParse_Away (comando, &prefix, &msg);
			pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "AWAY: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "AWAY: No se encuentran todos los parámetros obligatorios.");
			}else{
				usuario = NULL; nick_name = NULL; real = NULL; id = 0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				funcionAway(id, usuario, nick_name, real, away, msg, IDsocket);

			}

			free(prefix); free(msg);
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;


		case MODE:
			printf("\nENTRA MODE Y NO SALGO\n");
			result = IRCParse_Mode (comando, &prefix, &channeluser, &modo, &key);
			pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "MODE: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "MODE: No se encuentran todos los parámetros obligatorios.");
			}else{
				char *mode_msj; //*
				long modeUsuChannel, modeValUsu; //*

				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);
				//funcionMode(usuario, nick_name, channeluser, modo, key, IDsocket);
				/*Modo usuario en un canal*/
				modeUsuChannel = IRCTAD_GetUserModeOnChannel(channeluser, nick_name);
				modeValUsu = modeUsuChannel & IRCUMODE_OPERATOR;

				if(modeValUsu == IRCUMODE_OPERATOR){
					/*Cambia modo de un canal*/
					IRCTAD_Mode (channeluser, nick_name, modo);
					//<- por esta zona peta
					if(strstr(modo,"k")!=NULL){
						IRCTADChan_SetPassword (channeluser,key);
						free(key);
					}
					printf("\nCHECKPOINT\n");	
					IRCMsg_Mode (&mode_msj, PREFIJO, channeluser, modo, usuario);
					send(IDsocket,mode_msj,strlen(mode_msj),0);
					free(mode_msj);
				}
			}
			free(prefix); free(channeluser); free(modo);
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;

		case QUIT:
			
			result = IRCParse_Quit (comando, &prefix, &msg);
			pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "QUIT: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "QUIT: No se encuentran todos los parámetros obligatorios.");
			}else{
				char *quit_msj;

				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				IRCTAD_Quit(nick_name);
				IRCMsg_Quit(&quit_msj, PREFIJO, msg);
				send(IDsocket,quit_msj,strlen(quit_msj),0);
				
				free(quit_msj);
			}

			free(prefix); free(msg);	

			int z, k;
			for(z = 0; z < numeroUsuarios; z++){
				if(sockets[z] == IDsocket){
					
					for(k = z; k < numeroUsuarios; k++){
						strcpy(listaNicks[k], "");
						sockets[k] = sockets[k+1];
						strcpy(listaNicks[k], listaNicks[k+1]);
					}
					strcpy(listaNicks[numeroUsuarios], "");
					sockets[numeroUsuarios] = -1;
					numeroUsuarios--;
				}
			}		

			shutdown(IDsocket, SHUT_RDWR);
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			/**/
			break;

		case MOTD:
			result = IRCParse_Motd (comando, &prefix, &target);
			pthread_mutex_lock(&canal_mutex);
			pthread_mutex_lock(&user_mutex);

			if(result == IRCERR_NOSTRING){
				syslog(LOG_INFO, "MOTD: No se ha introducido ninguna cadena para parsear.");
			}else if(result == IRCERR_ERRONEUSCOMMAND){
				syslog(LOG_INFO, "MOTD: No se encuentran todos los parámetros obligatorios.");
			}else{
				usuario = NULL; nick_name=NULL; real=NULL; id=0;
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);
				funcionMotd(nick_name, target, IDsocket, fich);
			}
			free(prefix); free(target);
			pthread_mutex_unlock(&canal_mutex);
			pthread_mutex_unlock(&user_mutex);
			break;

		case IRCERR_UNKNOWNCOMMAND: /*Comando desconocido*/
			if(strstr(comando,"CAP") != NULL){

			}else{
				char *unk_msj;
				pthread_mutex_lock(&canal_mutex);
				pthread_mutex_lock(&user_mutex);

				usuario=NULL; nick_name=NULL; real=NULL; id=0;
				/*Obtenemos datos del usuario*/
				IRCTADUser_GetData (&id, &usuario, &nick_name, &real, &host, &ip, &IDsocket, &creacion, &accion, &away);

				IRCMsg_ErrUnKnownCommand (&unk_msj, PREFIJO, nick_name, comando);
				//syslog(LOG_INFO,"mens:%s",unkCom);
				send(IDsocket,unk_msj,strlen(unk_msj),0);
				free(unk_msj);
				pthread_mutex_unlock(&canal_mutex);
				pthread_mutex_unlock(&user_mutex);
			}
			break;

		default:
			break;
	}
}
