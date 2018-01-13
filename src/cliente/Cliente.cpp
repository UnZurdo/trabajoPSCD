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
bool aux=false;
string url;
Semaphore ganador(0);


void escritura(Socket& socket, int socket_fd, bool& fin, Semaphore& sem){
	// Recibimos la respuesta del servidor
    string aux;
    bool mensajeContinua=false;
    int read_bytes = socket.Recv(socket_fd, aux, MESSAGE_SIZE);
    // Cerramos Socket
    if(aux==RECHAZADO){
    	int error_code = socket.Close(socket_fd);
	    if(error_code == -1){
			cerr << "Error cerrando el socket: " << strerror(errno) << endl;
	    }
	    cout << "Conexión RECHAZADA"<<endl;
    }
    string mensaje;
    cout << "¿Desea unirse a la subasta? ( SI/NO ): "<<endl;
    getline(cin, mensaje);

    if(mensaje=="SI" || mensaje=="si"){
		cout <<"Gracias por participar"<<endl<< "Escriba \"EXIT\" para finalizar"<<endl << aux;
    }
    else{
    	cout << "Gracias por contactar." <<endl;
    	mensaje = MENS_FIN;
    }
    // Enviamos el mensaje
    int send_bytes = socket.Send(socket_fd, mensaje);
    if(send_bytes == -1){
		cerr << "Error al enviar datos: " << strerror(errno) << endl;
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}

    if(mensaje!=MENS_FIN){
	    // Buffer para almacenar la respuesta
		string buffer;
		bool mensajeContinua = false;
		do{
			mensaje="";

			if(buffer == URL){
				cout << "PUJA ganada"<<endl;
				mensaje = url;
			}
			// Si recibo fin del Servisdor
			if(buffer == MENS_FIN){
				mensajeContinua=true;
			}
			else {
				// Leer mensaje de la entrada estandar
				// Caso usuario no introduce nada, repetimo
				while(mensaje=="") {
					getline(cin, mensaje);
				}
			}

			if(!mensajeContinua){
				// Enviamos el mensaje
			    int send_bytes = socket.Send(socket_fd, mensaje);
			    cout << "ENVIADO: "<<mensaje<<endl;

			    if(send_bytes == -1){
					cerr << "Error al enviar datos: " << strerror(errno) << endl;
					// Cerramos el socket
					socket.Close(socket_fd);
					exit(1);
				}
			}

			if(mensaje != MENS_FIN){
			    // Recibimos la respuesta del servidor
			    int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
			    // Mostramos la respuesta

			    // Recibo el segundo mensaje
			    if(mensajeContinua){
			    	// Salgo dle bucle
			    	mensaje=MENS_FIN;
			    }
			    cout << "RESPUESTA: " << buffer << endl;
			}
		} while(mensaje != MENS_FIN);
	}
}


void handle_sigalrm(int signo){
	signal(SIGINT, handle_sigalrm);
}

int main(int argc, char* argv[]) {
	if (argc != 4){
		cout << "ERROR, inserte los parametros correctamente" << endl;
		exit(1);
	}

	url = argv[3];

	const int MAX_ATTEMPS = 3;
	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = argv[1];
	int SERVER_PORT = atoi(argv[2]);
	bool fin = false;
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

    thread esc;			//Proceso de escritura
    esc = thread(&escritura,ref(socket) ,socket_fd, ref(fin), ref(sem));
    // Lanzo nuevo proceso encargado de enviar la URL
	//thread enviar = thread(&enviarURL, ref(socket), socket_fd, ref(fin));

	esc.join();

	socket.Close(socket_fd);

    cout << "Cerrando el cliente..." << endl;
    sleep(5);
	exit(1);
}
