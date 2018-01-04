//*****************************************************************
// File:   Monitor.cpp
// Author: Alberto García Hernández
// Date:   noviembre 2017
// Coms:   Ejercicio 5 de PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Monitor.cpp -c
//*****************************************************************

#include "Monitor.h"

int randomT(){
    srand (time(NULL));
    int randomT = rand() % 100 + 10;
    return randomT;
}

Monitor::Monitor(){
    this->nClientes=0;
    this->nPujas=0;
    this->siguiente=0;
    this->actual=0;
    this->id=-1;
};

Monitor::Monitor(int min){
    this->nClientes=0;
    this->nPujas=0;
    this->siguiente=min+randomT();
    this->actual=min;
    this->id=-1;
};

int Monitor::pujaActual(){
    unique_lock<mutex> lck(mtx);
    return actual;
};

string Monitor::estado(){
    unique_lock<mutex> lck(mtx);
    ostringstream oss;
    oss <<"Puja maxima actual: "<<actual << "del cliente "<<id <<endl
    <<"Puja siguiente: "<<siguiente<<endl<<"Número de clientes participando"
    <<nClientes<<endl<< "Número de pujas: "<<nPujas<<endl;

    return oss.str();
};


int Monitor::siguientePuja(){
    unique_lock<mutex> lck(mtx);
    return  siguiente;
};

int Monitor::getId(){
    unique_lock<mutex> lck(mtx);
    return id;
};

// Falso si puja es menor que la actual
bool Monitor::Pujar(const int dinero, int id){
    unique_lock<mutex> lck(mtx);
    ++nPujas;
    if(dinero < siguiente){
        return false;
    }
    else{
        actual=siguiente;
        siguiente=siguiente+randomT();
        id=id;
        return true;
    }
};

int Monitor::clientes(){
    unique_lock<mutex> lck(mtx);
    return nClientes;
};


void Monitor::Finalizar(){
    unique_lock<mutex> lck(mtx);
    while(nClientes!=0){
        ocupado.wait(lck);
    }

};


void Monitor::Entrar(){
    unique_lock<mutex> lck(mtx);
    ++nClientes;
};


void Monitor::Salir(){
    unique_lock<mutex> lck(mtx);
    --nClientes;
    ocupado.notify_one();
};


