/** 
 * @file echo_client.c
 * @brief Cliente echo utilizado para comprender y desarrollar el servidor IRC.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 28/04/17
 * 
 */

#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>

#define BUFSIZE 1024 /**< Tam. max. del buffer que se lee*/

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[BUFSIZE] , server_reply[BUFSIZE];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    //keep communicating with server
    while((strcmp(message, "_STOP_") != 0))
    {
        printf("Enter message : ");
        scanf("%s" , message);

        //Send some data
        if( send(sock , message , sizeof(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
        if( recv(sock , server_reply , BUFSIZE , 0) < 0)
        {
            puts("recv failed");
            break;
        }
         
        puts("Server reply :");
        puts(server_reply);
    }
     
    puts("_STOP_ enviado, cerrar conexion");
    close(sock);
    return 0;
}
