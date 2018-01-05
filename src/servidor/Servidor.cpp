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

const char MENS_FIN[]="EXIT";
const char RECHAZADO[]="RECHAZADO";
const char ACEPTADO[]="ACEPTADO";
const char ESTADO[]="ESTADO";
const char AYUDA[]="AYUDA";
const char PUJAR[]="PUJAR";

//-------------------------------------------------------------
void recibir(Subasta& s, Monitor& monitor, Socket& soc, int client_fd, string& msg, bool& fin, bool& out){
	// Buffer para recibir el mensaje
	int length = 100;
	char buffer[length];
	char *temp;
	char *temp2;

	while(!out && !fin){
		msg="";
		int rcv_bytes = soc.Recv(client_fd,buffer,length);
		if (rcv_bytes == -1) {
			string mensError(strerror(errno));
    		cerr << "Error al recibir datos: " + mensError + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
		}
		if(strcmp(buffer, MENS_FIN)==0){
			out = true;
		}
		else if(strcmp(buffer, ESTADO)==0){
			msg = s.monitor->estado();
		}
		else if(strcmp(buffer,  AYUDA)==0){
			msg = "Escriba \"EXIT\" para abandonar la subasta.\nPara mostrar el estado actual de la subasta
			escriba ESTADO.\nSi desea pujar escriba: PUJAR <cantidad>\n";
		}
		else{
			// Parseamos al entrada ante varios delimitadores
			temp = strtok(buffer, " ");
			temp2 = strtok(NULL, " \n");
			// Valido mensaje
			if(temp && temp2){
				int puja = atoi(temp2);
				bool valida=s.monitor->Pujar(puja);
				if(!valida) msg="PUJA no aceptada" + s.monitor->estado();
				else msg="PUJA aceptada" + s.monitor->estado();
			}

		}

	}

}

//-------------------------------------------------------------
void enviar(Subasta& s, Monitor& monitor, Socket& soc, int client_fd, string& msg, bool& fin, bool& out){
	char* messageAUX;
	// else messageAUX = ACEPTADO;

	while (!out) {
		if(fin) {
			out = true;
			msg = RECHAZADO;
		}
		// Usar semaforo para evitar espera activa
		if(msg!=""){
			const char* message = msg.c_str();
			send_bytes = soc.Send(client_fd, message);
		}
	}
	if(out && !fin){
		// Desbloqueo espera:  recv() del cliente
		msg="FIN";
		const char* message = msg.c_str();
		send_bytes = soc.Send(client_fd, message);
	}
}

//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd, bool& fin,  Subasta& subasta) {
	subasta.monitor->Entrar();
	subasta.monitor->iniciar();
	string mensaje="";
	bool out = false;


	thread recibir = thread(&recibir , ref(subasta), ref(subasta.monitor), ref(soc), client_fd, ref(mensaje), ref(fin), ref(out));
	thread escribir = thread(&recibir , ref(subasta), ref(subasta.monitor), ref(soc), client_fd, ref(mensaje), ref(fin), ref(out));

	recibir.join();
	recibir.join();

	soc.Close(client_fd);
	subasta.monitor->Salir();

}

void administrator(Socket& socket, int socket_fd, bool& fin, Subasta& s){
	cout << "To quit write \"exit\""<<endl;
	string msg;
	while(msg!="exit"){
		getline(cin, msg);
	}
	cout << "Closing server...."<<endl;
	fin=true;

	subasta.monitor->Finalizar();

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

	// Creo subasta inicial
	Subasta s();
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
	administrador = thread(&administrator, ref(socket), socket_fd, ref(fin), ref(s));

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
		thread cliente = thread(&servCliente, ref(socket), client_fd, ref(fin), ref(s));
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
