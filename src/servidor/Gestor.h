//*****************************************************************
// File:   Gestor.h
// Date:   december 2017
// Coms:   TP6 PSCD
//*****************************************************************
#ifndef GESTOR_H
#define GESTOR_H


#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
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
    Semaphore turno;
    int contador;

    public:
        // Defino constructor por defecto
        Gestor();
        void anyadirValla(Valla valla);
        // fin := true, establecido por el admin
        void apagar();
        // Muestra informacion sobre el estado actual
        string estado();
        // Bucle infinito en el que se muestran las imagenes siempre que haya disponibles
        // en la cola, finaliza si fin := true, rechaza nuevas vallas y espera a que se terminen de
        // mostrar las encoladas
        void iniciar();

};


#endif
