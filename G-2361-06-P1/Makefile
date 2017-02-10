#=========================================
#   Redes de comunicaciones 2
# 	Escuela Politecnica Superior - UAM 
#=========================================

# Compilador
CC = gcc
# Creador de librerias
AR = ar rv

# Opciones de compilacion
CFLAGS = -Wall -g -ansi
LIBFLAG= -L ./lib
LIBS = -lpthread -lircredes -lirctad
DIRFLAG = doc include lib man obj src srclib Makefile

# Ejecutable
EXE = <ejecutable> empaquetar info documents

.PHONY: clean all


all: $(EXE)

#===================================
# Construccion de los ejecutables
#===================================
	# Ejecutable
<ejecutable>: src/<nombre_fic>.c srclib/<nombre_fic_lib>.c lib/<nombre_lib>.a
 	$(CC) -o @a src/<nombre_fic>.c srclib/<nombre_fic_lib>.c $(LIBFLAG) -l<nombre_lib> $(LIBS)

	# Objetos: 
obj/<nombre_obj>.o: srclib/<nombre_fic_lib>.c
	$(CC) $(CFLAGS) -o obj/<nombre_obj>.o -c srclib/<nombre_fic>.c

	# Librerias: 
lib/<nombre_lib>.a: obj/<nombre_obj>.o
	$(AR) lib/<nombre_lib>.a obj/<nombre_obj>.o
	
#===========================
# Limpia fichero temporales
#===========================
clean:
	rm -f obj/*.o lib/*.a <ejecutable>

#================================
# Autores/ayuda de la práctica
#================================
info:
	@echo "--------------------------------------------"
	@echo "    Practica X Redes de comunicaciones 2"
	@echo "     Alfonso Sebares y Beatriz de Pablo."
	@echo "			    Ayuda: make help" 
	@echo "		    Doxygen: make documents"
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
documents:
	doxygen -g <config_file>
	doxygen <config_file>
	
empaquetar:
	@make clean
	@rm -rf G-2361-<PP>-P1
	@rm -f G-2361-<PP>-P1.tar.gz
	@mkdir G-2361-<PP>-P1
	@cp -R $(DIRFLAG) G-2361-<PP>-P1
	@tar -czvf G-2361-<PP>-P1.tar.gz G-2361-<PP>-P1
	@rm -rf G-2361-<PP>-P1