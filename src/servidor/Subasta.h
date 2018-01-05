//*****************************************************************
// File:   Subasta.h
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
#include "Valla.h"
#include "Monitor.h"

using namespace std;



class Subasta{

    Monitor* monitor;
    int tInicial;
    int tiempoRestante;
    int duracion;
    int precioInicial;
    int precioMinimo;

    // Info Historica
    int beneficios;
    int nSubastas;
    int nImagenes;
    int tiempoTotal;
    int tiempoMedio;
    // Terminar subasta
    bool fin;


    public:
        // Crea una nueva Subasta con valores generados aleatoriamente
        Subasta();
        // Crea una nueva Subasta con valores establecidos
        Subasta(int tInicial, int duracion, int precioInicial, int precioMinimo);
        // Sobrescribo datos de la subasta actual con los de una nueva
        void nuevo(int min);
        // Precio PRIVADO minimo necesario para vender valla
        int obtenerPujaMin();
        // Tiempo que se mostrara la imagen
        int obtenerDuracion();
        // Devuelve true si el tiempo de la subasta se ha agotado
        bool finTiempo();
        // Fecha final en la que se cerrara la subasta
        int cierreSubasta();
        // Precio al que se abre la subasta
        int pujaInicial();
        // Aumentar precio para siguiente puja
        int siguientePuja();
        // Inicializa temporizador y muestra estado
        void iniciar();
        // Actualiza datos, guarda datos ganador, los encola en el GESTOR de VALLAS
        // y genera una nueva suabsta
        // Parcial: si no hay ganador
        bool cerrarSubasta(int& user_id);
        // Fin := true, no se realizaran nuevas subastas y esperara a que finalice
        // la actual.
        void finalizarSubasta();
        string infoHistorica();

};


#endif
