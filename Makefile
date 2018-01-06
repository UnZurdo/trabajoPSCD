#*****************************************************************
# File:   Makefile
# Date:   enero 2017
# Coms:   Ejecutar como "make"
#         Para compilar en Hendrix (Solaris OS), descomentar
#         "#${SOCKETSFLAGS}"
#*****************************************************************

# ######## #
# COMANDOS #
# ######## #
RM = rm -f # Comando de limpieza
CPP=g++ # Comando de compilacion

# ######################## #
# VARIABLES GLOBALES       #
# ######################## #

#Librerias
SOCKET=Socket
IMAGEDOWNLOADER=ImageDownloader
SEMAPHORE=Semaphore

#Modulos servidor
VALLA=Valla
ADMINISTRADOR=Administrador
MONITOR=Monitor
GESTOR=Gestor
SERVIDOR=Servidor

#Modulos cliente
CLIENTE=Cliente
CLIENTEAUTOMATICO=ClienteAuto


#Target
TARGET=${SERVIDOR} ${CLIENTE} ${CLIENTEAUTOMATICO}

# #################### #
# FLAGS DE COMPILACION #
# #################### #

CPPFLAGS=-I. -I/usr/local/include -O2 -std=c++11 -fmax-errors=1 -Werror -lsockets # Flags compilacion

LDFLAGS= -L/usr/X11R6/lib -L/usr/local/lib -lm -pthread -lcurl -lX11 # Flags linkado threads

SOCKETSFLAGS=-lsocket -lnsl # Flags linkado sockets (Solaris SunOS)

.PHONY:all

all: ${TARGET}


# Compilacion de librerias servidor
bin/${SOCKET}.o: src/librerias/${SOCKET}.h src/librerias/${SOCKET}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/${SOCKET}.cpp -o bin/${SOCKET}.o
#-----------------------------------------------------------
bin/${IMAGEDOWNLOADER}.o: src/librerias/${IMAGEDOWNLOADER}.hpp src/librerias/${IMAGEDOWNLOADER}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/${IMAGEDOWNLOADER}.cpp -o bin/${IMAGEDOWNLOADER}.o
#-----------------------------------------------------------
bin/${SEMAPHORE}.o: src/librerias/${SEMAPHORE}.h src/librerias/${SEMAPHORE}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/${SEMAPHORE}.cpp -o bin/${SEMAPHORE}.o
#-----------------------------------------------------------
bin/${ADMINISTRADOR}.o: src/servidor/${ADMINISTRADOR}.h src/servidor/${ADMINISTRADOR}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/${ADMINISTRADOR}.cpp -o bin/${ADMINISTRADOR}.o
#-----------------------------------------------------------
bin/${GESTOR}.o: src/servidor/${GESTOR}.h src/servidor/${GESTOR}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/${GESTOR}.cpp -o bin/${GESTOR}.o
#-----------------------------------------------------------
bin/${MONITOR}.o: src/servidor/${MONITOR}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/${MONITOR}.cpp -o bin/${MONITOR}.o
#-----------------------------------------------------------
bin/${VALLA}.o: src/servidor/${VALLA}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/${VALLA}.cpp -o bin/${VALLA}.o
#-----------------------------------------------------------

# Compilacion de librerias cliente
bin/${CLIENTE}.o: src/cliente/${CLIENTE}.cpp
	${CPP} -c $(CPPFLAGS) src/cliente/${CLIENTE}.cpp -o bin/${CLIENTE}.o
#-----------------------------------------------------------
bin/${CLIENTEAUTOMATICO}.o: src/cliente/${CLIENTEAUTOMATICO}.cpp
	${CPP} -c $(CPPFLAGS) src/cliente/${CLIENTEAUTOMATICO}.cpp -o bin/${CLIENTEAUTOMATICO}.o
#-----------------------------------------------------------
# Linkado
${SERVIDOR}: bin/${MONITOR}.o bin/${IMAGEDOWNLOADER}.o bin/${SEMAPHORE}.o bin/${VALLA}.o bin/${GESTOR}.o bin/${ADMINISTRADOR}.o bin/${SOCKET}.o
	${CPP} bin/${MONITOR}.o bin/${IMAGEDOWNLOADER}.o bin/${SEMAPHORE}.o bin/${VALLA}.o bin/${GESTOR}.o bin/${ADMINISTRADOR}.o bin/${SOCKET}.o -o  bin/${SERVIDOR} ${LDFLAGS} #${SOCKETSFLAGS} #descomentar para Hendrix
#-----------------------------------------------------------
# Linkado
${CLIENTE}: bin/${CLIENTE}.o bin/${SOCKET}.o
	${CPP} bin/${CLIENTE}.o bin/${SOCKET}.o -o bin/${CLIENTE} ${LDFLAGS} #${SOCKETSFLAGS}
#-----------------------------------------------------------
${CLIENTEAUTOMATICO}: bin/${CLIENTEAUTOMATICO}.o bin/${SOCKET}.o
	${CPP} bin/${CLIENTEAUTOMATICO}.o bin/${SOCKET}.o -o bin/${CLIENTEAUTOMATICO} ${LDFLAGS} #${SOCKETSFLAGS}
#-----------------------------------------------------------

# LIMPIEZA
clean:
	$(RM) bin/${VALLA}.o
	$(RM) bin/${MONITOR}.o
	$(RM) bin/${GESTOR}.o
	$(RM) bin/${ADMINISTRADOR}.o
	$(RM) bin/${SOCKET}.o
	$(RM) bin/${IMAGEDOWNLOADER}.o
	$(RM) bin/${SEMAPHORE}.o
	$(RM) bin/${CLIENTE}.o:
	$(RM) bin/${CLIENTEAUTOMATICO}.o:
	$(RM) bin/${SERVIDOR}
	$(RM) bin/${CLIENTE}
	$(RM) bin/${CLIENTEAUTOMATICO}
	$(RM) imagenes/*.*
