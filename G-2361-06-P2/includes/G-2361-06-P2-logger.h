/**
 * @file logger.c
 * @brief Descripcion de la liberia de log de errores complementaria a /var/log/syslog
 * @note La librería asume que el mutex se inicializa y destruye en el source principal.
 * @author Alfonso Sebares
 * @date 13/02/17
 */

#ifndef logger_h
#define logger_h

#include "G-2361-06-P2-types.h"

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#define LEN 64 		/**< tam de buffer para snaps de tiempo */
#define BIGLEN 1536	/**< tam de buffer para mensajes grabados en .log (1024 + 512, orientativo)*/

/**
*  @brief Declaracion del Mutex para el descriptor de fichero del log.
*  Siempre tiene que ser definida en el source princpal.
*/
extern pthread_mutex_t loglock; 

//No se usa de momento
struct val{
	int int_val;
	long llong_val;
	char* str;
};

typedef struct val val_struct;

/**
 * @page snapTime snapTime
 *
 * @synopsis
 * @code
 *	#include "types.h"
 *
 *	char* snapTime(char* buf, int len);
 *	
 * @endcode
 * 
 * @brief Devuelve una cadena con una snapshot de tiempo en formato %H:%M:%S
 *
 * @parameters
 * @param[in] buf: buffer donde escrbir la snap
 * @param[in] len: tamaño del buffer

 * @return
 * char* buffer con la snap de tiempo grabada
 *
 * @author
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
char* snapTime(char* buf, int len);

/**
 * @page snapClockTime snapClockTime
 *
 * @synopsis
 * @code
 *	#include "aux_functions.h"
 *
 *	char* snapClockTime(char* buf, int len);
 *	
 * @endcode
 * 
  * @brief Devuelve una cadena con una snapshot del tiempo actual
 *
 * @parameters
 * @param[in] buf: buffer donde escrbir la snap
 * @param[in] len: tamaño del buffer

 * @return
 * char* buffer con la snap de tiempo grabada
 *
 * @author
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
char* snapClockTime(char* buf, int len);

/**
 * @page initLog initLog
 *
 * @synopsis
 * @code
 *	#include "aux_functions.h"
 *
 *	FILE* initLog();
 *	
 * @endcode
 * 
 * @brief Inicializa la carpeta para logs si no existe y un fichero de log para eventos/errores.
 *
 * @parameters
 * Ninguno

 * @return
 * FILE* descriptor al log sobre el que escribir eventos/errores
 *
 * @author
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
FILE* initLog();

/**
 * @page logWrite logWrite
 *
 * @synopsis
 * @code
 *	#include "types.h"
 *
 *	int logWrite(char* log_msg, char* type);
 *	
 * @endcode
 * 
 * @brief (Primitiva) Escribe un evento/error en el log de la ejecucion actual
 *
 * @parameters
 * @param[in] log_msg: Mensaje a grabar en el log
 * @param[in] type: Tipo de evento (error o evento)
 *
 * @return
 * int OK o ERR
 * @author
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int logWrite(char* log_msg, char* type);

/**
 * @page logEvent logEvent
 *
 * @synopsis
 * @code
 *	#include "types.h"
 *
 *	int logEvent(char* log_msg);
 *	
 * @endcode
 *
 * @brief Escribe un evento en el log de la ejecucion actual
 *
 * @parameters
 * @param[in] log_msg: Evento a grabar en el log
 *
 * @return
 * int OK o ERR
 * @author
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int logEvent(char* log_msg);

/**
 * @page logERR logERR
 *
 * @synopsis
 * @code
 *	#include "types.h"
 *
 *	int logERR(char* log_msg);
 *	
 * @endcode
 * 
 * @brief Escribe un error en el log de la ejecucion actual
 *
 * @parameters
 * @param[in] log_msg: Error a grabar en el log
 *
 * @return
 * int OK o ERR
 * @author
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 *
 *
 * @date 13 de febrero de 2017
 *
 *<hr>
*/
int logERR(char* log_msg);
#endif

