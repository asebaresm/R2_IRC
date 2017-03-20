/**
 * @file servidor.c
 * @brief Implementacion del servidor IRC.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @date 13/02/17
 *
 */

 /* -- Includes -- */
#include "../includes/servidor.h"

/**
* @brief Definicion del Mutex para el descriptor de fichero del log
*/
pthread_mutex_t loglock;

/**
* @brief Definicion del Mutex para el descriptor de fichero del log
*/
pthread_mutex_t usrslock;

/** 
 * @brief Version 'thread-safe' de logERR
 * 
 * @param pthread_mutex_t *mutex: lock de control de acceso al fichero de log
 * @param cchar* log_msg: mensaje de error
 *  
 *
 * @return OK si todo ha salido bien, ERROR si hay algun fallo
 *
 */
/*
void logERR_r(char* log_msg){
	pthread_mutex_lock(&lock);
	logERR(log_msg);
	pthread_mutex_unlock(&lock);
}
*/

/** 
 * @brief Version 'thread-safe' de logEvent_r
 * 
 * @param pthread_mutex_t *mutex: lock de control de acceso al fichero de log
 * @param cchar* log_msg: mensaje de evento
 *  
 *
 * @return OK si todo ha salido bien, ERROR si hay algun fallo
 *
 */
/*
void logEvent_r(char* log_msg){
	pthread_mutex_lock(&lock);
	logEvent(log_msg);
	pthread_mutex_unlock(&lock);
}
*/


int main (int argc, char * argv[]){

	FILE* log_fp = NULL;
	int sockaddr_in_size;
	//int read_size;
	int ssocket_desc = NULL;					/*servidor fd*/
	int csocket_desc = NULL;					/*cliente  fd*/
	struct sockaddr_in server, client;
	struct hostent *hostp;						/* client host info */
	char *hostaddrp;							/* dotted decimal host addr string */
	char buf_event[AUX_SBUF];

	pthread_t tid;
	//pthread_attr_t attr;

	Users usrs;

	if (argc != 1) {
		logERR("numero de argumentos invalido (puerto definido en servidor.h)");
		return EXIT_FAILURE;
	}

	/*
	if(daemonizar (argv[1]) == ERROR){
		syslog(LOG_ERR, "Error al daemonizar");
		exit(EXIT_FAILURE);
		return ERROR;
	}*/

	log_fp = initLog();

	if (log_fp == NULL){
		perror("error en initLog()");
		return EXIT_FAILURE;
	}

	ssocket_desc = crearConexion(PORT,&server);
	if (ssocket_desc == ERR){
		logERR("crearConexion() devolvio ERR");
		return EXIT_FAILURE;
	}

	if (pthread_mutex_init(&loglock, NULL) != OK){
		logERR("\n mutex init ha devuelto error\n");
		return EXIT_FAILURE;
	}

	if (pthread_mutex_init(&usrslock, NULL) != OK){
		logERR("\n mutex init ha devuelto error\n");
		return EXIT_FAILURE;
	}

	usrs.num = 0; //ningun usuario al principio
	sockaddr_in_size = sizeof(struct sockaddr_in);
	
	while(1){
		csocket_desc = accept(ssocket_desc, (struct sockaddr *)&client, (socklen_t*)&sockaddr_in_size);

		if (csocket_desc == ERR){
			logERR("accept() devolvio ERR");
			return EXIT_FAILURE;
		}

		hostp = gethostbyaddr((const char *)&client.sin_addr.s_addr, sizeof(client.sin_addr.s_addr), AF_INET);
		hostaddrp = inet_ntoa(client.sin_addr);

		if (hostaddrp == NULL || hostp == NULL)
			logERR("gethostbyaddr()/inet_ntoa() devolvio NULL");
		else{
			sprintf(buf_event, "Conexion establecida con %s (%s)\n", hostp->h_name, hostaddrp);
			logEvent(buf_event);

			strcpy(usrs.users[usrs.num].hostname, hostp->h_name);
			strcpy(usrs.users[usrs.num].ip, hostaddrp);
		}

		usrs.users[usrs.num].sockfd = csocket_desc;
		usrs.num++;

		if(pthread_create(&tid , NULL, (void *)procesar_conexion, (void*) &usrs) != OK){
			logERR("pthread_create() devolvio error");
			return EXIT_FAILURE;
		}

		if (pthread_detach(tid) != OK){
			 logERR("pthread_detach() devolvio error");
			 return EXIT_FAILURE;
		}
		
	}

	pthread_mutex_destroy(&usrslock);
	pthread_mutex_destroy(&loglock);

	return EXIT_SUCCESS;
}

