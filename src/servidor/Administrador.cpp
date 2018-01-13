//*****************************************************************
// File:   Administrador.cpp
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
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
    string MENSAJE= "Numero de clientes: "+ to_string(N)+"\n";
    if((subasta->obtenerMonitor()->clientes()) > 0) {
         MENSAJE=MENSAJE+"Listado clientes: "+ clientes + "\n";
	}
    return subasta->obtenerMonitor()->estado() + gestor->estado()+ MENSAJE;
};


string Administrador::mostrarHistoria(){
    return subasta->infoHistorica();
};


void Administrador::apagarServidor(){
    subasta->finalizarSubasta();
};


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
};
