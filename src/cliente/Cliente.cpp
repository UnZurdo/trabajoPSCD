//*****************************************************************
// File:   Cliente.cpp
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
//*****************************************************************

//******************************************************************//
// SET de INSTRUCCIONES:  ESTADO,  AYUDA,  PUJAR <cantidad>,  PASO //
//****************************************************************//

#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "../librerias/Semaphore.h"
#include "../librerias/Socket.h"

using namespace std;
const int MESSAGE_SIZE = 4001;  //mensajes de no más 4000 caracteres
const char MENS_FIN[]="EXIT";
const char RECHAZADO[]="RECHAZADO";
// Informar de que se va a enviar datos a continuacion
const char URL[]="URL";

string url;

void escritura(Socket& socket, int socket_fd, bool& fin, Semaphore& sem){
	// Recibe la respuesta del servidor
    string aux;
    bool mensajeContinua=false;
    int read_bytes = socket.Recv(socket_fd, aux, MESSAGE_SIZE);
    // Se cierra el Socket
    if(aux==RECHAZADO){
    	int error_code = socket.Close(socket_fd);
	    if(error_code == -1){
			cerr << "Error cerrando el socket: " << strerror(errno) << endl;
	    }
	    cout << "Conexión RECHAZADA"<<endl;
    }
    string mensaje;

    while(mensaje != "SI" && mensaje != "NO") {
        cout << "Desea unirse a la subasta? ( SI/NO ): " << endl;
        getline(cin, mensaje);
    }

    if(mensaje=="SI"){
		cout <<"Gracias por participar"<<endl<< "Escriba \"EXIT\" para finalizar"<<endl << aux;
    }
    else{
    	cout << "Gracias por contactar." <<endl;
    	mensaje = MENS_FIN;
    }
    // Envía el mensaje
    int send_bytes = socket.Send(socket_fd, mensaje);
    if(send_bytes == -1){
		cerr << "Error al enviar datos: " << strerror(errno) << endl;
		// Se cierra el socket
		socket.Close(socket_fd);
		exit(1);
	}

    if(mensaje!=MENS_FIN){
	    // Buffer para almacenar la respuesta
		string buffer;
		bool mensajeContinua = false;
		do{
			mensaje="";
			// Si recibe fin del Servisdor
			if(buffer == MENS_FIN){
				mensajeContinua=true;
			}
			else if(buffer == URL){
				cout << "-- PUJA ganada --"<<endl;
				mensaje=url;
			}
			else {
				// Leer mensaje de la entrada estandar
				// Caso usuario no introduce nada, repetimo
				while(mensaje=="") {
					getline(cin, mensaje);
				}
			}

			if(!mensajeContinua){
				// Envía el mensaje
			    int send_bytes = socket.Send(socket_fd, mensaje);
			    cout << "ENVIADO: "<<mensaje<<endl;

			    if(send_bytes == -1){
					cerr << "Error al enviar datos: " << strerror(errno) << endl;
					// Se cierra el socket
					socket.Close(socket_fd);
					exit(1);
				}
			}

			if(mensaje != MENS_FIN){
			    // Recibe la respuesta del servidor
			    int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);

			    // Recibe el segundo mensaje
			    if(mensajeContinua){
    			    // Indica el fin del bucle
			    	mensaje=MENS_FIN;
			    }
			    cout << "RESPUESTA: " << buffer << endl;
			}
		} while(mensaje != MENS_FIN);
	}
}

//Función de protección frente señal
void handle_sigint(int signo){
	signal(SIGINT, handle_sigint);
}

int main(int argc, char* argv[]) {
	if (argc < 4){
		cout << "ERROR, inserte los parametros correctamente" << endl;
		exit(1);
	}

	const int MAX_ATTEMPS = 3;
	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = argv[1];
	int SERVER_PORT = atoi(argv[2]);
	bool fin = false;
	url = argv[3];
	Semaphore sem(0);

	// Protección frente señal
	signal(SIGINT, handle_sigint);

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_ADDRESS, SERVER_PORT);

    // Conexión con el servidor. Prueba varias conexiones
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

	esc.join();

	socket.Close(socket_fd);

    cout << "Cerrando el cliente..." << endl;
    sleep(5);
	exit(1);
}
