//******************************************************************
// File:   ServidorMulticliente.cpp
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
//*****************************************************************

#include "../librerias/Socket.h"
#include "../librerias/Semaphore.h"
#include <iostream>
#include <thread>
#include <sstream>
#include <cstring>
#include "Administrador.h"

#include <signal.h>
#include <stdio.h>
#include <unistd.h>


using namespace std;

const char MENS_FIN[]="EXIT";
const char RECHAZADO[]="RECHAZADO";
const char ACEPTADO[]="ACEPTADO";
const char ESTADO[]="ESTADO";
const char AYUDA[]="AYUDA";
const char PASO[]="PASO";
const char OCUPADO[]="OCUPADO";
// Informar de que se va a enviar datos a continuacion (se debe parsear el mensaje)
const char URL[]="URL";
const char PUJAR[]="PUJAR";

bool hayGanador = false;	// Indica si hay ganador en una subasta (true)
Semaphore aceptar(0);


//-------------------------------------------------------------
void recibir(Subasta& s, Socket& soc, int client_fd, string& msg, bool& fin, bool& out, string& estado){
	// Buffer para recibir el mensaje
	int length = 1000;
	char buffer[length];
	char *temp;
	char *temp2;
	int send_bytes;
	int puja = 0;
	bool primeraVez = true;
	bool ultimoMensaje = false;
	string msgAUX;

	if(fin) {
		out = true;
		msg = RECHAZADO;
		aceptar.signal();
	}
	// Confirma conexion
	else if(!fin){
		msg = estado;
		aceptar.signal();
	}
	// Envía ACEPTAR/RECHAZAR
	send_bytes = soc.Send(client_fd, msg);
	if(send_bytes == -1) {
		string mensError(strerror(errno));
		cerr << "Error al enviar datos: " + mensError + "\n";
		// Cierra los sockets
		exit(1);
	}

	// Recibe confirmación de que quiere unirse a la subasta
	int rcv_bytes = soc.Recv(client_fd,buffer,length);
	cout << "*BUFFER: "<<buffer<<endl;
	if (rcv_bytes == -1) {
		string mensError(strerror(errno));
		cerr << "Error al recibir datos: " + mensError + "\n";
		// Cierra los sockets
		soc.Close(client_fd);
	}
	//Recibe mensaje de fin
	if(strcmp(buffer,MENS_FIN)==0){
		out = true;
	}


	while(!out){

		msg="MENSAJE INVALIDO, vuelva a intentarlo\n";

		int rcv_bytes = soc.Recv(client_fd,buffer,length);
		cout << "*BUFFER: "<<buffer<<endl;
		if (rcv_bytes == -1) {
			string mensError(strerror(errno));
    		cerr << "Error al recibir datos: " + mensError + "\n";
			// Cierra los sockets
			soc.Close(client_fd);
		}
		//Recibe mensaje de fin
		if(strcmp(buffer,MENS_FIN)==0){
			msg="";
			out = true;
		}
		//Recibe solicitud de estado de la subasta
		else if(strcmp(buffer, ESTADO)==0){
			msg = "--ESTADO--\n";
			cout << msg;
		}
		else if(strcmp(buffer, PASO)==0){
			msg = "--TURNO PASADO--\n";
			cout << msg;
			puja=-1;
		}
		//Recibe mensaje de ayuda
		else if(strcmp(buffer,AYUDA)==0){
			msg = "\nEscriba \"EXIT\" para abandonar la subasta.\nPara mostrar el estado actual de la subasta escriba ESTADO.\nSi desea pujar escriba: PUJAR <cantidad>\n\n";
			cout << msg;
		}
		else{
			// Parseamos al entrada ante varios delimitadores
			temp = strtok(buffer, " ");
			temp2 = strtok(NULL, " \n");
			// Si el mensaje es válido
			if(temp && temp2){
				if(strcmp(temp,PUJAR)==0){
					puja = atoi(temp2);
					cout << "PUJA recibida de "<< puja<<" $"<<endl;
					bool valida=s.obtenerMonitor()->Pujar(puja, client_fd);
					if(!valida) {
						msg="--PUJA no aceptada--\n";
						cout << msg;
					}
					else {
						msg="--PUJA aceptada--\n";
						cout << msg;
					}
					puja = -2;
				}
			}
		}
		// Si no ha pujado envio puja vacía
		if(puja==0 || puja==-1){
			s.obtenerMonitor()->Pujar(puja, client_fd);
		}
		puja = 0;
		// Espera a que todos los clientes respondan
		s.siguienteTurno();

		if(msg!=""){
			// Si la subasta es nueva
			if(s.obtenerMonitor()->Pasar()) {
				// Si hay ganador espera a que se muestre la información
				s.obtenerMonitor()->bloquearSubasta();
				msg=msg + estado +"\n";
				// Si era la ultima subasta
				if(fin) {
					ultimoMensaje = true;
					// Copia mensaje antiguo
					msgAUX=msg;
					msg=MENS_FIN;

				}
			}
			// Añade información correspondiente a la siguiente ronda
			else msg= msg+ "\n" + s.obtenerMonitor()->estado();

			send_bytes = soc.Send(client_fd, msg);
			if(send_bytes == -1) {
				string mensError(strerror(errno));
				cerr << "Error al enviar datos: " + mensError + "\n";
				// Cierra los sockets
				exit(1);
			}
			//Borra el mensaje
			msg="";

			// Si el administador cierra la ubasta
			if(ultimoMensaje) {
				msg=msgAUX;
				// Deja de atender a más clientes
				send_bytes = soc.Send(client_fd, msg);
				//cout <<endl<<"---ENVIO: "<<msg<<endl;
				if(send_bytes == -1) {
					string mensError(strerror(errno));
					cerr << "Error al enviar datos: " + mensError + "\n";
					// Cierra los sockets
					exit(1);
				}
				out = true;
			}
		}
	}
}