void remove_entry(Users* usrs, char* nick){
	int i;

	for(i=0; i<usrs->num; i++){
		if(strcmp(usrs->users[i].nick, nick) == 0){

			pthread_mutex_lock(&usrslock);
			for(; i<usrs->num-1; i++){                     
				strcpy(usrs->users[i].nick, usrs->users[i+1].nick);
				strcpy(usrs->users[i].ip, usrs->users[i+1].ip);
				strcpy(usrs->users[i].hostname, usrs->users[i+1].hostname);
				usrs->users[i].sockfd = usrs->users[i+1].sockfd;
			}
			usrs->num--;
			pthread_mutex_unlock(&usrslock);

			break;
		}
	}
}

int procesar_conexion (void * ptr){
	thdata data;
	char message[MSG_LEN] = "";

	Users* usrs = (Users*) ptr;

	data.new_fd = usrs->users[usrs->num-1].sockfd;
	data.nick_sent = 0;
	data.user_sent = 0;
	data.passw_sent = 0;
	data.prefix = NULL;
	data.passw = NULL;
	data.nick = NULL;
	data.nick2 = NULL;
	data.user = NULL;
	data.modehost = NULL;
	data.serverother = NULL;
	data.realname = NULL;
	data.name = NULL;
	data.quit_message = NULL;
	data.channel = NULL;
	data.key = NULL;
	data.join_msg = NULL;
	data.topic = NULL;
	data.target = NULL;
	data.maskarray = NULL;
	data.msgtarget = NULL;
	data.server = NULL;
	data.server2 = NULL;

	strcpy(data.ip, usrs->users[usrs->num-1].ip);
	strcpy(data.hostname, usrs->users[usrs->num-1].hostname);

	while(1){
		//Caso: recibe 0 datos
		if (recvDatos(data.new_fd, message, MSG_LEN, data.hostname) == 0){
			//IRCTADUser_DeleteByUser(data.user);
			logEvent("EN procesar_conexion(): recvDatos() devolvio 0 bytes, cerrando conexion.");
			remove_entry(usrs, data.nick);
			break;
		}/*fin caso: leído 0 bytes*/
		if (procesar_mensaje(data.new_fd, message, &data, usrs) == ERR){
			logERR("EN procesar conexion: procesar_mensaje() devolvio error"); //(!)
			//remove_entry(usrs, data.nick);
			break;
		}
	} /*fin while(1)*/
	
	if (cerrarConexion(data.new_fd, data.hostname) == ERR){ 
		return ERR; 
	}

	pthread_exit(0);
	return ERR;
}

int procesar_mensaje (int new_fd, char *message, thdata* data, Users* usrs){

    char* token = NULL;
    char* command = NULL;

    token = IRC_UnPipelineCommands(message, &command);

    while (token != NULL){

        /*comprobar si el primero comando leído es válido*/        
        if(command_query(command, new_fd, data, usrs) == ERR){
            logERR("EN procesar_mensaje(): command_query() devolvio ERR, comando mal formado/no válido.");
            return ERR;
        }        
        token = IRC_UnPipelineCommands(message, &command);
    }

    free(command);
    return OK;
}

int command_query(char *message, int new_fd, thdata* data, Users* usrs){

	return OK;
}