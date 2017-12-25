//*****************************************************************
// File:   Monitor.cpp
// Author: Alberto García Hernández
// Date:   noviembre 2017
// Coms:   Ejercicio 5 de PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Monitor.cpp -c
//*****************************************************************

#include "Monitor.h"


Monitor::Monitor(){
    this->nClientes=0;

};


void Monitor::Finalizar(){
    unique_lock<mutex> lck(mtx);
    while(nClientes!=0){
        ocupado.wait(lck);
    }

};


int Monitor::Entrar(){
  
};


void Monitor::Salir(){
    unique_lock<mutex> lck(mtx);
    --nClientes;
    ocupado.notify_one();
};

void shutdown();

