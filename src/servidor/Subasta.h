//*****************************************************************
// File:   Subasta.h
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
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
    int nTurnos;            // Contador de turnos
    int duracion;           // Tiempo que dura la subasta
    int precioInicial;      // Precio en el que se comienza la subasta
    int precioMinimo;       // Precio mínimo a superar

    // Info Historica
    int beneficios;         // Beneficios totales
    int nSubastas;          // Contador de subastas hechas
    int nImagenes;          // Contador de imágenes mostradas
    int tiempoTotal;        // Total de tiempo ofertado
    int tiempoMedio;        // Media aritmética del tiempo ofertado

    bool fin;   // Indica el fin de subasta (True)

    static Semaphore esperar;   // Semáforo para controlar los turnos


public:
    /*
     * Pre: ---
     * Post: Crea una nueva Subasta con valores generados aleatoriamente
     */
    Subasta();

    /*
     * Pre: ---
     * Post: Crea una nueva Subasta con valores establecidos
     */
    Subasta(int duracion, int precioInicial, int precioMinimo);

    /*
     * Pre: ---
     * Post: Sobrescribe datos de la subasta actual con los de una nueva
     */
    void nuevo();

    /*
     * Pre: ---
     * Post: Acceso al monitor de Subasta
     */
    Monitor* obtenerMonitor();

    /*
     * Pre: ---
     * Post: Precio PRIVADO minimo necesario para vender valla
     */
    int obtenerPujaMin();

    /*
     * Pre: ---
     * Post: Tiempo que se mostrará la imagen
     */
    int obtenerDuracion();

    /*
     * Pre: ---
     * Post: Devuelve el número de vallas ofertadas hasta el momento
     */
    int nVallas();

    /*
     * Pre: ---
     * Post: Devuelve el precio al que se abre la subasta
     */
    int pujaInicial();

    /*
     * Pre: ---
     * Post: Aumenta precio para siguiente puja
     */
    int siguientePuja();

    /*
     * Pre: ---
     * Post: Si fin está en false, inicializa el temporizador y muestra estado
     */
    void iniciar(string& estado);

    /*
     * Pre: ---
     * Post: Espera a que todos los clientes respondan y comprueba si fin
     */
    void siguienteTurno();

    /*
     * Pre: ---
     * Post:
     */

    // Actualiza datos, guarda datos ganador, los encola en el GESTOR de VALLAS
    // y genera una nueva suabsta
    // Parcial: si no hay ganador
    bool cerrarSubasta(int& user_id, string& estado);

    /*
     * Pre: ---
     * Post:
     */
    // Terminar subasta
    // Fin := true, no se realizaran nuevas subastas y esperara a que finalice
    // la actual.
    void finalizarSubasta();

    /*
     * Pre: ---
     * Post: Devuelve una cadena que contiene información acerca de las
     *       subastas desde el momento que comenzó el programa
     */
    string infoHistorica();

    /*
     * Pre: ---
     * Post:
     */
    // Funcion AUX Subasta
    // Señal ALARM insegura para reiniciar la Subasta
    // Se inicializa desde el comienzo y se reinicia infinitamente hasta que se cierre
    static void handle_timer(int signo);
};


#endif
