/** 
 * @brief Contiene las funcion para poder demonizar un servidor
 * 
 * @file daemon.c
 * @author Alfonso Sebares y Beatriz de Pablo
 * 
 */
#include "../includes/daemon.h"

/** 
 * @brief Funcion para demonizar a un servicio
 * 
 * @param char * servicio: Nombre del servidor para dejarlo en segundo plano
 *  
 *
 * @return OK si todo ha salido bien, ERROR si hay algun fallo
 *
 */
int daemonizar (char * servicio) {

	pid_t pid;
	pid_t sid;

	if (servicio==NULL){
		syslog(LOG_ERR, "Escriba un servicio no nulo.");
		return ERROR;
	}

	/* 1. Ceamos proceso hijo y terminamos el proceso padre */
	pid=fork();

	if (pid < 0) {
		syslog(LOG_ERR, "Error al crear proceso hijo");
		return ERROR;
	}
	if (pid > 0) {
		syslog(LOG_INFO, "Liberando al padre");
		return OK;
	}

	
	/* 2. Crear una nueva sesión de tal forma que el proceso pase a ser lider de sesión, y no sea un zombie. */
	sid=setsid();
	if (sid < 0) {
		syslog (LOG_ERR, "Error creando un SID para el hijo del proceso");
		return ERROR;
	}

	
	/* 3. Cambiar la máscara para que los ficheros sean accesibles a cualquiera (0) */
	umask (0);

	/* 4. Establecer el directorio raíz / como directorio de trabajo */
	if((chdir("/")) < 0){
		syslog (LOG_ERR, "Error al cambiar el directorio de trabajo a la raíz");
		return ERROR;
	}

	/* 5. Cerrar todos los descriptores de fichero que pueda haber abiertos ya que no se puede usar la terminal*/
	syslog(LOG_INFO, "Cerrando descriptores");

	close(STDIN_FILENO); 
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	syslog(LOG_INFO, "Mandando descriptores a dev/null..");
	
	freopen("/dev/null", "r", stdin);
	freopen("/dev/null", "w", stdout);
	freopen("/dev/null", "w", stderr);

	/* 6. Abrir el log del sistema para su uso posterior (para que haya comunicacion con el demonio) */
	openlog (servicio, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL3);

	syslog (LOG_INFO, "Iniciado nuevo servidor");

	return OK;
}
