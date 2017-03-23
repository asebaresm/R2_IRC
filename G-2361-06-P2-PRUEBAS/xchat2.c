#include <redes2/ircxchat.h>
#include <redes2/irc.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include "includes/types.h"
#include "includes/conexion_tcp.h"
#include "includes/logger.h"

#define MAXDATA 1024

pthread_mutex_t loglock;

/**
* Globales útiles sobre la conexión del cliente con el servidor
*/
int sockfd_user = -1;
char nick_user[MAXDATA] = {0};
char* hostname;
char* active_channel = NULL;
char* stream;

char host_name[128]; //aux, uso en libreria TCP

//Funciones auxiliares
//void interface_mostrar_nicks(char* channel, char* list);
int command_query(char *message);
void unpipe(char* message);
int receive_messages(void* no_arg);

int clienteRecibirDatosTCP(int sockfd, char* message, int length){

	int numbytes = 0;
	char buf[AUX_SBUF];
    //printf("Cliente recibe datos - TCP\n");

    /*Esperamos respuesta del servidor*/
    if ((numbytes=recv(sockfd, message, length-1, 0 /*flags*/)) == -1) {
        perror("Error el recibir mensaje TCP cliente \n");
        return ERR;
    }
    message[numbytes] = '\0';

    sprintf(buf, "clienteRecibirDatosTCP(): \"%s\" (%d Bytes)", message, numbytes);
		logEvent(buf);
    return OK;    

}

int clienteEnviarDatosTCP(int sockfd, char* message, int length){
    
    //printf("Cliente envia datos - TCP\n");   
	char buf[AUX_SBUF];
    /*Enviamos el mensaje*/
    if (send(sockfd, (const void *)message, length, 0 /*flags*/) == -1){
        perror("Error al enviar el mensaje TCP cliente \n");
        return ERR;
    }
    sprintf(buf, "clienteEnviarDatosTCP(): \"%s\" (%d Bytes)", message, length);
		logEvent(buf);
    return OK;

}

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

	/*
	if(message == NULL) {
		g_print(RED "ERROR - In command_query: message == NULL al principio\n\n" RESET);
		return ERR;
	}
	*/

	IRCInterface_PlaneRegisterInMessage(message);

	switch(IRC_CommandQuery(message)){
		case NOTICE:
			//g_print("\n=======CASE NOTICE=======\n");
			g_print(GRN "\nComando NOTICE - message = %s\n" RESET, message);

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

		/*TRATAMIENTO DE ERRORES*/
		case IRCERR_NOCOMMAND:
			g_print(RED "ERROR - In command_query: Mensaje de error recibido en switch():\n\tIRCERR_NOCOMMAND -  no hay ningún comando en la cadena de caracteres\n" RESET);
			IRCInterface_WriteSystemThread("*",message);
			return ERR;

		case IRCERR_NOPARAMS:
			g_print(RED "ERROR - In command_query: Mensaje de error recibido en switch():\n\tIRCERR_NOPARAMS -  se ha introducido una cadena de caracteres nula\n" RESET);
			IRCInterface_WriteSystemThread("*",message);
			return ERR;

		case IRCERR_UNKNOWNCOMMAND:
			g_print(YEL "WARN - In command_query: Mensaje de error recibido en switch():\n\tIRCERR_UNKNOWNCOMMAND -  l comando que contiene no es reconocido por esta función.\n" RESET);
			IRCInterface_WriteSystemThread("*",message);
			return OK;

		default:
			g_print(YEL "WARN - In command_query: Alcanzado default del switch()(TO_DO):\n\t %s\n" RESET, message);
			IRCInterface_WriteSystemThread("*",message);
			return OK;
	}

	//Liberamos memoria
	//mfree(33,msg,nick,nick2,servername,versionname,timedate,version,availableusermodes,availablechannelmodes,addedg,type,visible,topic,channel,key,
	//msgtarget,origin_nick,filename,hostname_destino,nick_part,username_part,host_part,server_part,server,server2,command_pong,token,modechar,channeluser,
	//mode,user,comment,realname,host);
	return OK;

}

