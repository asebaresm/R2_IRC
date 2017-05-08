/**
 * @file log_test.c
 * @brief Prueba de la libreria de logs.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 13/02/17
 *
 */

#include "../includes/G-2361-06-P2-logger.h"

/**
* @brief Definicion del Mutex para el descriptor de fichero del log
*/
pthread_mutex_t loglock;

int main ()
{
   FILE* fp = NULL;
   fp = initLog();

   if (pthread_mutex_init(&loglock, NULL) != OK){
      perror("\n mutex init ha devuelto error\n");
      return EXIT_FAILURE;
   }

   if (fp == NULL){
      perror("ERR abriendo fichero");
      return (EXIT_FAILURE);
   }

   logEvent("Evento 1...");
   logEvent("Evento 2...");
   logEvent("Evento 3...");
   logEvent("Gatos :3");

   fp = fopen("fichero-inexistente.txt", "r");
   if (fp == NULL){
      logERR("Error de fopen detectado");
   }

   pthread_mutex_destroy(&loglock);
   return (EXIT_SUCCESS);
}