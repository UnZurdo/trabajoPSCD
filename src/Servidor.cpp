//******************************************************************
// File:   ServidorMulticliente.cpp
// Author: PSCD-Unizar
// Date:   Noviembre 2015
// Coms:   Ejemplo de servidor multicliente con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante "make".
//*****************************************************************

#include "Socket.h"
#include <iostream>
#include <thread>
#include <sstream>
#include <cstring> //manejo de cadenas tipo C
#include "Monitor.h"

using namespace std;

//-------------------------------------------------------------
// Cuenta el número de vocales existentes en un mensaje
// PRE: 
// POST: Devuelve el número de vocales existentes en el mensaje 'message'
int cuentaVocales(char* message) {

	int count = 0;
	for (int i=0; i < strlen(message); i++) {

		switch (message[i]) {
			case 'a': case 'A': 
			case 'e': case 'E': 
			case 'i': case 'I': 
			case 'o': case 'O': 
			case 'u': case 'U': 
						count++;
					  	break;
		}
	}

	return count;
}
//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd, Monitor& monitor) {
	char MENS_FIN[]="EXIT";
	// Buffer para recibir el mensaje
	int length = 100;
	char buffer[length];
	char *temp;

	bool out = false; // Inicialmente no salir del bucle
	while(!out) {
		// Recibimos el mensaje del cliente
		int rcv_bytes = soc.Recv(client_fd,buffer,length);
		if (rcv_bytes == -1) {
			string mensError(strerror(errno));
    		cerr << "Error al recibir datos: " + mensError + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
		}

		// Si recibimos "END OF SERVICE" --> Fin de la comunicación
		if (0 == strcmp(buffer, MENS_FIN)) {
			out = true; // Salir del bucle
		} else {

			// Parseamos al entrada ante varios delimitadores
			temp = strtok(buffer, " ,-");
			int fila = atoi(temp);
			temp = strtok(NULL, " ,-");
			int columna = atoi(temp);
			cout << "Asiento pedido ==> fila: " << fila << " columna: " << columna << endl;

			// Gestionamos respuesta
			int estado = monitor.Entrar(fila,columna);

			// Enviamos la respuesta
			string s;
			if(estado == 0){
				s = "Asiento solicidado RESERVADO";
			}
			else if(estado == 1){
				s = "Asiento solicidado ya esta OCUPADO";
			}
			else if(estado==2){
				s = "Vuelo esta LLENO";
			}
			else{
				s ="MENSAJE INVALIDO";
			}

			const char* message = s.c_str();
		
			int send_bytes = soc.Send(client_fd, message);
			if(send_bytes == -1) {
				string mensError(strerror(errno));
    			cerr << "Error al enviar datos: " + mensError + "\n";
				// Cerramos los sockets
				monitor.Salir();
				soc.Close(client_fd);
				exit(1);
			}
		}
	}
	monitor.Salir();
	soc.Close(client_fd);
}

void administrador();

//-------------------------------------------------------------
int main(int argc, char** argv) {
	int filas = 10;
	int columnas = 4;
	int max_connections = 10;
	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = "localhost";
	int SERVER_PORT = atoi(argv[1]);

    int client_fd;
    Monitor monitor(filas, columnas);

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_PORT);

	// Bind 
	int socket_fd =socket.Bind();
	if (socket_fd == -1) {
		string mensError(strerror(errno));
    	cerr << "Error en el bind: " + mensError + "\n";
		exit(1);
	}

	// Listen

	int error_code = socket.Listen(max_connections); ///////// ESC
	if (error_code == -1) {
		string mensError(strerror(errno));
    	cerr << "Error en el listen: " + mensError + "\n";
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}

	for (int i=0; i<max_connections; i++) {
		// Accept
		client_fd = socket.Accept();

		if(client_fd == -1) {
			string mensError(strerror(errno));
    		cerr << "Error en el accept: " + mensError + "\n";
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}

		cout << "Lanzo thread nuevo cliente " + to_string(i) + "\n";
		thread cliente = thread(&servCliente, ref(socket), client_fd, ref(monitor));
		cliente.detach();
		cout << "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
	}
	
	//¿Qué pasa si algún thread acaba inesperadamente?
	monitor.Finalizar();

    // Cerramos el socket del servidor
    error_code = socket.Close(socket_fd);
    if (error_code == -1) {
		string mensError(strerror(errno));
    	cerr << "Error cerrando el socket del servidor: " + mensError + "\n";
	}

	// Mensaje de despedida
	cout << "Bye bye" << endl;

    return error_code;
}
//-------------------------------------------------------------
