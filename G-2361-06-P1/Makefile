# Compilador
CC=gcc
# Opciones de compilacion
CFLAGS= -Wall 
LFLAGS= -lpthread -lircinterface -lircredes -lirctad -Llib -LG-2361-06-P1-libServidor

GROUP = G-2361-06
PREFIX = $(GROUP)-P1
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
MANS = $(addprefix $(OBJDIR)/$(PREFIX)-,)

## Nombre de los ejecutables 
EXECP1 = $(PREFIX)-Server 

#=============================================================
# Construccion de los ejecutables
#=============================================================

all : lib P1 #manual

lib : libServidor.a

.SECONDEXPANSION:
P1 : $(OBJSocket) $(OBJServer) $(OBJFunctions) $(OBJDaemon) $(OBJSMAIN)
	@echo "\n\t\033[35m--- Generando server.exe ---\033[00m"
	$(CC) -o  $(EXECP1) $(OBJDIR)/$(PREFIX)-prueba.o $(OBJSocket) $(OBJServer) $(OBJFunctions) $(OBJDaemon) $(LFLAGS)
	@echo "\t\033[32m--- Creado ejecutable server.exe ---\033[00m"  


#=============================================================
# Generacion .o
#=============================================================

$(OBJSocket) : $(SRCLIBS) $(INCLUDES) 
	@echo "\n\t\033[35m--- Generando Sockets.o ---\033[00m"
	$(CC) $(CFLAGS) -c $(SRCLIBS) -o  $(OBJDIR)/$(PREFIX)-Sockets.o 
	@echo "\t\033[32m--- Creado objeto Sockets.o ---\033[00m"

$(OBJServer) : $(SRCLIB)/$(PREFIX)-Server.c $(INCLUDES) 
	@echo "\t\033[35m--- Generando Server.o ---\033[00m"
	$(CC) $(CFLAGS) -c $(SRCLIB)/$(PREFIX)-Server.c -o  $(OBJDIR)/$(PREFIX)-Server.o 
	@echo "\n\t\033[32m--- Creado objeto Server.o ---\033[00m"

$(OBJFunctions) : $(SRCLIB)/$(PREFIX)-Functions.c $(INCLUDES) 
	@echo "\t\033[35m--- Generando Functions.o ---\033[00m"
	$(CC) $(CFLAGS) -c $(SRCLIB)/$(PREFIX)-Functions.c -o  $(OBJDIR)/$(PREFIX)-Functions.o 
	@echo "\n\t\033[32m--- Creado objeto Functions.o ---\033[00m"

$(OBJDaemon) : $(SRCLIB)/$(PREFIX)-Daemon.c $(INCLUDES) 
	@echo "\t\033[35m--- Generando Daemon.o ---\033[00m"
	$(CC) $(CFLAGS) -c $(SRCLIB)/$(PREFIX)-Daemon.c -o  $(OBJDIR)/$(PREFIX)-Daemon.o 
	@echo "\n\t\033[32m--- Creado objeto Daemon.o ---\033[00m"
	

	@echo "\n\t\033[35m--- Generando prueba.o ---\033[00m"
$(OBJSMAIN) : $(SRC)/$(PREFIX)-prueba.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $(SRC)/$(PREFIX)-prueba.c -o $(OBJDIR)/$(PREFIX)-prueba.o 
	@echo "\t\033[32m--- Creado objeto prueba.o ---\033[00m"
	
#=============================================================
# Generacion de librerias
#=============================================================
libServidor.a: $(OBJSocket) $(OBJServer) $(OBJFunctions) $(OBJDaemon)
	@echo "\n\t\033[35m--- Generando libreriasServidor ---\033[00m"
	ar -rv $(LIBDIR)/lib$(PREFIX)-Servidor.a $(OBJSocket) $(OBJServer) $(OBJFunctions)
	@echo "\t\033[32m--- Creadas libreriasServidor ---\033[00m"

#=============================================================
# Generacion de manual
#=============================================================
#manual:
#	@echo "\n\t\033[35m--- Generando manual ---\033[00m"
#	rm -rf *\.3 $(MANDIR)/man3/
#	doxygen Doxyfile
#	@echo "\t\033[32m--- Creado manual ---\033[00m"

#=============================================================
# Crea un archivo tar.gz para distribuir el codigo
#=============================================================

distr: clean
	cd .. ; tar -cvzf $(PREFIX).tar.gz $(PREFIX)
	cd .. ; cp $(PREFIX)/autores.txt .


#=============================================================
# Limpia fichero temporales
#=============================================================
clean:
	rm -rf *\.o $(EXECP1) $(OBJServerSocket) $(OBJServer) $(OBJFunctions) $(OBJDaemon) $(OBJSMAIN)
	#rm -rf *\.3 $(MANDIR)/man3/



