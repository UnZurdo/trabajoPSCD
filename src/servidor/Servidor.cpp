//******************************************************************
// File:   ServidorMulticliente.cpp
// Author: PSCD-Unizar
// Date:   Noviembre 2015
// Coms:   Ejemplo de servidor multicliente con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante "make".
//*****************************************************************

#include "../librerias/Socket.h"
#include <iostream>
#include <thread>
#include <sstream>
#include <cstring> //manejo de cadenas tipo C
#include "Administrador.h"


using namespace std;

//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd, Monitor& monitor, bool& fin) {
	monitor.Entrar();
	char MENS_FIN[]="exit";
	char RECHAZADO[]="rechazado";
	char ACEPTADO[]="aceptado";
	// Buffer para recibir el mensaje
	int length = 100;
	char buffer[length];
	char *temp;
	char *temp2;

	// Envio confirmacion de conexion aceptada/rechazada
	char* messageAUX;
	if(fin) messageAUX = RECHAZADO;
	else messageAUX = ACEPTADO;

	int send_bytes = soc.Send(client_fd, messageAUX);
	if(send_bytes == -1) {
		string mensError(strerror(errno));
		cerr << "Error al enviar datos: " + mensError + "\n";
		// Cerramos los sockets
		monitor.Salir();
		soc.Close(client_fd);
		exit(1);
	}
	// Inicialmente no salir del bucle
	bool out = false; 
	// Si conexion rechazada NO entrar al bucle y cerrar socket 
	if(strcmp(messageAUX,RECHAZADO)==0){
		out = true;
	}
	while(!out) {
		// Recibimos el mensaje del cliente
		int rcv_bytes = soc.Recv(client_fd,buffer,length);
		if (rcv_bytes == -1) {
			string mensError(strerror(errno));
    		cerr << "Error al recibir datos: " + mensError + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
		}
		string s;
		// Si recibimos "exit" --> Fin de la comunicación
		if (0 == strcmp(buffer, MENS_FIN)) {
			out = true; // Salir del bucle
		} 
		// Recibo confirmacion mensaje OCUPADO
		else if(0 == strcmp(buffer, "OK")){
			// Envio Info Asientos Libres si el mensaje anterior no ha fallado
			s = monitor.Estado();	
			const char* message = s.c_str();		
			send_bytes = soc.Send(client_fd, message);
			
		}else {

			// Parseamos al entrada ante varios delimitadores
			temp = strtok(buffer, " ,-");
			temp2 = strtok(NULL, " ,-");
			int fila,columna;
			// Valido mensaje
			if(temp && temp2){
				fila = atoi(temp);
				columna = atoi(temp2);
			}
			else{
				fila = -1;
				columna = -1;
			}
			cout << "Asiento pedido ==> fila: " << fila << " columna: " << columna;

			// Gestionamos respuesta
			int estado = monitor.Reservar(fila,columna);

			// Enviamos la respuesta
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
				cout << " -INVALIDO-";
			}
			cout <<endl;

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
	soc.Close(client_fd);
	monitor.Salir();
}

void administrator(Socket& socket, int socket_fd, bool& fin, Monitor& monitor){
	cout << "To quit write \"exit\""<<endl;
	string msg;
	while(msg!="exit"){
		getline(cin, msg);
	}
	cout << "Closing server...."<<endl;
	fin=true;

	monitor.Finalizar();

	cout << "Socket CLOSED"<<endl;

	socket.Close(socket_fd);
	exit(1);


}

//-------------------------------------------------------------
int main(int argc, char** argv) {
	int filas = 10;
	int columnas = 4;
	int max_connections = 100;
	bool fin = false;
	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = "localhost";
	int SERVER_PORT = atoi(argv[1]);
	thread administrador; 

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
	int error_code = socket.Listen(max_connections); 
	if (error_code == -1) {
		string mensError(strerror(errno));
    	cerr << "Error en el listen: " + mensError + "\n";
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}
	administrador = thread(&administrator, ref(socket), socket_fd, ref(fin), ref(monitor));

	int i=0;
	while(i<max_connections){
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
		thread cliente = thread(&servCliente, ref(socket), client_fd, ref(monitor), ref(fin));
		cliente.detach();
		cout << "Nuevo cliente " + to_string(i);
		if(!fin) cout << " aceptado" << endl;
		else cout << " RECHAZADO" <<endl;
		++i;	
	}
	
	//¿Qué pasa si algún thread acaba inesperadamente?
	monitor.Finalizar();
	administrador.join();


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
