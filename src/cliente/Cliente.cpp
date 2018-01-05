//*****************************************************************
// File:   Cliente.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2015
// Coms:   Ejemplo de cliente con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante
//         "make".
//*****************************************************************

#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "Socket.h"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

void lectura(Socket socket_fd, bool& fin){
	int read_bytes;
	string buffer;
	while(!fin){
		read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
		
	}
	
}
void escritura(Socket socket_fd, bool& fin){
	int send_bytes;
	string mensaje;
	while(!fin){*
		send_bytes = socket.Send(socket_fd, mensaje);
		
	}
	
}	
	
}
void handle_sigalrm(int signo){
	signal(SIGINT, handle_sigalrm);
}

int main(int argc, char* argv[]) {
	string SERVER_ADDRESS = argv[1];
	int SERVER_PORT = atoi(argv[2]);
	bool fin = false;
	
	const string MENS_FIN("exit");
	const string LLENO ("Vuelo esta LLENO");
	const string RECHAZADO("rechazado");
	const string ACEPTADO("aceptado");
	const string OCUPADO("Asiento solicidado ya esta OCUPADO");

	// Protegemos frente señal
	signal(SIGINT, handle_sigalrm);

    // Dirección y número donde escucha el proceso servidor

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_ADDRESS, SERVER_PORT);

    // Conectamos con el servidor. Probamos varias conexiones
	const int MAX_ATTEMPS = 3;
	int count = 0;
	int socket_fd;
	do {
		// Conexión con el servidor
    	socket_fd = socket.Connect();
    	count++;

    	// Si error --> esperamos 1 segundo para reconectar
    	if(socket_fd == -1){
    	    this_thread::sleep_for(chrono::seconds(1));
    	}
    } while(socket_fd == -1 && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_fd == -1){
    	return socket_fd;
    }

    thread lec;
    thread esc;
    lec = thread(&lectura,ref(socket_fd),ref(fin));
    esc = thread(&escritura,ref(socket_fd),ref(fin));
		
	

    // Recibimos la respuesta del servidor 
    string aux; 
    int read_bytes = socket.Recv(socket_fd, aux, MESSAGE_SIZE);
    // Cerramos Socket
    if(aux==RECHAZADO){

    	int error_code = socket.Close(socket_fd);
	    if(error_code == -1){
			cerr << "Error cerrando el socket: " << strerror(errno) << endl;
	    }
	    cout << "Conexión RECHAZADA"<<endl;
	    return error_code;
    }

    cout << "\"exit\" para finalizar operación"<<endl;
    cout << "Para seleccionar asiento escriba: fila columna"<<endl;
    string mensaje = "";

    // Buffer para almacenar la respuesta
	string buffer;
	bool mensajeContinua = false;
    int i = 0;
	do{

		if(buffer==OCUPADO){
			mensaje = "OK";
			mensajeContinua = true;
		}
		// Envio señal de finalizacion
		else if(i==4) {
			mensaje = MENS_FIN;
			cout << "Enviadas demasiadas solicitudes erroneas"<<endl;
		}
		else if(buffer == LLENO){
			mensaje = MENS_FIN;
			cout << "Vuelo LLENO"<<endl;
		}
		else {
			// Leer mensaje de la entrada estandar
			cout << "Que asiento desea reservar: ";
			getline(cin, mensaje);
			// Caso usuario no introduce nada, repetimo
			while(mensaje=="") {
				cout << "Que asiento desea reservar: ";
				getline(cin, mensaje);
			}
		}

		// Enviamos el mensaje
	    int send_bytes = socket.Send(socket_fd, mensaje);

	    if(send_bytes == -1){
			cerr << "Error al enviar datos: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}

		if(mensaje != MENS_FIN){
		    if(mensajeContinua){
		    	int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
		    	// Mostramos asientos libres
		    	cout << buffer << endl;
		    	mensajeContinua=false;
		    	++i;
		    }
		    else{
			    // Recibimos la respuesta del servidor  
			    int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
			    // Mostramos la respuesta
			    cout << "RESPUESTA: " << buffer << endl;
		    }
		}
	} while(mensaje != MENS_FIN);

    // Cerramos el socket
    int error_code = socket.Close(socket_fd);
    if(error_code == -1){
		cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    }

    return error_code;
}
