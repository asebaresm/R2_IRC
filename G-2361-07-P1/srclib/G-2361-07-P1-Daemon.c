
#include "../includes/G-2361-07-P1-Daemon.h"

int daemonizar (char * servicio ){
	pid_t pid;
	int i;
	int tamanyo = 0;
	
	/*En caso de no recibir puntero sin servicio*/
	if (servicio == NULL)
		return -1;
	
	/*1.Crear un proceso hijo y terminar el proceso padre*/
	pid = fork();

	if( pid < 0 ) 
		exit(-1);

	if( pid > 0 ) 
		exit(0);

	/*3.Cambiar la máscara de modo de ficheros para que sean accesibles a cualquiera*/
	umask(0);
	setlogmask(LOG_UPTO(LOG_INFO));
	
	/*6.Abrir el log del sistema para su uso posterior para reportar errores y ver si nuestro demonio funciona correctamente se debe generar un fichero de tipo .log
	*/ 
	openlog (servicio, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_DAEMON);
	syslog (LOG_ERR, "Initiating new server.");
	
	/*2.Crear una nueva sesión de tal forma que el proceso pase a ser el líder de sesión*/
	if(setsid() < 0){
		syslog (LOG_ERR,"Error al crear sesion para el proceso hijo.");
		exit(-1);
	}

	/*4.Establecer el directorio raíz / como directorio de trabajo*/
	if((chdir("/")) != 0){
		syslog(LOG_ERR,"Error al cambiar el directorio de trabajo = \"/\"");
		exit(-1);
	}

	/*5.Cerrar todos los descriptores de fichero que pueda haber abiertos incluidos stdin,stdout,stderr*/
	syslog(LOG_INFO,"Cerrando descriptores");
	tamanyo = getdtablesize();
	for(i = 0;i < tamanyo; i++){
		close(i);
	}

	openlog (servicio, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_DAEMON);

	return 0;
}
