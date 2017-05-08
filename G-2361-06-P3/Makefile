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
LIBSSLL = -lircinterface -lircredes -lirctad
LFLAGSSSL = -lssl -lcrypto
# Ejecutables
EXE = certificados
ECHO = cliente_echo servidor_echo G-2361-06-P3-echo_server G-2361-06-P3-echo_client 

##PRACTICA 1 SERVIDOR IRC:
LFLAGS= -lpthread -lircinterface -lircredes -lirctad -Llib -LG-2361-06-P1-libServidor
GROUP = G-2361-06
PREFIX = $(GROUP)-P1
PREFIX3 = $(GROUP)-P3
DIR = $(GROUP)
SRC = src
SRCLIB = srclib
SRCLIBS = $(addprefix $(SRCLIB)/$(PREFIX)-,Sockets.c)
LIBDIR = lib
LIBS = $(addprefix $(LIBDIR)/$(PREFIX)-,libServidor.a)
INCLUDESDIR = includes
INCLUDES = $(addprefix $(INCLUDESDIR)/$(PREFIX)-,Sockets.h Server.h Daemon.h Functions.h)
OBJDIR = obj
OBJSocket= $(addprefix $(OBJDIR)/$(PREFIX)-,Sockets.o)
OBJServer=$(addprefix $(OBJDIR)/$(PREFIX)-,Server.o)
OBJFunctions=$(addprefix $(OBJDIR)/$(PREFIX)-,Functions.o)
OBJDaemon=$(addprefix $(OBJDIR)/$(PREFIX)-,Daemon.o)
OBJSMAIN= $(addprefix $(OBJDIR)/$(PREFIX)-,prueba.o)
MANDIR = man
## Nombre de los ejecutables 
EXECP1 = cliente_servidor/servidor_IRC

.PHONY: clean all

all: $(EXE) $(ECHO)

#===================================
# Construccion de los ejecutables
#===================================
# Ejecutables
G-2361-06-P3-echo_server: src/G-2361-06-P1-echo_server.c
	$(CC) $(CFLAGS) -o G-2361-06-P1-echo_server src/G-2361-06-P1-echo_server.c
	@echo "\t\033[32m--- Creado ejecutable echo-server.exe ---\033[00m"  

G-2361-06-P3-echo_client: src/G-2361-06-P2-echo_client.c
	$(CC) $(CFLAGS) -o G-2361-06-P2-echo_client src/G-2361-06-P2-echo_client.c
	@echo "\t\033[32m--- Creado ejecutable echo-client.exe ---\033[00m"  

cliente_echo: src/G-2361-06-P3-echo_clientSSL.c lib/G-2361-06-P3-lib-funciones_ssl.a
	$(CC) $(CFLAGS) -o echo/cliente_echo src/G-2361-06-P3-echo_clientSSL.c lib/G-2361-06-P3-lib-funciones_ssl.a $(LFLAGSSSL)
	@echo "\t\033[32m--- Creado ejecutable cliente_echo.exe ---\033[00m"  

servidor_echo: src/G-2361-06-P3-echo_serverSSL.c lib/G-2361-06-P3-lib-funciones_ssl.a
	$(CC) $(CFLAGS) -o echo/servidor_echo src/G-2361-06-P3-echo_serverSSL.c lib/G-2361-06-P3-lib-funciones_ssl.a $(LFLAGSSSL)
	@echo "\t\033[32m--- Creado ejecutable server_echo.exe ---\033[00m"    


#__________________Objetos______________
objlib/G-2361-06-P3-funciones_ssl.o: srclib/G-2361-06-P3-funciones_ssl.c
	$(CC) $(CFLAGS) -c srclib/G-2361-06-P3-funciones_ssl.c -o objlib/G-2361-06-P3-funciones_ssl.o $(IFLAGS) $(LIBSSLL)
	@echo "\t\033[32m--- Creado objeto funciones_ssl.o ---\033[00m"


#____________________________Libs_____________________________
lib/G-2361-06-P3-lib-funciones_ssl.a: objlib/G-2361-06-P3-funciones_ssl.o
	ar rcs lib/G-2361-06-P3-lib-funciones_ssl.a objlib/G-2361-06-P3-funciones_ssl.o 
	@echo "\t\033[32m--- Creadas libreriasSSL ---\033[00m"


#==================================================================================
# Creacion de certificados (cliente y servidor) para comunicaciones seguras
#==================================================================================
certificados:

	@echo "\nGenerando certificado CA"
	openssl genrsa -out certs/rootkey.pem 2048; 
	openssl req -new -x509 -key certs/rootkey.pem -out certs/rootcert.pem -subj "/C=ES/ST=Madrid/L=Alcobendas/O=EPS/CN=Redes2 CA"; 
	cat certs/rootcert.pem certs/rootkey.pem > certs/ca.pem
	openssl x509 -subject -issuer -noout -in certs/ca.pem;

	@echo "\nGenerando certificado Cliente"
	openssl genrsa -out certs/clientkey.pem 2048; 
	openssl req -new -key certs/clientkey.pem -out certs/clientreq.pem -subj "/C=ES/ST=Madrid/L=Alcobendas/O=EPS/CN=G-2361-06-P3-client"; 
	openssl x509 -req -in certs/clientreq.pem -CA certs/rootcert.pem -CAkey certs/rootkey.pem -CAcreateserial -out certs/clientcert.pem; 
	cat certs/clientcert.pem certs/clientkey.pem certs/rootcert.pem > certs/cliente.pem
	openssl x509 -subject -issuer -noout -in certs/cliente.pem 

	@echo "\nGenerando certificado Servidor"
	openssl genrsa -out certs/serverkey.pem 2048; 
	openssl req -new -key certs/serverkey.pem -out certs/serverreq.pem -subj "/C=ES/ST=Madrid/L=Alcobendas/O=EPS/CN=G-2361-06-P3-server"; 
	openssl x509 -req -in certs/serverreq.pem -CA certs/rootcert.pem -CAkey certs/rootkey.pem -CAcreateserial -out certs/servercert.pem; 
	cat certs/servercert.pem certs/serverkey.pem certs/rootcert.pem > certs/servidor.pem
	openssl x509 -subject -issuer -noout -in certs/servidor.pem 


#===========================
# Limpia ficheros temporales
#===========================
clean:
	rm -f certs/*.pem certs/*.srl lib/*.a obj/*.o objlib/*.o echo/cliente_echo echo/servidor_echo $(EXE) $(EXE_LEARN) $(DIR_NAME).tar.gz G-2361-06-P1-echo_server G-2361-06-P2-echo_client
	
#==================
# Destruir servidor 
#==================
kill:
	killall servidor
	@echo "Demonio eliminado"

#====================================
# Documentación/comprimir la práctica
#====================================	
distr: clean
	cd .. ; tar -cvzf $(PREFIX3).tar.gz $(PREFIX3)
	cd .. ; cp $(PREFIX3)/autores.txt .