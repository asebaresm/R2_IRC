/**
 * @file aux_functions.c
 * @brief Implementacion de las funciones auxiliares de xchat2
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 13/02/17
 *
 */

#include "../includes/aux_functions.h"

/**
* @brief Libera punteros si estos no estan a NULL
* Uso
*
* mfree(3,a,b,c);
* mfree(4,a,b,c,d); 
*/
void mfree(int n, ...){

	va_list ap;
	char *p = NULL;
	register int i;
	va_start(ap, n);
	for (i = 0; i < n; ++i)
	{
		p = (char *) va_arg(ap, char*);
		if( p != NULL) free(p);
	}
	va_end (ap);
}

/**
 * @brief Establece la conexíon con el servidor de archivos para recibir los datos y escribirlos en un fichero
 * @param args estructura que contiene los parametros necesarios para establacer la conexion
 * @return OK si todo fue correcto y ERR si ocurrio un error
 */
int save_file(void* args){

	g_print("\n =========save_file========= \n");

	File_args* file_args = (File_args*) args;
	FILE* file = fopen(file_args->filename,"w");
	char message[MAXDATA];
	g_print("Guardamos el archivo\n");

	struct hostent *he;
    struct in_addr **addr_list;
    int i=0;
    char ip_addr[INET_ADDRSTRLEN]="";
    sleep(2);
    g_print("file_args->hostname: %s \nfile_args->port: %d \n",file_args->hostname, file_args->port); 

    int file_sockfd;

    if ((he = gethostbyname(file_args->hostname)) == NULL) {  // get the host info
        g_print("ERROR: IRCInterface_Connect - gethostbyname\n");
        return ERR;
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
    memset(&server_struct, '0', sizeof(server_struct)); 
    server_struct.sin_family = AF_INET;
    server_struct.sin_port = htons(file_args->port);
	//server_struct.sin_port = file_args->port;
    //memset(&(server_struct.sin_zero), '\0', 8);
    server_struct.sin_addr = **addr_list;

    //Socket
    file_sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (file_sockfd == -1){
        g_print("Error creando socket\n");
        return ERR;
    }
    //Connect
    g_print("Conectando con socket %d\n",file_sockfd);
    int cnct = connect(file_sockfd, (struct sockaddr*)&server_struct, sizeof(server_struct));
    	if (cnct==-1){
			switch(errno){
				case EACCES:
			    	printf("For UNIX domain sockets, which are identified by pathname: Write permission is denied on the socket file, or search permission is denied for one of the directories in the path prefix. (See also path_resolution(7).) \n");
					printf("or The user tried to connect to a broadcast address without having the socket broadcast flag enabled or the connection request failed because of a local firewall rule.\n" );
							 		
					break;
			 	
			 	case EPERM:
			    		printf("The user tried to connect to a broadcast address without having the socket broadcast flag enabled or the connection request failed because of a local firewall rule.\n"); 
					break;
				case EADDRINUSE:
			    	printf("Local address is already in use. \n");
					break;
				case EAFNOSUPPORT:
			    	printf("The passed address didn't have the correct address family in its sa_family field. \n");
					break;
				case EAGAIN:
				    printf("No more free local ports or insufficient entries in the routing cache. For AF_INET see the description of /proc/sys/net/ipv4/ip_local_port_range ip(7) for information on how to increase the number of local ports. \n");
					break;
				case EALREADY:
				    printf("The socket is nonblocking and a previous connection attempt has not yet been completed.\n"); 
					break;
				case EBADF:
				    printf("The file descriptor is not a valid index in the descriptor table.\n"); 
					break;
				case ECONNREFUSED:
				    printf("No-one listening on the remote address. \n");
					break;
				case EFAULT:
				    printf("The socket structure address is outside the user's address space. \n");
					break;
				case EINPROGRESS:
				    printf("The socket is nonblocking and the connection cannot be completed immediately. It is possible to select(2) or poll(2) for completion by selecting the socket for writing. After select(2) indicates writability, use getsockopt(2) to read the SO_ERROR option at level SOL_SOCKET to determine whether connect() completed successfully (SO_ERROR is zero) or unsuccessfully (SO_ERROR is one of the usual error codes listed here, explaining the reason for the failure). \n"); 
					break;
				case EINTR:
				    printf("The system call was interrupted by a signal that was caught; see signal(7).\n"); 
					break;
				case EISCONN:
				    printf("The socket is already connected.\n"); 
					break;
				case ENETUNREACH:
				    printf("Network is unreachable. \n");
					break;
				case ENOTSOCK:
				    printf("The file descriptor is not associated with a socket.\n"); 
					break;
				case ETIMEDOUT:
				    printf("Timeout while attempting connection. The server may be too busy to accept new connections. Note that for IP sockets the timeout may be very long when syncookies are enabled on the server.\n");
				    break;
			}
		return -1;
	}

	g_print("Campos recibidos: hostname=%s, filename=%s, port=%d, length=%ld\n",file_args->hostname, file_args->filename, file_args->port, file_args->length);
	
	recvDatos(file_sockfd, message, MAXDATA, he->h_name);
	g_print("Estoy recibiendo datos\n");
	fputs(message, file);
	//fwrite(message, 1, sizeof(message), file);
	g_print("Algo paso...\n");

	return OK;
}

/**
* @brief Actualiza la lista de nicks de la interfaz y sus estados
* @param channel canal que se quiere actualizar
* @param list lista de nicks obtenida del comando names, separados por espacios
* @return void
*/
void interface_mostrar_nicks(char* channel, char* list){
	g_print(BLU "\n aux call: interface_mostrar_nicks() \n" RESET);
	char* show_nick;
	//obtenemos los nicks del mensaje y los mostramos por separado

	//Eliminar los nicks del canal
	IRCInterface_NickListChannelThread(channel, NULL, NULL, NULL, NULL, NONE, 0);

	show_nick = strtok(list, " ");
	g_print("channel: %s \n",channel);
	g_print("show_nick: %s \n",show_nick);
	g_print("list: %s \n\n",list);
	if (show_nick[0] == '@')
	{
		IRCInterface_AddNickChannelThread (channel, ++show_nick, list, list, list, OPERATOR);
	}else if (show_nick[0] == '+'){
		IRCInterface_AddNickChannelThread (channel, ++show_nick, list, list, list, VOICE);
	} else{
		IRCInterface_AddNickChannelThread (channel, show_nick, list, list, list, NONE);
	}
	
	while ((show_nick = strtok(NULL, " ")) != NULL){
		if (show_nick[0] == '@')
		{
			IRCInterface_AddNickChannelThread (channel, ++show_nick, list, list, list, OPERATOR);
		}else if (show_nick[0] == '+'){
			IRCInterface_AddNickChannelThread (channel, ++show_nick, list, list, list, VOICE);
		} else{
			IRCInterface_AddNickChannelThread (channel, show_nick, list, list, list, NONE);
		}
	}
}

/**
* @brief Devuelve una cadena que empieza inmediatamente después de la cadena 'haystack' tras la primera aparición de 'ch'
* @param haystack string original donde hacer la busqueda
* @param ch delimitador
* @return char* substring con la cadena generada, NULL si no se ha encontrado 'ch'
*/
char *strnext(char* haystack, int ch){
	int i, o_length;
	char *sep_at = strchr(haystack, ch);
	
	if(sep_at != NULL){
		o_length = strlen(sep_at);
		for(i=0; i<strlen(sep_at); i++)
			sep_at[i] = sep_at[i+1];
		sep_at[o_length-1] = '\0';
	}

	return sep_at;
}

/**
* @brief Devuelve el tipo de comando (código 3 digitos) de un mensaje no reconocido por IRC_CommandQuery()
* @param message mensaje original
* @return int codigo de comando si es un codigo valido, ERR si no lo es o comando invalido
*/
int parse_type(const char* message){
	int unknw_type;
	char* token = NULL;
	char* message_cp = NULL;

	if(message[0] == ':'){ //mensaje con prefijo, no originado por el cliente
		message_cp = strdup(message);
		token = strtok(message_cp, " ");
		if(token != NULL){
			token = strtok(NULL, " ");
			free(message_cp);
			unknw_type = atoi(token);
			if (unknw_type < MAX_IRC_COMMAND){
				return unknw_type;
			}
		}
	}

	return ERR;
}

/**
* @brief Versión 2 de pruebas. Devuelve el tipo de comando (código 3 digitos) de un mensaje no reconocido por IRC_CommandQuery()
* @param message mensaje original
* @return int codigo de comando si es un codigo valido, ERR si no lo es o comando invalido
*/
int parse_type2(const char* message){
	int unknw_type;
	char* token = NULL;
	char* message_cp = NULL;

	message_cp = strdup(message);
	token = strtok(message_cp, " ");
	if(token != NULL){
		token = strtok(NULL, " ");
		free(message_cp);
		unknw_type = atoi(token);
		if (unknw_type < MAX_IRC_COMMAND){
			return unknw_type;
		}
	}
	
	return ERR;
}

void IRCInterface_WriteSystem_Pretty(char *nick, char *msg){
	char snap[SNAP_SIZE];
	char f_nick[MAX_NICK_FIELD];

	if(strlen(nick) > 9){
		logERR("En IRCInterface_WriteSystemThread_Pretty: strlen(nick) > 9");
		return;
	}

	strcpy(f_nick, "[");
	strcat(f_nick, snapTime(snap,SNAP_SIZE));
	strcat(f_nick, "] ");
	strcat(f_nick, "              *");

	//g_print(MAG"\n>>>>%s\n" RESET, f_nick);
	if(msg[strlen(msg) - 2] == 13) //check si es comienzo de CR,LF
		msg[strlen(msg) - 2] = '\0';

	IRCInterface_WriteSystem(f_nick,msg);
}

void IRCInterface_WriteSystemThread_Pretty(char *nick, char *msg){
	char snap[SNAP_SIZE];
	char f_nick[MAX_NICK_FIELD];

	if(strlen(nick) > 9){
		logERR("En IRCInterface_WriteSystemThread_Pretty: strlen(nick) > 9");
		return;
	}

	strcpy(f_nick, "[");
	strcat(f_nick, snapTime(snap,SNAP_SIZE));
	strcat(f_nick, "] ");
	strcat(f_nick, "              *");

	//g_print(MAG"\n>>>>%s\n" RESET, f_nick);
	if(msg[strlen(msg) - 2] == 13) //check si es comienzo de CR,LF
		msg[strlen(msg) - 2] = '\0';

	IRCInterface_WriteSystemThread(f_nick,msg);
}

void IRCInterface_WriteChannelThread_Pretty(char *chan, char *nick, char *msg){
	char snap[SNAP_SIZE];
	char f_nick[MAX_NICK_FIELD];

	if(strlen(nick) > 9){
		logERR("En IRCInterface_WriteSystemThread_Pretty: strlen(nick) > 9");
		return;
	}

	strcpy(f_nick, "[");
	strcat(f_nick, snapTime(snap,SNAP_SIZE));
	strcat(f_nick, "]");
	strcat(f_nick, nick);

	IRCInterface_WriteChannelThread(chan, f_nick, msg);	
}

int testIRC_CommandQuery(char* message){
	switch(IRC_CommandQuery(message)){
		case IRCERR_NOCOMMAND:
			return ERR;
		case IRCERR_UNKNOWNCOMMAND:
			return ERR;
		default:
			return OK;
	}
}

int changeMode(char *channel, char *nick, char *mode){
	int ret = -1;
	char *command = NULL;

	//long IRCMsg_Mode (char **command, char *prefix, char * channeloruser, char *mode, char *user)
	ret = IRCMsg_Mode (&command, NULL, channel, mode, nick);
	if(ret != IRC_OK){
		g_print(RED "ERROR - In changeMode: Error en IRCMsg_Mode, no devolvió IRC_OK\n" RESET);
		return ERR;
	}
	ret = enviarDatos(sockfd_user, command, strlen(command));
	if(ret == ERR){
		g_print(RED "ERROR - In changeMode: Error en enviarDatos, devolvió ERR\n" RESET);
		return ERR;
	}
	IRCInterface_PlaneRegisterOutMessage(command);
	free(command);

	return OK;
}
