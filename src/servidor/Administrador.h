//*****************************************************************
// File:   Administrador.h
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Administrador.cpp -c
//*****************************************************************
#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H


#include <iostream>
#include <ctime>
#include <iomanip>
#include <queue>
#include "Monitor.h"
#include "Gestor.h"
#include "Subasta.h"

using namespace std;

class Administrador{
	Gestor* gestor;
	Subasta* subasta;
	Monitor* monitor;

    public:
        Administrador(Gestor* gestor, Subasta* subasta, Monitor* monitor);
        void apagarServidor();
		void mostrarEstado();
		void mostrarHistoria();
        // Bucle infinito hasta END OF SERVICE en el que se leeran las ordenes
        // por la linea de comandos (mostrar Estado, Historia o apagar)
        void iniciarAdmin();

};


#endif
