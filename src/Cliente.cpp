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
#include "Socket.h"

using namespace std;


/*
 	-Limitar la cantidad de dinero del cliente???
 	- Establecer una cantidad inicial

*/
const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

int main(int argc, char* argv[]) {
	string SERVER_ADDRESS = argv[1];
	int SERVER_PORT = atoi(argv[2]);

	const string MENS_FIN("EXIT");
    // Dirección y número donde escucha el proceso servidor

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_ADDRESS, SERVER_PORT);

    // Conectamos con el servidor. Probamos varias conexiones
	const int MAX_ATTEMPS = 10;
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

    cout << "EXIT para finalizar operación"<<endl;
    string mensaje;
	do{
		// Leer mensaje de la entrada estandar
		cout << "Que asiento desea reservar: ";
		getline(cin, mensaje);
		// Enviamos el mensaje
	    int send_bytes = socket.Send(socket_fd, mensaje);

	    if(send_bytes == -1){
			cerr << "Error al enviar datos: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}

		if(mensaje != MENS_FIN){
		    // Buffer para almacenar la respuesta
		    string buffer;

		    // Recibimos la respuesta del servidor  
		    int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);

		    // Mostramos la respuesta
		    cout << "RESPUESTA: " << buffer << endl;
		}
	} while(mensaje != MENS_FIN);

    // Cerramos el socket
    int error_code = socket.Close(socket_fd);
    if(error_code == -1){
		cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    }

    return error_code;
}
