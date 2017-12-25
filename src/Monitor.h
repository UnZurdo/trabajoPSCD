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
#include <mutex>
#include <condition_variable>
#include <iomanip>


using namespace std;

class Monitor{

    int nClientes;
    mutex mtx;
    condition_variable ocupado;

    // ? Señal del administrador para finalizar el gestor
    bool apagar;

    public:
        Monitor();
        // En caso de finalizacion inesperada de cliente
        void Finalizar();
        int Entrar();
        void Salir();
        // Administrador apaga servidor
        void shutdown();
};


#endif