void gestorSubasta(Socket& soc, Subasta& subasta, Gestor& gestor, string& estado, bool& fin){
	int length = 1000;
	char buffer[length];
	string estadoAux;
	estado="";

	while(!fin){
		// Reiniciar Subasta
		subasta.nuevo();

		// Inicializo nueva subasta
		subasta.iniciar(estadoAux);
		estado=estado+estadoAux;
		cout << estado;
		// SUBASTA BLOQUEADA MIENTRAS SE TRAMITA LA INFORMACION
		subasta.obtenerMonitor()->desbloquearSubasta();

		int user_id;
		// Finalizar Subasta ==> ESTADO: OCUPADO
		bool hayGanador = !(subasta.cerrarSubasta(ref(user_id), estado));

		// Informar ganador si lo hay y obtener datos url
		if(hayGanador){
			cout << "---> Hay ganador"<<endl;
			// Comprueba que aun sigue conectado
			if(subasta.obtenerMonitor()->esta(user_id)){
				// Pido URL al cliente, la recibo en el proceso de recibir
				string msg = URL;
				const char* message = msg.c_str();
				int send_bytes = soc.Send(user_id, message);
				if(send_bytes == -1) {
					string mensError(strerror(errno));
	    			cerr << "Error al enviar datos: " + mensError + "\n";
					// Cerramos los sockets
					exit(1);
				}
				// Recibo URL
				int rcv_bytes = soc.Recv(user_id,buffer,length);
				if (rcv_bytes == -1) {
					string mensError(strerror(errno));
		    		cerr << "Error al recibir datos: " + mensError + "\n";
					// Cerramos los sockets
					soc.Close(user_id);
				}
				string url_cliente = buffer;
				int d = subasta.obtenerDuracion();
				Valla valla;
				// creo un nombre con el que se mostrara la valla
				string path = "valla" + to_string(subasta.nVallas()) + ".jpg";
				crear(valla, url_cliente, path, d);
				gestor.anyadirValla(valla);
				cout << "valla añadida"<<endl<<endl;
			}
		}
	}
	cout << estado;
	// SUBASTA FINALIZADA
	subasta.obtenerMonitor()->desbloquearSubasta();
}

//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd, bool& fin, string& estado, Subasta& subasta) {
	subasta.obtenerMonitor()->Entrar(client_fd);

	string mensaje="";
	bool out = false;

	thread rec = thread(&recibir , ref(subasta), ref(soc), client_fd, ref(mensaje), ref(fin), ref(out), ref(estado));
	rec.join();

	soc.Close(client_fd);
	subasta.obtenerMonitor()->Salir(client_fd);
	cout << "->Fin SERVIR a cliente "<<client_fd<<endl;

}


void administrator(Socket& socket, int socket_fd, bool& fin, Administrador& admin){
	admin.iniciarAdmin(ref(fin));
	socket.Close(socket_fd);
	cout << "----Socket CLOSED----"<<endl;
	exit(1);
}


void handle_sigalrm(int signo){
	signal(SIGINT, handle_sigalrm);
}
//-------------------------------------------------------------
int main(int argc, char** argv) {
	if (argc != 2){
		cout << "ERROR, inserte los parametros correctamente" << endl;
		exit(1);
	}

	int max_connections = 100;
	bool fin = false;
	string estado;

	// Creo subasta inicial
	Subasta subasta;
	// Creo gestor
	Gestor gestor;
	// Creo modulo administrador
	Administrador admin(&gestor, &subasta);

	// Protegemos frente señal
	signal(SIGINT, handle_sigalrm);

	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = "localhost";
	int SERVER_PORT = atoi(argv[1]);

	thread administrador;		//Proceso administrador
	thread gestorP;				//Proceso gestor
	thread gestorP2;			//Proceso gestor2
	thread subastador;			//Proceso control de la subasta

    int client_fd;


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

	administrador = thread(&administrator, ref(socket), socket_fd, ref(fin), ref(admin));
	gestorP = thread(&Gestor::iniciar, ref(gestor));
	gestorP2 = thread(&Gestor::iniciar, ref(gestor));
	subastador = thread(&gestorSubasta, ref(socket), ref(subasta), ref(gestor), ref(estado), ref(fin));

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
		thread cliente = thread(&servCliente, ref(socket), client_fd, ref(fin), ref(estado), ref(subasta));
		cliente.detach();
		cout << "Nuevo cliente " + to_string(i);
		if(!fin) cout << " aceptado" << endl;
		else cout << " RECHAZADO" <<endl;
		++i;
	}

	//¿Qué pasa si algún thread acaba inesperadamente?
	gestorP.join();
	gestorP2.join();
	subastador.join();
	administrador.join();


    // Cerramos el socket del servidor
    error_code = socket.Close(socket_fd);
    if (error_code == -1) {
		string mensError(strerror(errno));
    	cerr << "Error cerrando el socket del servidor: " + mensError + "\n";
	}

	// Mensaje de despedida
	cout << "FIN PROGRAMA" << endl;

    return error_code;
}
//-------------------------------------------------------------
