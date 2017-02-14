#=========================================
#   Redes de comunicaciones 2
# 	Escuela Politecnica Superior - UAM 
#=========================================

# Compilador
CC = gcc
# Flags compilacion
CFLAGS = -g -Wall
# Ejecutables
EXE = servidor
EXE_LEARN = echo_server echo_client
DIR_NAME = G-2361-06-P1
.PHONY: clean all

all: $(EXE)

echo: $(EXE_LEARN)

#===================================
# Construccion de los ejecutables
#===================================
# Ejecutables
servidor: src/servidor.c src/daemon.c
	$(CC) $(CFLAGS) -o servidor src/servidor.c src/daemon.c

echo_server: src/echo_server2.c
	$(CC) $(CFLAGS) -o echo_server src/echo_server2.c

echo_client: src/echo_client.c
	$(CC) $(CFLAGS) -o echo_client src/echo_client.c

#===========================
# Limpia fichero temporales
#===========================
clean:
	rm -f *.o *.a $(EXE) $(EXE_LEARN) $(DIR_NAME).tar.gz

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