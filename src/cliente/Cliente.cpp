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
string url;
Semaphore ganador(0);

void enviarURL(Socket& socket, int socket_fd, bool& fin){
	while(!fin){
		string mensaje;
		//Espero a que me den permiso para enviar la url
		ganador.wait();
		if(esGanador) {
			mensaje= "URL " + url;
			// name= "NOMBRE " + name;
			int send_bytes = socket.Send(socket_fd, mensaje);
			if(send_bytes == -1){
				cout << "Error en el send del cliente" << endl;
				exit(0);
			}
		}
	}
}

void lectura(Socket& socket, int socket_fd, bool& fin, bool& primeraVez, Semaphore& sem){
	cout << "LECTURA"<<endl;
	int read_bytes;
	string buffer;

	read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
	if(read_bytes == -1){
		cout << "Error en el recv del cliente" << endl;
		exit(0);
	}
	if(buffer==RECHAZADO){
		fin=true;
	}
	cout << buffer <<endl;
	sem.signal();

	while(!fin){
		read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
		if(read_bytes == -1){
			cout << "Error en el recv del cliente" << endl;
			exit(0);
		}
		else {

			//OCUPADO ==> SERVER NO PUEDE RESPONDER DE MOMENTO

			if(buffer == "FIN"){
				fin = true;
				cout << "Final recibido del servidor" << endl;
			}
			else if(buffer == URL){
				esGanador = true;
				ganador.signal();
			}
			else{
				cout <<"RESPUESTA: "<< buffer << endl;
			}
		}
	}
	// Lo despierto para que salgfa del bucle
	ganador.signal();
}


void escritura(Socket& socket, int socket_fd, bool& fin, bool& primeraVez, Semaphore& sem){
	cout << "ESCRITURA"<<endl;
	int send_bytes;
	string mensaje;
	string name;

	sem.wait();
	while(!fin){
		mensaje="";
		primeraVez=false;
		//if(primeraVez == true){
		//	sem.wait();
		//	mensaje= "URL " + url;
		//	// name= "NOMBRE " + name;
			//send_bytes = socket.Send(socket_fd, mensaje);
			//if(send_bytes == -1){
			//	cout << "Error en el send del cliente" << endl;
			//	exit(0);
			//}
			//cout << "ENVIADO: "<<mensaje<<endl;
			//primeraVez=false;
		//}

		getline(cin, mensaje);
		// Caso usuario no introduce nada, repetimo
		while(mensaje=="") {
			//cout << "Que asiento desea reservar: ";
			getline(cin, mensaje);
		}
		cout << "ENVIADO: "<<mensaje<<endl;
		send_bytes = socket.Send(socket_fd, mensaje);
		//if(send_bytes == -1){
		//	cout << "Error en el send del cliente" << endl;
		//	exit(0);
		//}
		//cout << "TRAS ENVIO: "<<mensaje<<endl;
		if (mensaje == MENS_FIN){
				fin = true;
		}

		// 1) MENSAJE == AYUDA

		// 2) MENSAJE == ESTADO
	}
	if(primeraVez)cout << "CONEXION RECHAZADA"<<endl;


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
	url = argv[4];

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
    esc = thread(&escritura,ref(socket) ,socket_fd, ref(fin), ref(primeraVez), ref(sem));
    // Lanzo nuevo proceso encargado de enviar la URL
	thread enviar = thread(&enviarURL, ref(socket), socket_fd, ref(fin));

	esc.join();
	lec.join();
	enviar.join();

	cout << "Cerrando Cliente..."<<endl;

	socket.Close(socket_fd);
	exit(1);
}
