/**
 * @file funciones_SSL.h
 * @brief Implementacion de la seguridad SSL
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 25 de Abril de 2017
 *
 */

#ifndef _FUNCIONES_SSL_H
#define _FUNCIONES_SSL_H

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/x509v3.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <syslog.h>

#define CERT_PATH "certs/" /**< Carpeta de los certificados*/
#define CERT_FILE "certs/rootcert.pem" /**< Fichero con el certificado CA SSL */
#define SERVER_CERT "certs/servercert.pem" /**< Fichero con el certificado del servidor*/
#define SERVER_KEY "certs/serverkey.pem" /**< Fichero con la llave del servidor*/
#define CLIENT_CERT "certs/clientcert.pem" /**< Fichero con el certificado del cliente*/
#define CLIENT_KEY "certs/clientkey.pem" /**< Fichero con la llave del cliente*/

/**
 * 
 * @page inicializar_nivel_SSL inicializar_nivel_SSL
 *
 * @synopsis
 * @code
 * 	#include "G-2361-06-P3-funciones_ssl.h"
 *  void inicializar_nivel_SSL();
 * @endcode
 * 
 * @brief Función que realiza las llamadas necesarias utilizar la capa ssl.
 *
 * @description
 * La función se encargará de realizar todas las llamadas necesarias para habilitar 
 * en la aplicación el uso de la capa segura SSL.
 * 
 *
 * @parameters
 * No tiene ningun parametro.
 *
 * @return
 * No devuelve ningun valor 
 *
 * @author
 * Beatriz de Pablo García (beatriz.depablo@estudiante.uam.es) 
 * Celia Mateos del Miguel(cel.mateos@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 * 
 * @date 25 de Abril de 2017
 *
 * <hr>
*/
void inicializar_nivel_SSL();

/**
 * 
 * @page fijar_contexto_SSL fijar_contexto_SSL
 *
 *
 * @synopsis
 * @code
 * #include "G-2361-06-P3-funciones_ssl.h"
 * SSL_CTX* fijar_contexto_SSL(char* cert, char* key);
 * @endcode
 * 
 * @brief Función que inicializa el contexto para hacer uso de canales seguros .
 *
 * @description
 * La función se encargar de inicializar el contexto que será utilizado para
 * la creación de canales seguros. 
 * Recibe como parámetros las rutas a los certificados y las claves con la que se va a trabajar. 
 *
 * @parameters
 * @param[in] cert: certificado del que se utiliza la función
 * @param[in] key: certificado llave del que se utiliza la función
 *
 * @return
 * Puntero de la estructura del contexto creado.
 *
 * @author
 * Beatriz de Pablo García (beatriz.depablo@estudiante.uam.es) 
 * Celia Mateos del Miguel(cel.mateos@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 * 
 * @date 25 de Abril de 2017
 *
 * <hr>
*/
SSL_CTX* fijar_contexto_SSL(char* cert, char* key);

/**
 * 
 * @page conectar_canal_seguro_SSL conectar_canal_seguro_SSL
 *
 *
 * @synopsis
 * @code
 * #include "G-2361-06-P3-funciones_ssl.h"
 * SSL* conectar_canal_seguro_SSL(SSL_CTX* ctx,int sockfd);
 * @endcode
 * 
 * @brief Función que inicia el proceso de handshake para crear un canal seguro.
 *
 * @description
 * La función se encargará de obtener un canal seguro SSL inciando el proceso de handshake con el otro extremo.
 * Recibe como parámetros contexto SSL y identificador de socket. 
 *
 * @parameters
 * @param[in] ctx: contexto SSL
 * @param[in] sockfd: id del socket
 *
 * @return
 * Puntero de la estructura del contexto.
 *
 * @author
 * Beatriz de Pablo García (beatriz.depablo@estudiante.uam.es) 
 * Celia Mateos del Miguel(cel.mateos@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 * 
 * @date 25 de Abril de 2017
 *
 * <hr>
*/
SSL* conectar_canal_seguro_SSL(SSL_CTX* ctx,int sockfd);

/**
 * 
 * @page aceptar_canal_seguro_SSL aceptar_canal_seguro_SSL
 *
 * @synopsis
 * @code
 *  #include "G-2361-06-P3-funciones_ssl.h"
 *  SSL* aceptar_canal_seguro_SSL(SSL_CTX* ctx,int sockfd);
 * @endcode
 * 
 * @brief Función que se encarga de bloquear la aplicación. 
 *
 * @description
 *
 * La función se encargará de bloquear la aplicación, que se quedará esperando hasta recibir un handshake por parte del cliente.
 * Recibe como parámetros contexto SSL y identificdor de socket. 
 *
 * @parameters
 * @param[in] ctx: contexto SSL
 * @param[in] sockfd: id del socket
 *
 * @return
 * Puntero de la estructura del contexto. 
 *
 * @author
 * Beatriz de Pablo García (beatriz.depablo@estudiante.uam.es) 
 * Celia Mateos del Miguel(cel.mateos@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 * 
 * @date 25 de Abril de 2017
 *
 * <hr>
*/
SSL* aceptar_canal_seguro_SSL(SSL_CTX* ctx,int sockfd);

