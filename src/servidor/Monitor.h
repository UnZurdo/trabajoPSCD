//*****************************************************************
// File:   Monitor.h
// Author: Alberto García Hernández
// Date:   noviembre 2017
// Coms:   Ejercicio 5 de PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Monitor.cpp -c
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
    int id;
    mutex mtx;
    condition_variable ocupado;
    int clientList[MAX];

    public:
        Monitor(int min);
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
        void get_all_clients(int clients_fd[], int& n);
        // Falso si puja es menor que la actual
        bool Pujar(const int dinero, int id);

};


#endif