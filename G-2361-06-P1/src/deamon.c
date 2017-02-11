void daemonizar (char * servicio) {

	pid_t pid;
	int tam=0;

	/* 1. Ceamos proceso hijo y terminamos el proceso padre */
	pid=fork();
	if (pid < 0) {
		exit (EXIT_FAILURE);
	}
	if (pid > 0) {
		exit (EXIT_SUCCESS);
	}

	/* 6. Abrir el log del sistema para su uso posterior */
	setlogmask (LOG_UPTO (LOG_INFO));
	openlog (servicio, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL3);

	/* 2. Crear una nueva sesión de tal forma que el proceso pase a ser lider de sesión */
	if (setsid() < 0) {
		syslog (LOG_ERR, "Error creando un SID para el hijo del proceso");
		
	}

	
	/* 3. Cambiar la máscara para que los ficheros sean accesibles a cualquiera (0) */
	unmask (0);

	/* 4. Establecer el directorio raíz / como directorio de trabajo */
	if((chdir("/")) < 0){
		syslog (LOG_INFO, "Error al cambiar el directorio de trabajo a la raíz");
		exit (EXIT_FAILURE);
	}

	/* 5. Cerrar todos los descriptores de fichero que pueda haber abiertos */
	syslog(LOG_INFO, "Cerrando los ficheros");
	
	close(STDIN_FILENO); 
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/* Mandar a dev/null STDIN/STDOUT/STDERR*/

	/* close(STDIN_FILENO);  ????*/
	syslog (LOG_INFO, "Initiating new server");
}

int ini_server(void){
	int sock_val;
	/* struct sockaddr? */

	syslog (LOG_INFO, "Creating socket");
	if((sock_val= socket(AF_INET, SOCK_STREAM,0)) < 0){

	}
}