/**
 * 
 * @page evaluar_post_connectar_SSL evaluar_post_connectar_SSL
 *
 *
 * @synopsis
 * @code
 *  #include "G-2361-06-P3-funciones_ssl.h"
 *  int evaluar_post_connectar_SSL(SSL * ssl);
 * @endcode
 * 
 * @brief Función que comprueba si la comunicación se puede considerar segura. 
 *
 * @description
 * La funciónque comprueba una vez realizado el handshake si el canal de co-
 * municación se puede considerar seguro.
 * 
 * Recibe como parámetros contexto SSL. 
 *
 * @parameters
 * @param[in] ssl: contexto SSL
 *
 * @return
 * Devuelve 0 al no encontrar el certificado en caso contrario...
 *
 * @author
 * Beatriz de Pablo García (beatriz.depablo@estudiante.uam.es) 
 * Celia Mateos del Miguel(cel.mateos@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 * 
 * @date 25 de Abril de 2017
 *
 * <hr>
*/
int evaluar_post_connectar_SSL(SSL * ssl);

/**
 * 
 * @page enviar_datos_SSL enviar_datos_SSL
 *
 * @synopsis
 * @code
 * #include "G-2361-06-P3-funciones_ssl.h"
 * int enviar_datos_SSL(SSL *ssl, char *buffer);
 * @endcode
 * 
 * @brief Función que permite el envío de mensajes a través del canal seguro.  
 * 
 * @description
 * La función esequivalente a la función de envío de mensajes que se realizó en la
 * práctica 1, pero será utilizada para enviar datos a través del canal seguro.
 * Recibe como parámetros contexto SSL y el buffer
 *
 * @parameters
 * @param[in] ssl: contexto SSL
 * @param[in] buffer: cadena de texto
 *
 * @return
 * Devuelve 0 al no encontrar el certificado en caso contrario...
 *
 * @author
 * Beatriz de Pablo García (beatriz.depablo@estudiante.uam.es) 
 * Celia Mateos del Miguel(cel.mateos@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 * 
 * @date 25 de Abril de 2017
 *
 * <hr>
*/
int enviar_datos_SSL(SSL *ssl, char *buffer);


/**
 * 
 * @page recibir_datos_SSL recibir_datos_SSL
 *
 * @synopsis
 * @code
 * #include "G-2361-06-P3-funciones_ssl.h"
 * int recibir_datos_SSL(SSL *ssl, char *buffer);
 * @endcode
 * 
 *
 * @brief Función que será el equivalente a la función de lectura de mensajes que se realizó en la
 * práctica 1, pero será utilizada para enviar datos a través del canal seguro.  Es importante que sea genérica y
 * pueda ser utilizada independientemente de los datos que se vayan a recibir..
 *
 * @description
 * La función realiza el rcv de los datos.
 * Recibe como parámetros contexto SSL, buffer 
 *
 * @parameters
 * @param[in] ssl: contexto SSL
 * @param[in] buffer: cadena de texto
 *
 * @return
 * Devuelve 0 al no encontrar el certificado en caso contrario...
 *
 * @author
 * Beatriz de Pablo García (beatriz.depablo@estudiante.uam.es) 
 * Celia Mateos del Miguel(cel.mateos@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 * 
 * @date 25 de Abril de 2017
 *
 * <hr>
*/
int recibir_datos_SSL(SSL *ssl, char *buffer);

/**
 * 
 * @page cerrar_canal_SSL cerrar_canal_SSL
 *
 *
 * @synopsis
 * @code
 *  #include "G-2361-06-P3-funciones_ssl.h"
 *  void cerrar_canal_SSL(SSL **ssl,SSL_CTX **ctx, int sockfd);
 * @endcode
 * 
 * @brief Función que cierra la comunicación de un canal seguro.
 * previamente.
 *
 * @description
 * La función libera todos los recursos y cerra el canal de comunicación seguro creado.
 * Recibe como parámetros contexto SSL, ctx y el numero del socket. 
 *
 * @parameters
 * @param[in] ssl: certificado SSL
 * @param[in] ctx: contexto SSL
 * @param[in] sockfd: numero del socket
 *
 * @return
 * No devuelve ningun valor
 *
 * @author
 * Beatriz de Pablo García (beatriz.depablo@estudiante.uam.es) 
 * Celia Mateos del Miguel(cel.mateos@estudiante.uam.es)
 * Alfonso Sebares Mecha (alfonso.sebares@estudiante.uam.es)
 * 
 * @date 25 de Abril de 2017
 *
 * <hr>
*/
void cerrar_canal_SSL(SSL **ssl,SSL_CTX **ctx, int sockfd);

#endif
