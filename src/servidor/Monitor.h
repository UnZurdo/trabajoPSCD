//*****************************************************************
// File:   Monitor.h
// Date:   december 2017
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
    int nPASAR;
    bool finSubastaActual;
    int nPujas;
    int nPujasTotales;
    // Mayor puja actual
    int actual; 
    int siguiente;
    int id_ganador;
    mutex mtx;
    condition_variable esperar;
    condition_variable ocupado;
    int clientList[MAX];

    public:
        Monitor(int min);
        //Devuelve el mismo pero con valores actualizados
        void nuevo(int min);
        int pujaActual();
        // Esperar a que todos terminen de pujar para comenzar nueva ronda de pujas
        // Si envia otro mensaje puja = 0
        // Al terminar ++Rondas
        void siguientePuja();
        int clientes();
        int getId();
        // Return true if finSubastaActual
        bool Pasar();
        string estado();
        // En caso de finalizacion inesperada de cliente
        void Finalizar();
        // ++nClientes y guardo su id
        void Entrar(int id);
        // --nClientes y borro su id
        void Salir(int id);
        void iniciar();
        // Devuelve true si el cliente sigue conectado
        bool esta(int client_fd);
        // Devuelve una lista con los id de todos los clientes actuales
        void get_all_clients(int clients_fd[], int* n);
        // Falso si puja es menor que la actual
        bool Pujar(const int dinero, int id);

};


#endif
