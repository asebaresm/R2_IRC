/**
 * @file funciones_SSL.c
 * @brief Implementacion de la seguridad SSL
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 25/04/17
 *
 */

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/x509v3.h>

#include "../includes/G-2361-06-P3-funciones_ssl.h"

#define BUFSIZE 2048   /**< Tam. max. del buffer que se lee*/


void inicializar_nivel_SSL(){

	/* Capturamos los errores que puede dar de SSL*/
	SSL_load_error_strings();
	/* Iniciamos SSL*/
	SSL_library_init();

}


SSL_CTX* fijar_contexto_SSL(char* cert, char* key){

	SSL_CTX *ctx;

	/*crea un nuevo contexto para la utilización de
la capa SSL y lo inicializa*/
	ctx=SSL_CTX_new(SSLv23_method());
	if (ctx==NULL){
		return NULL;
	}
	
	/*validar los certificados recibidos por la aplicación*/
	if (!SSL_CTX_load_verify_locations(ctx,CERT_FILE,CERT_PATH)){
		return 0;
	}

	/*utilizar
los certificados de las entidades CA bien conocidas*/
	SSL_CTX_set_default_verify_paths(ctx);

	/*especificar el certificado que utilizará nuestra aplicación*/
	if(!SSL_CTX_use_certificate_chain_file(ctx, cert)){
		ERR_print_errors_fp(stdout);
		return 0;
	}
	
	/*especificar la clave privada de nuestra
aplicación*/
	SSL_CTX_use_PrivateKey_file(ctx, key, SSL_FILETYPE_PEM);

	/*fijará el modo de funcionamiento de nuestra
aplicación*/
	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
	
	return ctx;
}


SSL* conectar_canal_seguro_SSL(SSL_CTX* ctx,int sockfd){
	
	SSL * ssl=NULL;

	/*puntero a la estructura creada será guardado para añadir seguridad al canal*/
	ssl=SSL_new(ctx);

	if(ssl==NULL){
		return NULL;
	}

	/*asociar la estructura SSL creada el canal de comunicación*/
	if(!SSL_set_fd(ssl, sockfd)){
		return NULL;
	}


	/* inicializará el handshake con el servidor*/
	if(!SSL_connect(ssl)){
		return NULL;
	}

	return ssl;
}


int evaluar_post_connectar_SSL(SSL * ssl){

	/* comprobar que de hecho se ha recibido un certificado*/
	if(SSL_get_peer_certificate(ssl)==NULL){
		return 0;
	}

	/* validar el certificado con algún CA*/
	return SSL_get_verify_result(ssl);
}


int enviar_datos_SSL(SSL *ssl, char *buffer){

	if(!buffer || sizeof(buffer) < 0){
		return -1;
	}

	/* envia datos */
	return SSL_write(ssl, buffer, BUFSIZE);
}



SSL* aceptar_canal_seguro_SSL(SSL_CTX* ctx,int sockfd){
	SSL * ssl=NULL;
	
	/*puntero a la estructura creada será guardado para añadir seguridad al canal*/
	ssl=SSL_new(ctx);
	
	if(ssl==NULL || ctx==NULL){
		return NULL;
	}


	/*asociar la estructura SSL creada el canal de comunicación*/
	if(!SSL_set_fd(ssl, sockfd)){
		return NULL;
	}

	

	/* función bloqueará al servidor y lo pondrá en espera hasta recibir el hand-
shake por parte del cliente*/
	if(!SSL_accept(ssl)){
		return NULL;
	}
	
	return ssl;
}


int recibir_datos_SSL(SSL *ssl, char * buffer){

	if(!ssl || sizeof(buffer) < 0){
		return -1;
	}

	/* recibe datos */
	return SSL_read(ssl, buffer, BUFSIZE);
}


void cerrar_canal_SSL(SSL **ssl,SSL_CTX **ctx, int sockfd){
	
	/*cierra el canal y libera las estructuras*/
	if(*ssl != NULL){
		SSL_shutdown(*ssl);
		SSL_free(*ssl);
		ssl=NULL;
	}

	if(*ctx != NULL){
		SSL_CTX_free(*ctx);
		ctx=NULL;
	}

	close(sockfd);
}


