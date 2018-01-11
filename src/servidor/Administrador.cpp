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
    return subasta->obtenerMonitor()->estado();
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
	subasta->finalizarSubasta();
	gestor->apagar();
	// Ejecucion bloqueada hasta que todos los clientes salgan
	// Echarlos????????????
	subasta->obtenerMonitor()->Finalizar();
};
