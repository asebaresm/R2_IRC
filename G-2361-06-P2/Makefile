#=========================================
#   Redes de comunicaciones 2
# 	Escuela Politecnica Superior - UAM 
#=========================================

#Evitar el warning 'accesibility bus': export NO_AT_BRIDGE=1

# Compilador
CC = gcc
#Cuando se rompe algo en el portátil de Alfonso (ignorar)
INC = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gtk-2.0 -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/gtk-3.0/gdk -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/atk-1.0
# Flags compilacion
#CFLAGS = -g -Wall $(INC) -Wno-unused-variable -Wno-unused-but-set-variable
CFLAGS = -g -Wall -Wno-unused-variable -Wno-unused-but-set-variable
IFLAGS = -lircinterface `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0` -rdynamic
LIBS = -lircinterface -lircredes -lirctad
# Ejecutables
EXE = xchat2
EXE_LEARN = echo_server echo_client log_test conexion_tcp_test
DIR_NAME = G-2361-06-P1
.PHONY: clean all

all: $(EXE)

echo: $(EXE_LEARN)

#===================================
# Construccion de los ejecutables
#===================================
# Ejecutables
xchat2: xchat2.c src/daemon.c srclib/lib-logger.a srclib/lib-conexion_tcp.a srclib/lib-aux_functions.a srclib/lib-user_commands.a
	$(CC) $(CFLAGS) xchat2.c -o xchat2 $(IFLAGS) srclib/lib-logger.a srclib/lib-conexion_tcp.a srclib/lib-user_commands.a srclib/lib-aux_functions.a -pthread ${LIBS}

echo_server: src/echo_server.c
	$(CC) $(CFLAGS) -o echo_server src/echo_server.c

echo_client: src/echo_client.c
	$(CC) $(CFLAGS) -o echo_client src/echo_client.c

log_test: src/log_test.c srclib/lib-logger.a
	$(CC) $(CFLAGS) -o log_test src/log_test.c srclib/lib-logger.a

conexion_tcp_test: src/conexion_tcp_test.c srclib/lib-logger.a srclib/lib-conexion_tcp.a
	$(CC) $(CFLAGS) -o conexion_tcp_test src/conexion_tcp_test.c srclib/lib-conexion_tcp.a srclib/lib-logger.a

#Objetos
obj/logger.o: src/logger.c
	$(CC) $(CFLAGS) -c src/logger.c -o obj/logger.o

obj/conexion_tcp.o: src/conexion_tcp.c
	$(CC) $(CFLAGS) -c src/conexion_tcp.c -o obj/conexion_tcp.o

obj/aux_functions.o: src/aux_functions.c
	$(CC) $(CFLAGS) -c src/aux_functions.c -o obj/aux_functions.o $(IFLAGS) $(LIBS)

obj/user_commands.o: src/user_commands.c
	$(CC) $(CFLAGS) -c src/user_commands.c -o obj/user_commands.o $(IFLAGS) $(LIBS)

#Libs
srclib/lib-logger.a: obj/logger.o	
	ar rcs srclib/lib-logger.a obj/logger.o

srclib/lib-conexion_tcp.a: obj/conexion_tcp.o
	ar rcs srclib/lib-conexion_tcp.a obj/conexion_tcp.o

srclib/lib-user_commands.a: obj/user_commands.o
	ar rcs srclib/lib-user_commands.a obj/user_commands.o

srclib/lib-aux_functions.a: obj/aux_functions.o
	ar rcs srclib/lib-aux_functions.a obj/aux_functions.o

#===========================
# Limpia fichero temporales
#===========================
clean:
	rm -f obj/*.o srclib/*.a $(EXE) $(EXE_LEARN) $(DIR_NAME).tar.gz

#================================
# Autores/ayuda de la práctica
#================================
info:
	@echo "--------------------------------------------"
	@echo "    Practica 1 Redes de comunicaciones 2"
	@echo "     Alfonso Sebares y Beatriz de Pablo."
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
# Documentación/comprimir la práctica
#====================================	
empaquetar:
	@make clean
	@tar -czvf $(DIR_NAME).tar.gz ../$(DIR_NAME)