/**
 * @file types.h
 * @brief Definicion de tipos e includes comunes utilizados por los distintos modulos.
 * @author Alfonso Sebares
 * @author Beatriz de Pablo
 * @author Celia Mateos
 * @date 13/02/17
 *
 */

#ifndef types_h
#define types_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*Standard return types*/
#define ERR -1
#define OK 0

#define MAXDATA 1024
#define MAX_IRC_COMMAND 1000 /*https://www.alien.net.au/irc/irc2numerics.html*/

/*Color definitions*/
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#endif