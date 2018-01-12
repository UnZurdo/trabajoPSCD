//*****************************************************************
// File:   Administrador.cpp
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Administrador.cpp -c
//*****************************************************************
#include "Administrador.h"

Administrador::Administrador(Gestor* gestor, Subasta* subasta){
  this->gestor = gestor;
  this->subasta = subasta;
};


string Administrador::mostrarEstado(){
	int clients_id[MAX];
	int N;
	subasta->obtenerMonitor()->get_all_clients(clients_id, &N);
	string clientes;
	for(int i = 0; i< N; ++i){
		clientes = clientes + to_string(clients_id[i]) +", ";
	}
    string MENSAJE= "Numero de clientes: "+ to_string(N);
    if((subasta->obtenerMonitor()->clientes()) > 0) {
         MENSAJE=MENSAJE+"\nListado clientes: "+ clientes + "\n";
	}
    return subasta->obtenerMonitor()->estado() + gestor->estado()+ MENSAJE;
};


string Administrador::mostrarHistoria(){
    return subasta->infoHistorica();
};


void Administrador::apagarServidor(){
    subasta->finalizarSubasta();
};


// Bucle infinito hasta END OF SERVICE en el que se leeran las ordenes
// por la linea de comandos (mostrar Estado, Historia o apagar)
void Administrador::iniciarAdmin(bool& fin){
	cout << "Para finalizar escribir \"EXIT\""<<endl;
	string msg;

	while(msg!="EXIT"){
		getline(cin, msg);
		if(msg=="HISTORICO"){
			cout << mostrarHistoria();
		}
		else if(msg=="ESTADO"){
			cout << mostrarEstado();
		}
	}

	cout << "Closing server...."<<endl;
	fin=true;
	// NO CREO SUBASTAS NUEVAS
	subasta->finalizarSubasta();
	// ESPERO A QUE TERMINEN CLIENTES
	subasta->obtenerMonitor()->Finalizar();
	// NO AÑADO MAS VALLAS
	gestor->apagar();
	// Ejecucion bloqueada hasta que todos los clientes salgan
	// Echarlos????????????
};
