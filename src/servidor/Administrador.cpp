//*****************************************************************
// File:   Administrador.cpp
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Administrador.cpp -c
//*****************************************************************
#include "Administrador.h"

Administrador::Administrador(Gestor* gestor, Subasta* subasta, Monitor* monitor){
      this->gestor = gestor;
      this->subasta = subasta;
      this->monitor = monitor;
};


void Administrador::mostrarEstado(){
    cout << monitor->estado() << endl;
};


void Administrador::mostrarHistoria(){
    cout << subasta->infoHistorica() << endl;
};


void Administrador::apagarServidor(){
    subasta->finalizarSubasta();
};


// Bucle infinito hasta END OF SERVICE en el que se leeran las ordenes
// por la linea de comandos (mostrar Estado, Historia o apagar)
void Administrador::iniciarAdmin(){
	/*
	
	UTILIZAR JUNTO PROCEDIMIENTO ADMINISTRADOR SERVER

	*/
};
