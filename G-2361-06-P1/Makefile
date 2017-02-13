#=========================================
#   Redes de comunicaciones 2
# 	Escuela Politecnica Superior - UAM 
#=========================================

# Compilador
CC = gcc
# Flags compilacion
CFLAGS = -Wall -g
# Ejecutables
EXE = servidor
DIR_NAME = G-2361-06-P1
.PHONY: clean all

all: $(EXE)

#===================================
# Construccion de los ejecutables
#===================================
# Ejecutable
servidor: src/servidor.c src/daemon.c
	$(CC) $(CFLAGS) -o servidor src/servidor.c src/daemon.c

	
#===========================
# Limpia fichero temporales
#===========================
clean:
	rm -f *.o *.a $(EXE) $(DIR_NAME).tar.gz

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
	@echo "Para crear el ejecutable: make"
	@echo "Para borrar el ejecutable: make clean"
	@echo "Para eliminar el demonio: make killall"
	
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