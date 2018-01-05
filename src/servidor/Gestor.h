//*****************************************************************
// File:   Gestor.h
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
    queue<Valla> q;
    bool fin;

    public:
        Gestor();
        void anyadirValla(Valla valla);
        // fin := true, establecido por el admin
        void apagar();
        // Bucle infinito en el que se muestran las imagenes siempre que haya disponibles 
        // en la cola, finaliza si fin := true, rechaza nuevas vallas y espera a que se terminen de 
        // mostrar las encoladas
        void iniciar(bool& fin);

};


#endif
