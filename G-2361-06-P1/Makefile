#=========================================
#   Redes de comunicaciones 2
# 	Escuela Politecnica Superior - UAM 
#=========================================

# Compilador
CC = gcc
# Flags compilacion
CFLAGS = -g -Wall
LIBS = -lircinterface -lircredes -lirctad
# Ejecutables
EXE = servidor
EXE_LEARN = echo_server echo_client log_test conexion_tcp_test
DIR_NAME = G-2361-06-P1
.PHONY: clean all

all: $(EXE)

echo: $(EXE_LEARN)

#===================================
# Construccion de los ejecutables
#===================================
# Ejecutables
servidor: src/servidor.c src/daemon.c srclib/lib-logger.a srclib/lib-conexion_tcp.a
	$(CC) $(CFLAGS) -o servidor src/servidor.c src/daemon.c srclib/lib-logger.a srclib/lib-conexion_tcp.a -lpthread ${LIBS}

echo_server: src/echo_server.c
	$(CC) $(CFLAGS) -o echo_server src/echo_server.c

echo_client: src/echo_client.c
	$(CC) $(CFLAGS) -o echo_client src/echo_client.c

log_test: src/log_test.c srclib/lib-logger.a
	$(CC) $(CFLAGS) -o log_test src/log_test.c srclib/lib-logger.a

obj/logger.o: src/logger.c
	gcc -c src/logger.c -o obj/logger.o

obj/conexion_tcp.o: src/conexion_tcp.c
	gcc -c src/conexion_tcp.c -o obj/conexion_tcp.o

srclib/lib-logger.a: obj/logger.o	
	ar rcs srclib/lib-logger.a obj/logger.o

srclib/lib-conexion_tcp.a: obj/conexion_tcp.o
	ar rcs srclib/lib-conexion_tcp.a obj/conexion_tcp.o

conexion_tcp_test: src/conexion_tcp_test.c srclib/lib-logger.a srclib/lib-conexion_tcp.a
	$(CC) $(CFLAGS) -o conexion_tcp_test src/conexion_tcp_test.c srclib/lib-conexion_tcp.a srclib/lib-logger.a

#===========================
# Limpia fichero temporales
#===========================
clean:
	rm -f obj/*.o srclib/lib/*.a $(EXE) $(EXE_LEARN) $(DIR_NAME).tar.gz
	rm -f logs/*.log

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