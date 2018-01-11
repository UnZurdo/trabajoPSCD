//*****************************************************************
// File:   Subasta.h
// Date:   december 2017
// Coms:   TP6 PSCD
//*****************************************************************
#ifndef SUBASTA_H
#define SUBASTA_H

#include <iostream>
#include <iomanip>
#include "Valla.h"
#include "Monitor.h"
#include <signal.h>
#include <unistd.h>
#include "../librerias/Semaphore.h"

using namespace std;



class Subasta{

    Monitor* monitor;
    int nTurnos;
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

    static Semaphore esperar;


    public:
        // Crea una nueva Subasta con valores generados aleatoriamente
        Subasta();
        // Crea una nueva Subasta con valores establecidos
        Subasta(int nTurnos, int duracion, int precioInicial, int precioMinimo);

        // Sobrescribo datos de la subasta actual con los de una nueva
        void nuevo();
        // Acceso al monitor de Subasta
        Monitor* obtenerMonitor();
        // Precio PRIVADO minimo necesario para vender valla
        int obtenerPujaMin();
        // Tiempo que se mostrara la imagen
        int obtenerDuracion();
        // Devuelve el numero de vallas ofertadas hasta el momento
        int nVallas();
        // Precio al que se abre la subasta
        int pujaInicial();
        // Aumentar precio para siguiente puja
        int siguientePuja();
        // Si !fin ==> Inicializa temporizador y muestra estado
        void iniciar(string& estado);
        // Devuelve la duracion de la ronda actual en segundos
        int obtenerDuracionSubasta();
        // Espera a que todos los clientes respondan y comprueba si fin
        void siguienteTurno();
        // Actualiza datos, guarda datos ganador, los encola en el GESTOR de VALLAS
        // y genera una nueva suabsta
        // Parcial: si no hay ganador
        bool cerrarSubasta(int& user_id, string& estado);
        // Fin := true, no se realizaran nuevas subastas y esperara a que finalice
        // la actual.
        void finalizarSubasta();
        string infoHistorica();

        // Funcion AUX Subasta
        // Señal ALARM insegura para reiniciar la Subasta
        // Se inicializa desde el comienzo y se reinicia infinitamente hasta que se cierre
        static void handle_timer(int signo);

};


#endif
