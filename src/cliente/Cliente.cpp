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
#include "../librerias/Semaphore.h"
#include "../librerias/Socket.h"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

const char MENS_FIN[]="EXIT";
const char RECHAZADO[]="RECHAZADO";
const char ACEPTADO[]="ACEPTADO";
const char ESTADO[]="ESTADO";
const char AYUDA[]="AYUDA";
const char OCUPADO[]="OCUPADO";
// Informar de que se va a enviar datos a continuacion
const char NOMBRE[]="NOMBRE";
const char URL[]="URL";
const char PUJAR[]="PUJAR";

bool esGanador = false;

void lectura(Socket& socket, int socket_fd, bool& fin,bool& primeraVez,Semaphore& sem){
	int read_bytes;
	string buffer;
	while(!fin){
		read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
		if(read_bytes == -1){
			cout << "Error en el recv del cliente" << endl;
			exit(0);
		}
		else {
			if(primeraVez == true){
				primeraVez = false;
				sem.signal();
			}

			//OCUPADO ==> SERVER NO PUEDE RESPONDER DE MOMENTO

			if(buffer == "FIN"){
				fin = true;
				cout << "Final recibido del servidor" << endl;
			}
			else if(buffer == "URL"){
				esGanador = true;
			}
			else{
				cout << buffer << endl;
			}
		}
	}
}
void escritura(Socket& socket, string url, int socket_fd, bool& fin,bool& primeraVez,Semaphore& sem){
	int send_bytes;
	string mensaje;
	string name;
	while(!fin){
		if(primeraVez == true){
			sem.wait();

			mensaje= "URL " + url;
			// name= "NOMBRE " + name;
			send_bytes = socket.Send(socket_fd, mensaje);
			if(send_bytes == -1){
				cout << "Error en el send del cliente" << endl;
				exit(0);
			}
		}

		getline(cin, mensaje);
		// Caso usuario no introduce nada, repetimo
		while(mensaje=="") {
			//cout << "Que asiento desea reservar: ";
			getline(cin, mensaje);
		}

		send_bytes = socket.Send(socket_fd, mensaje);
		if(send_bytes == -1){
			cout << "Error en el send del cliente" << endl;
			exit(0);
		}
		if (mensaje == "EXIT"){
				fin = true;
		}

		// 1) MENSAJE == AYUDA

		// 2) MENSAJE == ESTADO
	}
	//Fuera del bucle???
	if(esGanador) {
		mensaje= "URL " + url;
		// name= "NOMBRE " + name;
		send_bytes = socket.Send(socket_fd, mensaje);
		if(send_bytes == -1){
			cout << "Error en el send del cliente" << endl;
			exit(0);
		}
	}
}


void handle_sigalrm(int signo){
	signal(SIGINT, handle_sigalrm);
}

int main(int argc, char* argv[]) {
	if (argc != 5){
		cout << "ERROR, inserte los parametros correctamente" << endl;
		exit(1);
	}

	string name = argv[3];
	string url = argv[4];

	const int MAX_ATTEMPS = 3;
	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = argv[1];
	int SERVER_PORT = atoi(argv[2]);
	bool fin = false;
	bool primeraVez = true;
	Semaphore sem(0);

	// Protegemos frente señal
	signal(SIGINT, handle_sigalrm);

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_ADDRESS, SERVER_PORT);

    // Conectamos con el servidor. Probamos varias conexiones
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

    thread lec;			//Proceso de lectura
    thread esc;			//Proceso de escritura
    lec = thread(&lectura,ref(socket), socket_fd, ref(fin), ref(primeraVez), ref(sem));
    esc = thread(&escritura,ref(socket), url ,socket_fd, ref(fin), ref(primeraVez), ref(sem));

	esc.join();
	lec.join();

	socket.Close(socket_fd);
	exit(1);
}