/**
* @brief Funcion para dividir en comandos la cadena "message"
* @param message cadena recibida, puede incluir mas de un comando
* @return void
*/
void unpipe(char* message){

	char *q = message, *command;

	while (q != NULL){
		//printf(BLU "\nwhile en unpipe\n" RESET);
		q = IRC_UnPipelineCommands(q, &command);
		if (command_query(command) == ERR){
			g_print(RED "ERROR - In unpipe: command_query(command) devolvio ERR\n" RESET);
			free(command);
			break;
		}
		free(command);
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
int receive_messages(void* no_arg){

	char message[MAXDATA];
	int ret;
	g_print(GRN "Hilo Preparado para recibir mensajes\n" RESET);

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
		if(ret == 0){
			g_print(RED "ERROR - In receive_messages: recvDatos() leyó 0 Bytes(ver .log)\n\t\tEl cliente se cerrará.\n" RESET);
			exit(1);
		}
		unpipe(message);
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
	g_print(YEL "\nIRCInterface_ActivateChannelKey(char *channel, char *key) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_ActivateExternalMessages(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_ActivateInvite(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_ActivateModerated(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_ActivateNicksLimit(char *channel, int limit) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_ActivatePrivate(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_ActivateProtectTopic(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_ActivateSecret(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_BanNick(char *channel, char *nick) call\n" RESET);
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
	long ret = -1;
	int retorno = -1;
	char *command = NULL;
	char *prefix = NULL;
	int sockfd = -1;

	struct hostent *he;
    struct in_addr **addr_list;
    int i=0;
    char ip_addr[INET_ADDRSTRLEN]={0};

    char *msgNick=NULL;
    char mode[MAXDATA]={0};

	//thread
	pthread_t tid;

	g_print("IRCInterface_Connect - Datos introducidos por el usuario \n");
	g_print("\t nick: %s \n",nick);
	g_print("\t user: %s \n",user);
	g_print("\t realname: %s \n",realname);
	g_print("\t password: %s \n",password);
	g_print("\t server: %s \n",server);
	g_print("\t port: %d \n",port);
	g_print("\t ssl: %d \n",ssl);

    if ((he = gethostbyname(server)) == NULL) {  // get the host info
        g_print("ERROR: IRCInterface_Connect - gethostbyname\n");
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
        g_print("Error creando socket\n");
        return IRCERR_NOCONNECT;
    }

    memset(&server_struct, '0', sizeof(server_struct)); 
    server_struct.sin_family = AF_INET;
    server_struct.sin_port = htons(port); 
    memset(&(server_struct.sin_zero), '\0', 8);
    server_struct.sin_addr = **addr_list; 

    /*Connect*/
    if (connect(sockfd, (struct sockaddr*)&server_struct, sizeof(server_struct)) < 0){
        g_print("Error en connect\n");
        return IRCERR_NOCONNECT;
    }

	//Conectarse al socket
	//sockfd = clienteConexionTCP(ip_addr,port);
	if(sockfd == -1){
		g_print("ERROR: IRCInterface_Connect - clienteConexionTCP\n");
	}else{
		g_print("Conexión establecida con host: %s y puerto: %d \n",server,port);
	}

	//Copiar los parametros a las variables globales
    strcpy(nick_user,nick);
    sockfd_user=sockfd;

	if(ssl == FALSE){

		g_print("Lanzamos el hilo que recibe mensajes\n");
		if(pthread_create( &tid, NULL, (void*) receive_messages, (void*) "no_arg") < 0){
	    	g_print("Error en receive receive_messages\n");
			return ERR;
		}

		//Enviar pass si existe
		if((password != NULL) && (strlen(password) > 0)){ //Comprobar si hay password
			//Enviar mensaje al servidor con la pass
			ret = IRCMsg_Pass (&command, prefix, password);
			if(ret != IRC_OK){
				g_print("ERROR: IRCInterface_Connect - IRCMsg_Pass\n");
				return IRCERR_NOCONNECT;
			}
			g_print("IRCMsg_Pass: %s",command);	
			retorno = enviarDatos(sockfd,command, strlen(command)+1);
			if(retorno == ERR){
				g_print("ERROR: IRCInterface_Connect - enviarDatos - Pass\n");
			}
			IRCInterface_PlaneRegisterOutMessage(command);
			free(command);		

		}

		//Enviar mensaje con el nick
		command=NULL;
		ret = IRCMsg_Nick (&command, prefix, nick, msgNick);
		
		if(ret != IRC_OK){
			g_print("ERROR: IRCInterface_Connect - IRCMsg_Nick\n");
			return IRCERR_NOCONNECT;
		}
		g_print("IRCMsg_Nick: %s",command);
		retorno = enviarDatos(sockfd,command, strlen(command));
		if(retorno == ERR){
			g_print("ERROR: IRCInterface_Connect - enviarDatos - Nick\n");
		}
		IRCInterface_PlaneRegisterOutMessage(command);
		free(command);
		
		//mode
		//Enviar mensaje con el user
		strcpy(mode,"0");
		command=NULL;
		ret = IRCMsg_User (&command, prefix, user, mode , realname);
		
		if(ret != IRC_OK){
			g_print("ERROR: IRCInterface_Connect - IRCMsg_User\n");
			return IRCERR_NOCONNECT;
		}
		g_print("IRCMsg_User: %s",command);	
		retorno = enviarDatos(sockfd,command, strlen(command));
		if(retorno == ERR){
			g_print("ERROR: IRCInterface_Connect - enviarDatos - Nick\n");
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
	g_print(YEL "\nIRCInterface_DeactivateChannelKey(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_DeactivateExternalMessages(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_DeactivateInvite(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_DeactivateModerated(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_DeactivateNicksLimit(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_DeactivatePrivate(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_DeactivateProtectTopic(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_DeactivateSecret(char *channel) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_DisconnectServer(char *server, int port) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_ExitAudioChat(char *nick) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_GiveOp(char *channel, char *nick) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_GiveVoice(char *channel, char *nick) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_KickNick(char *channel, char *nick) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_NewCommandText(char *command) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_NewTopicEnter(char *topicdata) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_SendFile(char *filename, char *nick, char *data, long unsigned int length) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_StartAudioChat(char *nick) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_StopAudioChat(char *nick) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_TakeOp(char *channel, char *nick) call\n" RESET);
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
	g_print(YEL "\nIRCInterface_TakeVoice(char *channel, char *nick) call\n" RESET);
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
