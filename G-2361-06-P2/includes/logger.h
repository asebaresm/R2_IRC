/**
 * @file logger.c
 * @brief Descripcion de la liberia de log de errores complementaria a /var/log/syslog
 * 
 * @note La librería asume que el mutex se inicializa y destruye en el source principal.
 * @author Alfonso Sebares
 * @date 13/02/17
 */

#ifndef logger_h
#define logger_h

#include "types.h"

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#define LEN 64 		/**< tam de buffer para snaps de tiempo */
#define BIGLEN 512	/**< tam de buffer para mensajes grabados en .log */

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

char* snapTime(char* buf, int len);
char* snapClockTime(char* buf, int len);
FILE* initLog();
int logWrite(char* log_msg, char* type);
int logEvent(char* log_msg);
int logERR(char* log_msg);
#endif

