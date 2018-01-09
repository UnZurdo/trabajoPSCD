//******************************************************************
// File:   ServidorMulticliente.cpp
// Author: PSCD-Unizar
// Date:   Noviembre 2015
// Coms:   Ejemplo de servidor multicliente con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante "make".
//*****************************************************************

#include "../librerias/Socket.h"
#include "../librerias/Semaphore.h"
#include <iostream>
#include <thread>
#include <sstream>
#include <cstring> //manejo de cadenas tipo C
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
const char OCUPADO[]="OCUPADO";
// Informar de que se va a enviar datos a continuacion (se debe parsear el mensaje)
const char NOMBRE[]="NOMBRE";
const char URL[]="URL";
const char PUJAR[]="PUJAR";

// Mejor no global
string url_cliente;
bool hayGanador = false;
Semaphore hayMensaje(0);
Semaphore esperarURL(0);
Semaphore aceptar(0);


// Envia mensaje a todos los clientes
void informar_all(Subasta& s, Socket& soc, string msg){
	int clients_id[MAX];
	int N;
	s.obtenerMonitor()->get_all_clients(clients_id, &N); /////////////////////////////////////////////////////
	for(int i = 0; i< N; ++i){
		cout << "Client: "<<clients_id[i] <<endl;
		const char* message = msg.c_str();
		int send_bytes = soc.Send(clients_id[i], message);
		if(send_bytes == -1) {
			string mensError(strerror(errno));
			cerr << "Error al enviar datos: " + mensError + "\n";
			// Cerramos los sockets
			exit(1);
		}
	}
	cout << "nClientes: "<<N<<endl;
}

//-------------------------------------------------------------
void recibir(Subasta& s, Socket& soc, int client_fd, string& msg, bool& fin, bool& out){
	// Buffer para recibir el mensaje
	int length = 100;
	char buffer[length];
	char *temp;
	char *temp2;
	//bool primeraVez = true;

	// Esperar hasta enviar ACEPTAR / RECHAZA conexion 
	aceptar.wait();

	while(!out && !fin){
		msg="";
		int rcv_bytes = soc.Recv(client_fd,buffer,length);
		if(strcmp(buffer, "ACK")!=0)cout << "BUFFER: "<<buffer<<endl;
		if (rcv_bytes == -1) {
			string mensError(strerror(errno));
    		cerr << "Error al recibir datos: " + mensError + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
		}
		if(strcmp(buffer, MENS_FIN)==0){
			out = true;
			// Despiero a enviar
			hayMensaje.signal();
		}
		else if(strcmp(buffer, ESTADO)==0){
			msg = s.obtenerMonitor()->estado();
			hayMensaje.signal();
		}
		else if(strcmp(buffer,  AYUDA)==0){
			msg = "Escriba \"EXIT\" para abandonar la subasta.\nPara mostrar el estado actual de la subasta escriba ESTADO.\nSi desea pujar escriba: PUJAR <cantidad>\n";
			hayMensaje.signal();
		}
		else{
			// Parseamos al entrada ante varios delimitadores
			temp = strtok(buffer, " ");
			temp2 = strtok(NULL, " \n");
			// Valido mensaje
			if(temp && temp2){
				if(strcmp(temp,PUJAR)==0){
					int puja = atoi(temp2);
					cout << "PUJA recibida de "<< puja<<"€"<<endl;
					bool valida=s.obtenerMonitor()->Pujar(puja, client_fd);
					if(!valida) {
						msg="PUJA no aceptada " + s.obtenerMonitor()->estado();
						cout << msg;
					}
					else {
						string info= s.obtenerMonitor()->estado();
						msg="PUJA aceptada ";
						informar_all(s, soc, info);
						msg = msg + info;
						cout << msg;
						hayMensaje.signal();
						
					}
					// Enviar puja actual a todos los usuarios (Monitor)
					// void informar_all(Subasta& s, Socket& soc)
				}
				else if(strcmp(temp,URL)==0){
					// Actualiza info concursante
					url_cliente = temp2;
					// Despierto al gestorSubasta
					esperarURL.signal();
				}
			}

		}

	}

}

