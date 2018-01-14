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
	string mensaje;
	subasta->obtenerMonitor()->get_all_clients(clients_id, &N);
	string clientes;
    // Cuenta los clientes en un momento concreto
	for(int i = 0; i< N; ++i){
		clientes = clientes + to_string(clients_id[i]) +", ";
	}
    string mensajeAUX= "Numero de clientes: "+ to_string(N)+"\n";
    if((subasta->obtenerMonitor()->clientes()) > 0) {
        mensajeAUX=mensajeAUX+"Listado clientes: "+ clientes + "\n";
	}
	mensaje="\n--------------- ESTADO ---------------\n";
	mensaje+= subasta->obtenerMonitor()->estado() + gestor->estado() + mensajeAUX;
	mensaje+= "--------------------------------------\n";
    return mensaje;
};


string Administrador::mostrarHistoria(){
	string mensaje;
	mensaje="\n-------------- HISTÓRICO --------------\n";
    mensaje += subasta->infoHistorica() + gestor->estado();
    mensaje+="----------------------------------------\n";
    return mensaje;
};


void Administrador::iniciarAdmin(bool& fin){
	cout << "Para finalizar escribir \"EXIT\""<<endl;
	string msg;

	while(msg!="EXIT"){
		getline(cin, msg);
		if(msg=="HISTORICO"){
			cout <<endl<< mostrarHistoria()<<endl;
		}
		else if(msg=="ESTADO"){
			cout << mostrarEstado()<<endl;
		}
	}
	cout << "Closing server...."<<endl;
	fin=true;
	// Deja de crear subastas nuevas
	subasta->finalizarSubasta();
	// Espera a que terminen los clientes
	subasta->obtenerMonitor()->Finalizar();
	// Deja de añadir vallas
	gestor->apagar();
	// Ejecución bloqueada hasta que todos los clientes salgan
};
