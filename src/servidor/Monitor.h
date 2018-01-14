//*****************************************************************
// File:   Monitor.h
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
//*****************************************************************
#ifndef MONITOR_H
#define MONITOR_H


#include <iostream>
#include <thread>
#include <cstring>
#include <sstream>
#include <mutex>
#include <condition_variable>
#include <iomanip>
#include <ctime>
#include <random>
#include <atomic>

using namespace std;

#define MAX 100

class Monitor{
    //Lista de clientes
    int nClientes;
    int nPASAR;
    bool finSubastaActual;
    int nPujas;
    int nPujasValidas;
    int nPujasTotales;
    // Mayor puja actual
    int actual;
    int siguiente;
    int id_ganador;
    
    mutex mtx;
    condition_variable esperar;
    condition_variable ocupado;

    condition_variable bloqueado;
    condition_variable esperando_ultimo;
    int clientList[MAX];

public:

    /*
     * Pre: ---
     * Post: Crea un monitor con un valor mínimo min para la subasta
     */
    Monitor(int min);

    /*
     * Pre: ---
     * Post: Actualiza los valores
     */
    void nuevo(int min);

    /*
     * Pre: ---
     * Post: Devuelve el valor de la última puja realizada por un cliente
     */
    int pujaActual();

    /*
     * Pre: ---
     * Post: Espera a que todos terminen de pujar para comenzar una nueva ronda
     *       de pujas
     */
    void siguientePuja();

    /*
     * Pre: ---
     * Post: Devuelve el número de clientes participando en la subasta
     */
    int clientes();

    /*
     * Pre: ---
     * Post: Devuelve el id del cliente ganador
     */
    int getId();

    /*
     * Pre: ---
     * Post: Devuelve true si y sólo si acaba la subasta actual
     */
    bool Pasar();

    /*
     * Pre: ---
     * Post: Devuelve una cadena con información acerca del estado
     *       actual de la subasta
     */
    string estado();

    /*
     * Pre: ---
     * Post: Espera que todos los clientes hayan sido cerrados
     */
    void Finalizar();

    /*
     * Pre: ---
     * Post: Añade un nuevo cliente y guarda su id en la lista clientList
     */
    void Entrar(int id);

    /*
     * Pre: ---
     * Post: Elimina el cliente id de la lista
     */
    void Salir(int id);

    /*
     * Pre: ---
     * Post: Devuelve true si y sólo si el cliente_fd exise en la Lista
     *       de clientes
     */
    bool esta(int client_fd);

    /*
     * Pre: ---
     * Post: Crea una lista con todos los clientes almecenados en la Lista
     *       clientList
     */
    void get_all_clients(int clients_fd[], int* n);

    /*
     * Pre: ---
     * Post: Devuelve true si y sólo si la puja introducida por el cliente
     *       es mayor a la mínimo exigida. En este caso se actualiza el valor
     *       de la nueva puja a superar. En caso contrario devuelve false
     */
    bool Pujar(const int dinero, int id);

    /*
     * Pre: ---
     * Post: Bloquea la subasta
     */
    void bloquearSubasta();

    /*
     * Pre: ---
     * Post: Desbloquea la subasta
     */
    void desbloquearSubasta();
};


#endif
