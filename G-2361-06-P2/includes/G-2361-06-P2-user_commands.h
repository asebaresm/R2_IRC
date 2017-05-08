/**
 * @file user_commands.h
 * @brief Declaraciones de funciones, definición de tipos: implementación de funciones IRC de usuario.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 20/03/17
 *
 */
#ifndef user_commands_h
#define user_commands_h

#include <redes2/ircxchat.h>
#include <redes2/irc.h>
#include <pthread.h>

#include "G-2361-06-P2-conexion_tcp.h"
#include "G-2361-06-P2-aux_functions.h"

typedef int (*p_funcion)(char* );	/**< definicion del tipo de puntero a array de funciones*/

extern int sockfd_user;				/**< global con el descriptor socket abierto con el servidor*/
extern char nick_user[MAXDATA];		/**< nick del usuario operador del cliente*/
extern pthread_t recv_tid;			/**< tid del hilo encargado de recibir datos del servidor*/

/**
 * @page punotice punotice
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int punotice(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: NOTICE
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int punotice(char* command);

/**
 * @page pucycle pucycle
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pucycle(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: CYCLE
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pucycle(char* command);

/**
 * @page pumotd pumotd
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pumotd(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: MOTD
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pumotd(char* command);

/**
 * @page pulusers pulusers
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pulusers(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: LUSERS
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pulusers(char* command);

/**
 * @page pumode pumode
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pumode(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: MODE
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pumode(char* command);

/**
 * @page pupartall pupartall
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pupartall(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: PARTALL
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pupartall(char* command);

/**
 * @page puback puback
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int puback(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: BACK
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int puback(char* command);

/**
 * @page puunaway puunaway
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int puunaway(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: (UN)AWAY
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int puunaway(char* command);

/**
 * @page puoper puoper
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int puoper(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: OPER
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int puoper(char* command);

/**
 * @page puban puban
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int puban(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: BAN
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int puban(char* command);

/**
 * @page pufsend pufsend
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pufsend(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: FSEND
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pufsend(char* command);

/**
 * @page pufaccept pufaccept
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pufaccept(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: FACCEPT
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pufaccept(char* command);

/**
 * @page pufclose pufclose
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pufclose(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: FCLOSE
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pufclose(char* command);

/**
 * @page putopic putopic
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int putopic(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: TOPIC
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int putopic(char* command);

/**
 * @page pukick pukick
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pukick(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: KICK
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pukick(char* command);

/**
 * @page puinvite puinvite
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int puinvite(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: INVITE
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int puinvite(char* command);

/**
 * @page puwhois puwhois
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int puwhois(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: WHOIS
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int puwhois(char* command);

/**
 * @page puaway puaway
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int puaway(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: AWAY
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int puaway(char* command);

/**
 * @page punick punick
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int punick(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: NICK
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int punick(char* command);

/**
 * @page puquit puquit
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int puquit(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: QUIT
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int puquit(char* command);

/**
 * @page puleave puleave
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int puleave(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: LEAVE
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int puleave(char* command);

/**
 * @page pupart pupart
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pupart(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: PART
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pupart(char* command);

/**
 * @page pujoin pujoin
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pujoin(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: JOIN
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pujoin(char* command);

/**
 * @page puwho puwho
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int puwho(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: WHO
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int puwho(char* command);

/**
 * @page punames punames
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int punames(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: NAMES
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int punames(char* command);

/**
 * @page pumsg pumsg
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pumsg(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: MSG
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pumsg(char* command);

/**
 * @page pulist pulist
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pulist(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: LIST
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pulist(char* command);

/**
 * @page puhelp puhelp
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int puhelp(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: HELP
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int puhelp(char* command);

/**
 * @page pdefault pdefault
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int pdefault(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: (DEFAULT)
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int pdefault(char* command);

/**
 * @page puquery puquery
 *
 * @synopsis
 * @code
 *	#include "user_commands.h"
 *
 *	int puquery(char* command);
 *	
 * @endcode
 * 
 * @brief Comando IRC de usuario: QUERY
 *
 * @parameters
 * @param[in] command: Cadena que contiene el comando de usuario correspondiente
 *
 * @return
 * int OK si se ha enviado correctamente el mensaje o ERR si se ha producido un error
 *
 * @author
 * Celia Mateos de Miguel (cel.mateos@estudiante.uam.es)
 * Beatriz de Pablo Garcia (beatriz.depablo@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int puquery(char* command);

#endif