//*****************************************************************
// File:   Monitor.h
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Subasta.cpp -c
//*****************************************************************
#ifndef SUBASTA_H
#define SUBASTA_H


#include <iostream>
#include <ctime>
#include <iomanip>
#include "Fecha.h"
#include "Gestor.h"
#include "Valla.h"

using namespace std;

struct Subasta;
// Crea una nueva Subasta con valores generados aleatoriamente
void crear(Subasta& s);
// Precio PRIVADO minimo necesario para vender valla
int obtenerPujaMin(const Subasta& s);
// Tiempo que se mostrara la imagen 
int obtenerDuracion(const Subasta& s);
// Fecha final en la que se cerrara la subasta
int cierreSubasta(const Subasta& s);
// Precio al que se abre la subasta
int precioInicial(const Subasta& s);
// Aumentar precio para siguiente puja
int siguientePuja(const Subasta& s);
// Actualiza datos, guarda datos ganador, los encola en el GESTOR de VALLAS
// y genera una nueva suabsta
void cerrarSubasta(Subasta& s);


struct Subasta{
    friend void crear(Subasta& s);
    friend int obtenerPujaMin(const Subasta& s);
    friend int obtenerDuracion(const Subasta& s);
    friend int cierreSubasta(const Subasta& s);
    friend int precioInicial(const Subasta& s);
    friend int siguientePuja(const Subasta& s);
    friend void cerrarSubasta(Subasta& s);


    private:
        // Imagenes
        Valla valla;
        //Fecha
        Fecha fecha;
        int duracion;
        int precioInicial;
        int precioMinimo;
        // Mayor puja actual
        int precioActual; 
        int siguientePuja;
};


#endif