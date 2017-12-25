//*****************************************************************
// File:   Monitor.h
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Subasta.cpp -c
//*****************************************************************
#ifndef GESTOR_H
#define GESTOR_H


#include <iostream>
#include <ctime>
#include <iomanip>
#include <queue>
#include "ImageDownloader.hpp"
#include "CImg.h"
#include "Valla.h"

using namespace std;

class Gestor{
    Valla valla;
    queue<Valla> q;

    public:
        Gestor();
        void anyadirValla(Valla valla);
        void quitarValla(Valla valla);
        void mostrar();

};


#endif