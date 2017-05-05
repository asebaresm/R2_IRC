/**
 * @file logger.c
 * @brief Implementacion de la liberia de log de errores complementaria a /var/log/syslog.
 * Se ha implementado teniendo en cuenta al concurrencia entre hilos.
 * @author Alfonso Sebares
 * @date 13/02/17
 *
 */

#include "../includes/logger.h"

char glog_dir[LEN]; /**< Global con la ruta del .log para esta ejecucion */


char* snapTime(char* buf, int len){
	time_t curtime;
	struct tm *loc_time;

	//Getting current time of system
	curtime = time (NULL);
	// Converting current time to local time
	loc_time = localtime (&curtime);
	strftime (buf, len, "%H:%M:%S", loc_time);

	return buf;
}

char* snapClockTime(char* buf, int len){
	struct timespec snap;
	clock_gettime(CLOCK_MONOTONIC, &snap);
	sprintf(buf,"%d", (int)snap.tv_nsec);
	return buf;
}

//No es necesario hacerla thread-safe ya que no tiene sentido inicializar la carpeta
//de logs a nivel de hilo.
FILE* initLog(){
	char buf[LEN];
	char log_dir[LEN];
	struct stat st = {0};
	int ret = 0;
	FILE *fp = NULL;

	if (stat("logs", &st) == -1) {
		//ret = mkdir("logs", 0700);
		if ( (ret = mkdir("logs", ACCESSPERMS)) != OK ){
			perror("Error al crear el directorio de logs");
			return NULL;
		}
	}

	strcpy(log_dir, "logs/");
	strcat(log_dir, snapTime(buf, LEN));
	strcat(log_dir, ".log");

	if ((fp = fopen(log_dir, "w+")) == NULL){
		perror("Error al abrir/crear el log");
		return NULL;
	}
	
	strcpy(glog_dir, log_dir);

	if (fclose(fp) != 0){
		perror("ERR al cerrar log creado");
		return NULL;
	}

	return fp;
}

int logWrite(char* log_msg, char* type){
	FILE* fp = NULL;
	char buf[LEN];		//snap de tiempo
	char bbuf[BIGLEN];
	char buf_err[LEN];	//strerror

	if (strlen(log_msg) > BIGLEN){
		perror("Mensaje de log supera BIGLEN, abortado");
		return ERR;
	}

	strcpy(bbuf, "[");
	strcat(bbuf, snapTime(buf,LEN));
	strcat(bbuf, "] ");
	strcat(bbuf, "(");
	strcat(bbuf, snapClockTime(buf,LEN));
	strcat(bbuf, ") ");
	strcat(bbuf, type);
	strcat(bbuf, log_msg);

	//Ver si es tipo informativo o de error
	if (strcmp(type, "-(!)- ") == 0){
		strcat(bbuf, " : ");
		strerror_r(errno, buf_err, LEN);
		strcat(bbuf, buf_err);
	}

	pthread_mutex_lock(&loglock);
	if ((fp = fopen(glog_dir, "a")) == NULL){
		perror("Error al abrir log para escritura de evento");
		return ERR;
	}

	if (fprintf(fp, "%s\n", bbuf) < 0){
		perror("Error de escritura en el log");
		return ERR;
	}
	fclose(fp);
	pthread_mutex_unlock(&loglock);

	return OK;
}

int logEvent(char* log_msg){
	if (logWrite(log_msg, "- i - ") == ERR){
		return ERR;
	}
	return OK;
}

int logERR(char* log_msg){
	if (logWrite(log_msg, "-(!)- ") == ERR){
		return ERR;
	}
	return OK;
}