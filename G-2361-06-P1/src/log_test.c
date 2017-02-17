/**
 * @file log_test.c
 * @brief Prueba de la libreria de logs.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @date 13/02/17
 *
 */

#include "../includes/logger.h"

int main ()
{
   FILE* fp = NULL;
   fp = initLog();
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

   return (EXIT_SUCCESS);
}