//*****************************************************************
// File:   Gestor.h
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
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
    queue<Valla> q;             // Cola de vallas
    bool fin;                   // Variable booleana que indica fin
    Semaphore s;
    Semaphore turno;
    Semaphore gestorCerrado;
    int contador;

public:

    /*
     * Pre: ---
     * Post: Crea un Gestor y lo inicializa
     */
    Gestor();

    /*
     * Pre: ---
     * Post: Añade una valla a cola
     */
    void anyadirValla(Valla valla);

    /*
     * Pre: ---
     * Post: Bloquea el gestor hasta que terminen las posibles
     *       vallas y luego lo cierra
     */
    void apagar();

    /*
     * Pre: ---
     * Post: Devuelve una cadena con información acerca de
     *       las vallas
     */
    string estado();

    /*
     * Pre: ---
     * Post: Va mostrando las vallas que haya disponibles en la cola. En
     *       el momento que fin está a true rechaza nuevas vallas y espera
     *       a que terminen
     */
    void iniciar();
};


#endif
