/**
 * @file logger.c
 * @brief Descripcion de la liberia de log de errores complementaria a /var/log/syslog
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @date 13/02/17
 *
 */

#ifndef logger_h
#define logger_h

#include "types.h"

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define LEN 64 		/**< tam de buffer para snaps de tiempo */
#define BIGLEN 256	/**< tam de buffer para mensajes grabados en .log */

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

