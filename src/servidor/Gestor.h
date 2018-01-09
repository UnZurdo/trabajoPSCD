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
#include <string>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <queue>
#include "../librerias/ImageDownloader.hpp"
#include "../librerias/Semaphore.h"
#include "../librerias/CImg.h"
#include "Valla.h"

using namespace std;
using namespace cimg_library;

class Gestor{
    queue<Valla> q;
    bool fin;
    Semaphore s;
    int contador;

    public:
        // Defino constructor por defecto
        Gestor();
        void anyadirValla(Valla valla);
        // fin := true, establecido por el admin
        void apagar();
        // Bucle infinito en el que se muestran las imagenes siempre que haya disponibles 
        // en la cola, finaliza si fin := true, rechaza nuevas vallas y espera a que se terminen de 
        // mostrar las encoladas
        void iniciar();

};


#endif
