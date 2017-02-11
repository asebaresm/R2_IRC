#=========================================
#   Redes de comunicaciones 2
# 	Escuela Politecnica Superior - UAM 
#=========================================

# Compilador
CC = gcc

# Ejecutable
EXE = servidor info

.PHONY: clean all

all: $(EXE)

#===================================
# Construccion de los ejecutables
#===================================
# Ejecutable
servidor: src/servidor.c src/daemon.c
	$(CC) -o servidor src/servidor.c src/daemon.c

	
#===========================
# Limpia fichero temporales
#===========================
clean:
	rm -f servidor G-2361-06-P1.tar.gz

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
	@tar -czvf G-2361-06-P1.tar.gz ../G-2361-06-P1