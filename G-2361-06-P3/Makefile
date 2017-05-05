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
ECHO = cliente_echo servidor_echo
EXE_LEARN = echo_server echo_client log_test conexion_tcp_test
DIR_NAME = G-2361-06-P3
.PHONY: clean all

all: $(EXE) $(ECHO)

extra: $(EXE_LEARN)

#===================================
# Construccion de los ejecutables
#===================================
# Ejecutables
xchat2: src/xchat2.c src/daemon.c lib/lib-logger.a lib/lib-conexion_tcp.a lib/lib-aux_functions.a lib/lib-user_commands.a
	$(CC) $(CFLAGS) src/xchat2.c -o xchat2 $(IFLAGS) lib/lib-logger.a lib/lib-conexion_tcp.a lib/lib-user_commands.a lib/lib-aux_functions.a -pthread ${LIBS}

echo_server: src/echo_server.c
	$(CC) $(CFLAGS) -o echo_server src/echo_server.c

echo_client: src/echo_client.c
	$(CC) $(CFLAGS) -o echo_client src/echo_client.c

log_test: src/log_test.c lib/lib-logger.a
	$(CC) $(CFLAGS) -o log_test src/log_test.c lib/lib-logger.a

conexion_tcp_test: src/conexion_tcp_test.c lib/lib-logger.a lib/lib-conexion_tcp.a
	$(CC) $(CFLAGS) -o conexion_tcp_test src/conexion_tcp_test.c lib/lib-conexion_tcp.a lib/lib-logger.a

#Ejecutables para las pruebas c3po - servidor/cliente echo con SSL
cliente_echo: src/echo_client.c
	$(CC) $(CFLAGS) -o echo/cliente_echo src/echo_client.c

servidor_echo: src/echo_server.c
	$(CC) $(CFLAGS) -o echo/servidor_echo src/echo_server.c

#Objetos
objlib/logger.o: srclib/logger.c
	$(CC) $(CFLAGS) -c srclib/logger.c -o objlib/logger.o

objlib/conexion_tcp.o: srclib/conexion_tcp.c
	$(CC) $(CFLAGS) -c srclib/conexion_tcp.c -o objlib/conexion_tcp.o

objlib/aux_functions.o: srclib/aux_functions.c
	$(CC) $(CFLAGS) -c srclib/aux_functions.c -o objlib/aux_functions.o $(IFLAGS) $(LIBS)

objlib/user_commands.o: srclib/user_commands.c
	$(CC) $(CFLAGS) -c srclib/user_commands.c -o objlib/user_commands.o $(IFLAGS) $(LIBS)

#Libs
lib/lib-logger.a: objlib/logger.o	
	ar rcs lib/lib-logger.a objlib/logger.o

lib/lib-conexion_tcp.a: objlib/conexion_tcp.o
	ar rcs lib/lib-conexion_tcp.a objlib/conexion_tcp.o

lib/lib-user_commands.a: objlib/user_commands.o
	ar rcs lib/lib-user_commands.a objlib/user_commands.o

lib/lib-aux_functions.a: objlib/aux_functions.o
	ar rcs lib/lib-aux_functions.a objlib/aux_functions.o
#==================================================================================
# Creacion de certificados (cliente y servidor) para comunicaciones seguras
#==================================================================================
certificados:
	#Certificado CA
	openssl genrsa -out certs/rootkey.pem 2048
	openssl req -new -x509 -key certs/rootkey.pem -out certs/rootcert.pem -subj "/C=ES/ST=Madrid/L=Alcobendas/O=EPS/CN=Redes2 CA"
	#printf 'ES\nMadrid\nAlcobendas\nEPS\nREDES2\nCA\nCA@uam.es\n'| openssl req -new -x509 -key certs/rootkey.pem -out certs/rootcert.pem
	cat certs/rootcert.pem certs/rootkey.pem > certs/ca.pem
	openssl x509 -subject -issuer -noout -in certs/ca.pem

	#Cliente
	openssl genrsa -out certs/clientkey.pem 2048
	openssl req -new -key certs/clientkey.pem -out certs/clientcert.pem -subj "/C=ES/ST=Madrid/L=Alcobendas/O=EPS/CN=G-2361-06-P3-cliente"
	#printf 'ES\nMadrid\nAlcobendas\nEPS\nREDES2\nclient\nclient@uam.es\n'| openssl req -new -key certs/clientkey.pem -out certs/clientcert.pem
	openssl x509 -req -in certs/clientcert.pem -CA certs/ca.pem -CAkey certs/ca.pem -CAcreateserial -out certs/clientcert.pem
	cat certs/clientcert.pem certs/clientkey.pem > certs/cliente.pem
	openssl x509 -subject -issuer -noout -in certs/cliente.pem
	openssl rsa -in certs/clientkey.pem -passin pass:$(PASS) -pubout -out certs/clientpubkeytemp.pem
	cat certs/clientpubkeytemp.pem certs/clientkey.pem > certs/clientpubkey.pem
	rm certs/clientpubkeytemp.pem

	#Servidor
	openssl genrsa -out certs/serverkey.pem 2048
	openssl req -new -key certs/serverkey.pem -out certs/servercert.pem -subj "/C=ES/ST=Madrid/L=Alcobendas/O=EPS/CN=G-2361-06-P3-servidor"
	#printf 'ES\nMadrid\nAlcobendas\nEPS\nREDES2\nserver\nserver@uam.es\n'| openssl req -new -key certs/serverkey.pem -out certs/servercert.pem
	openssl x509 -req -in certs/servercert.pem -CA certs/ca.pem -CAkey certs/ca.pem -CAcreateserial -out certs/servercert.pem
	cat certs/servercert.pem certs/serverkey.pem > certs/servidor.pem
	openssl x509 -subject -issuer -noout -in certs/servidor.pem
	openssl rsa -in certs/serverkey.pem -passin pass:$(PASS) -pubout -out certs/serverpubkeytemp.pem
	cat certs/serverpubkeytemp.pem certs/serverkey.pem > certs/serverpubkey.pem
	rm certs/serverpubkeytemp.pem	


#===========================
# Limpia ficheros temporales
#===========================
clean:
	rm -f lib/*.a obj/*.o objlib/*.o echo/cliente_echo echo/servidor_echo $(EXE) $(EXE_LEARN) $(DIR_NAME).tar.gz

clean_certs:
	rm -f certs/*.pem certs/*.srl
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