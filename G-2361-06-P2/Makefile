#=========================================
#   Redes de comunicaciones 2
# 	Escuela Politecnica Superior - UAM 
#=========================================

#Evitar el warning 'accesibility bus': export NO_AT_BRIDGE=1

# Compilador
CC = gcc
#Cuando se rompe algo en el portátil de Alfonso (ignorar)
#INC = -I/G-2361-06-P2-usr/G-2361-06-P2-include/G-2361-06-P2-glib-2.0 -I/G-2361-06-P2-usr/G-2361-06-P2-lib/G-2361-06-P2-x86_64-linux-gnu/G-2361-06-P2-glib-2.0/G-2361-06-P2-include -I/G-2361-06-P2-usr/G-2361-06-P2-include/G-2361-06-P2-gtk-2.0 -I/G-2361-06-P2-usr/G-2361-06-P2-include/G-2361-06-P2-cairo -I/G-2361-06-P2-usr/G-2361-06-P2-include/G-2361-06-P2-pango-1.0 -I/G-2361-06-P2-usr/G-2361-06-P2-include/G-2361-06-P2-gtk-3.0/G-2361-06-P2-gdk -I/G-2361-06-P2-usr/G-2361-06-P2-include/G-2361-06-P2-gdk-pixbuf-2.0 -I/G-2361-06-P2-usr/G-2361-06-P2-include/G-2361-06-P2-atk-1.0
# Flags compilacion
#CFLAGS = -g -Wall $(INC) -Wno-unused-variable -Wno-unused-but-set-variable
CFLAGS = -g -Wall -Wno-unused-variable -Wno-unused-but-set-variable
IFLAGS = -lircinterface `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0` -rdynamic
LIBS = -lircinterface -lircredes -lirctad
# Ejecutables
EXE = G-2361-06-P2-xchat2
EXE_LEARN = G-2361-06-P2-echo_server G-2361-06-P2-echo_client G-2361-06-P2-log_test G-2361-06-P2-conexion_tcp_test
DIR_NAME = G-2361-06-P2
.PHONY: clean all

all: $(EXE)

extra: $(EXE_LEARN)

#===================================
# Construccion de los ejecutables
#===================================
# Ejecutables
G-2361-06-P2-xchat2: src/G-2361-06-P2-xchat2.c lib/G-2361-06-P2-lib-logger.a lib/G-2361-06-P2-lib-conexion_tcp.a lib/G-2361-06-P2-lib-aux_functions.a lib/G-2361-06-P2-lib-user_commands.a
	$(CC) $(CFLAGS) src/G-2361-06-P2-xchat2.c -o G-2361-06-P2-xchat2 $(IFLAGS) lib/G-2361-06-P2-lib-logger.a lib/G-2361-06-P2-lib-conexion_tcp.a lib/G-2361-06-P2-lib-user_commands.a lib/G-2361-06-P2-lib-aux_functions.a -pthread ${LIBS}

G-2361-06-P2-echo_server: src/G-2361-06-P2-echo_server.c
	$(CC) $(CFLAGS) -o G-2361-06-P2-echo_server src/G-2361-06-P2-echo_server.c

G-2361-06-P2-echo_client: src/G-2361-06-P2-echo_client.c
	$(CC) $(CFLAGS) -o G-2361-06-P2-echo_client src/G-2361-06-P2-echo_client.c

G-2361-06-P2-log_test: src/G-2361-06-P2-log_test.c lib/G-2361-06-P2-lib-logger.a
	$(CC) $(CFLAGS) -o G-2361-06-P2-log_test src/G-2361-06-P2-log_test.c lib/G-2361-06-P2-lib-logger.a

G-2361-06-P2-conexion_tcp_test: src/G-2361-06-P2-conexion_tcp_test.c lib/G-2361-06-P2-lib-logger.a lib/G-2361-06-P2-lib-conexion_tcp.a
	$(CC) $(CFLAGS) -o G-2361-06-P2-conexion_tcp_test src/G-2361-06-P2-conexion_tcp_test.c lib/G-2361-06-P2-lib-conexion_tcp.a lib/G-2361-06-P2-lib-logger.a

#Objetos
objlib/G-2361-06-P2-logger.o: srclib/G-2361-06-P2-logger.c
	$(CC) $(CFLAGS) -c srclib/G-2361-06-P2-logger.c -o objlib/G-2361-06-P2-logger.o

objlib/G-2361-06-P2-conexion_tcp.o: srclib/G-2361-06-P2-conexion_tcp.c
	$(CC) $(CFLAGS) -c srclib/G-2361-06-P2-conexion_tcp.c -o objlib/G-2361-06-P2-conexion_tcp.o

objlib/G-2361-06-P2-aux_functions.o: srclib/G-2361-06-P2-aux_functions.c
	$(CC) $(CFLAGS) -c srclib/G-2361-06-P2-aux_functions.c -o objlib/G-2361-06-P2-aux_functions.o $(IFLAGS) $(LIBS)

objlib/G-2361-06-P2-user_commands.o: srclib/G-2361-06-P2-user_commands.c
	$(CC) $(CFLAGS) -c srclib/G-2361-06-P2-user_commands.c -o objlib/G-2361-06-P2-user_commands.o $(IFLAGS) $(LIBS)

#Libs
lib/G-2361-06-P2-lib-logger.a: objlib/G-2361-06-P2-logger.o	
	ar rcs lib/G-2361-06-P2-lib-logger.a objlib/G-2361-06-P2-logger.o

lib/G-2361-06-P2-lib-conexion_tcp.a: objlib/G-2361-06-P2-conexion_tcp.o
	ar rcs lib/G-2361-06-P2-lib-conexion_tcp.a objlib/G-2361-06-P2-conexion_tcp.o

lib/G-2361-06-P2-lib-user_commands.a: objlib/G-2361-06-P2-user_commands.o
	ar rcs lib/G-2361-06-P2-lib-user_commands.a objlib/G-2361-06-P2-user_commands.o

lib/G-2361-06-P2-lib-aux_functions.a: objlib/G-2361-06-P2-aux_functions.o
	ar rcs lib/G-2361-06-P2-lib-aux_functions.a objlib/G-2361-06-P2-aux_functions.o

#===========================
# Limpia ficheros temporales
#===========================
clean:
	rm -f lib/*.a obj/*.o objlib/*.o $(EXE) $(EXE_LEARN) $(DIR_NAME).tar.gz

#================================
# Autores/G-2361-06-P2-ayuda de la práctica
#================================
info:
	@echo "--------------------------------------------"
	@echo "    Practica 1 Redes de comunicaciones 2"
	@echo "     Alfonso Sebares, Beatriz de Pablo y"
	@echo "              Celia Mateos"
	@echo "            Ayuda: make help" 
	@echo "--------------------------------------------"
	
help:
	@echo "Crear el ejecutable de la practica:\t\tmake"
	@echo "Borrar todo lo generado:\t\tmake clean"
	@echo "Para eliminar el demonio:\t\tmake killall"
	
#==================
# Destruir servidor 
#==================
kill:
	killall servidor
	@echo "Demonio eliminado"

	
#====================================
# Documentación/G-2361-06-P2-comprimir la práctica
#====================================	
empaquetar:
	@make clean
	@tar -czvf $(DIR_NAME).tar.gz ../$(DIR_NAME)