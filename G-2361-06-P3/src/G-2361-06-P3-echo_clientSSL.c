/** 
 * @file echo_client.c
 * @brief Cliente echo utilizado para comprender y desarrollar el servidor IRC con protocolo SSL.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * 
 */

#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>

#include "../includes/G-2361-06-P3-funciones_ssl.h"

#define BUFSIZE 1024 /**< Tam. max. del buffer que se lee*/

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[BUFSIZE] , server_reply[BUFSIZE];
    SSL_CTX* ctx=NULL;
    SSL * ssl=NULL;

    inicializar_nivel_SSL();
    ctx=fijar_contexto_SSL(CLIENT_CERT, CLIENT_KEY);

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        return 1;
    }
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        return 1;
    }
    
    ssl=conectar_canal_seguro_SSL(ctx, sock); 

    //keep communicating with server
    while(strcmp(message, "exit") != 0)
    {
        fscanf(stdin, "%s" , message);

        if( enviar_datos_SSL(ssl, message) < 0)
        	break;

        if( recibir_datos_SSL(ssl, message) < 0)
        	break;

        puts(message);

    }
    
    cerrar_canal_SSL(&ssl, &ctx, sock);
    fflush(stdout);
    close(sock);
    return 0;
}
