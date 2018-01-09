//*****************************************************************
// File:   Monitor.h
// Date:   December 2017
// Coms:   TP6 PSCD
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


using namespace std;

#define MAX 100

class Monitor{

    //Lista de clientes
    
    int nClientes;
    int nPujas;
    // Mayor puja actual
    int actual; 
    int siguiente;
    int id_ganador;
    mutex mtx;
    condition_variable ocupado;
    int clientList[MAX];

    public:
        Monitor(int min);
        //Devuelve el mismo pero con valores actualizados
        void nuevo(int min);
        int pujaActual();
        int siguientePuja();
        int clientes();
        int getId();
        string estado();
        // En caso de finalizacion inesperada de cliente
        void Finalizar();
        // ++nClientes y guardo su id
        void Entrar(int id);
        // --nClientes y borro su id
        void Salir(int id);
        void iniciar();
        // Devuelve una lista con los id de todos los clientes actuales
        void get_all_clients(int clients_fd[], int* n);
        // Falso si puja es menor que la actual
        bool Pujar(const int dinero, int id);

};


#endif
