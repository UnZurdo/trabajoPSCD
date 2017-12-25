//*****************************************************************
// File:   Monitor.h
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Subasta.cpp -c
//*****************************************************************
#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H


#include <iostream>
#include <ctime>
#include <iomanip>
#include <queue>
#include "ImageDownloader.hpp"
#include "CImg.h"
#include "Gestor.h"
#include "Monitor.h"
#include "Subasta.h"

using namespace std;

class Administrador{
	Gestor* gestor;
	Subasta* subasta;
	Monitor* monitor;

    public:
        Administrador(Gestor* gestor, Subasta* subasta, Monitor* monitor);
        void mostrarEstado();
        void mostrarHistoria();
        void apagarServidor();

};


#endif