//-------------------------------------------------------------
void enviar(Subasta& s, Socket& soc, int client_fd, string& msg, bool& fin, bool& out){
	char* messageAUX;
	int send_bytes;
	bool primeraVez = true;
	// else messageAUX = ACEPTADO;

	while (!out) {
		if(fin) {
			out = true;
			msg = RECHAZADO;
			aceptar.signal();
		}
		// Confirmo conexion
		else if(!fin && primeraVez){
			primeraVez=false;
			msg= ACEPTADO;
			msg = msg + "\n" + s.obtenerMonitor()->estado();
			//out = true;
			aceptar.signal();
		}

		if(msg!=""){
			const char* message = msg.c_str();
			send_bytes = soc.Send(client_fd, message);
			if(send_bytes == -1) {
				string mensError(strerror(errno));
    			cerr << "Error al enviar datos: " + mensError + "\n";
				// Cerramos los sockets
				exit(1);
			}
			// Envio primer mensaje ACEPTO /RECHAZO conexion y espero al siguiente
			msg="";
			// Tas enviar el mensaje de confirmacion espero a que haya que enviar mas mensajes
			hayMensaje.wait();
		}
	}
	// Desbloqueo espera:  recv() del cliente
	if(out && !fin && !primeraVez){
		msg="FIN";
		const char* message = msg.c_str();
		send_bytes = soc.Send(client_fd, message);
		cout << "Cliente "<< client_fd <<" ha pedido salir"<<endl;
		if(send_bytes == -1) {
			string mensError(strerror(errno));
			cerr << "Error al enviar datos: " + mensError + "\n";
			// Cerramos los sockets
			exit(1);
		}
	}
}

/*

	IMPLEMENTAR ESTADOS??	

*/
void gestorSubasta(Socket& soc, Subasta& subasta, Gestor& gestor, bool& fin){
	string estado;

	while(!fin){
		// Inicializo nueva subasta
		subasta.iniciar(estado);
		// Informmar ALL ha comenzado
		informar_all(subasta, soc, estado);

		int user_id;
		// Finalizar Subasta ==> ESTADO: OCUPADO
		bool hayGanador = !(subasta.cerrarSubasta(user_id, estado));
		// Informmar ALL ha finalizado
		informar_all(subasta, soc, estado);

		// Informar ganador si lo hay y obtener datos url
		if(hayGanador){
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


			// Espero ha que el cliente me envie la URL
			esperarURL.wait();
			int d = subasta.obtenerDuracionSubasta();
			Valla valla;
			// creo un nombre con el que se mostrara la valla
			string path = "valla" + to_string(subasta.nVallas());
			cout << "Nueva Valla: "<< url_cliente << "   path"<<path<<endl;
			crear(valla, url_cliente, path, d);
			gestor.anyadirValla(valla);
			cout << "valla anadida"<<endl<<endl;
		}

		// Informmar ALL ha nueva Subasta ==> ESTADO: DISPONIBLE
		// Reiniciar Subasta
		subasta.nuevo();
	}

}

//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd, bool& fin,  Subasta& subasta) {
	subasta.obtenerMonitor()->Entrar(client_fd);

	string mensaje="";
	bool out = false;

	
	// Enviar puja actual a todos los usuarios (Monitor)
	thread rec = thread(&recibir , ref(subasta), ref(soc), client_fd, ref(mensaje), ref(fin), ref(out));
	thread env = thread(&enviar , ref(subasta), ref(soc), client_fd, ref(mensaje), ref(fin), ref(out));

	rec.join();
	env.join();

	soc.Close(client_fd);
	subasta.obtenerMonitor()->Salir(client_fd);

}


void administrator(Socket& socket, int socket_fd, bool& fin, Administrador& admin){
	admin.iniciarAdmin(fin);
	socket.Close(socket_fd);
	exit(1);
}

//-------------------------------------------------------------
int main(int argc, char** argv) {
	if (argc != 2){
		cout << "ERROR, inserte los parametros correctamente" << endl;
		exit(1);
	}

	int max_connections = 100;
	bool fin = false;

	// Creo subasta inicial
	Subasta subasta;
	// Creo gestor
	Gestor gestor;
	// Creo modulo administrador
	Administrador admin(&gestor, &subasta);


	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = "localhost";
	int SERVER_PORT = atoi(argv[1]);

	thread administrador;		//Proceso administrador
	thread gestorP;				//Proceso gestor
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
	subastador = thread(&gestorSubasta, ref(socket), ref(subasta), ref(gestor), ref(fin));


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
		thread cliente = thread(&servCliente, ref(socket), client_fd, ref(fin), ref(subasta));
		cliente.detach();
		cout << "Nuevo cliente " + to_string(i);
		if(!fin) cout << " aceptado" << endl;
		else cout << " RECHAZADO" <<endl;
		++i;
	}

	//¿Qué pasa si algún thread acaba inesperadamente?
	subasta.obtenerMonitor()->Finalizar();
	administrador.join();
	gestorP.join();
	subastador.join();


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
