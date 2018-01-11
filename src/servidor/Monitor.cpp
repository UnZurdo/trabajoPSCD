//*****************************************************************
// File:   Monitor.cpp
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Subasta.cpp -c
//*****************************************************************

#include "Monitor.h"

int randomS(){
    srand (time(NULL));
    int random = rand() % 100 + 10;
    return random;
}

Monitor::Monitor(int min){
    this->nClientes=0;
    this->nPujasTotales=0;
    this->nPujas=0;
    this->siguiente=min+randomS();
    this->actual=min;
    this->id_ganador=-1;
    // Inicializo vector a 0
    for(int i = 0; i<MAX; ++i){
        this->clientList[i]=0;
    }
};

void Monitor::nuevo(int min){
    this->siguiente=min+randomS();
    this->actual=min;
    this->nPujas=0;
    this->id_ganador=-1;
}

int Monitor::pujaActual(){
    unique_lock<mutex> lck(mtx);
    return actual;
};

string Monitor::estado(){
    unique_lock<mutex> lck(mtx);
    ostringstream oss;
    if (id_ganador == -1){
      oss << "\n Todavia no hay ninguna puja" << endl
      <<"Puja iniciada a: "<<actual<<endl;
    }
    else{
      oss <<"Puja maxima actual: "<<actual << " del cliente "<<id_ganador <<endl
      <<"Puja siguiente: "<<siguiente<<endl<<"Numero de clientes participando: "
      <<nClientes<<endl<< "Numero de pujas: "<<nPujasTotales<<endl<<endl;
    }
    return oss.str();
};


void Monitor::siguientePuja(int& nRondas){
    unique_lock<mutex> lck(mtx);
    while(nPujas<nClientes){
        esperar.wait(lck);
    }
    --nRondas;
};

int Monitor::getId(){
    unique_lock<mutex> lck(mtx);
    return id_ganador;
};

bool Monitor::esta(int client_fd){
    unique_lock<mutex> lck(mtx);
    bool esta=false;
    int i = 0;
    while(!esta && i < nClientes){
        if(clientList[i]==client_fd) esta = true;
        ++i;
    }
    return esta;
}

// PROBLEMA CON REFERENCIA
void Monitor::get_all_clients(int clients_fd[], int* n){
    int j = 0;
    for(int i = 0; i< MAX;++i){
        if(clientList[i]!=0){
            clients_fd[j]= clientList[i];
            ++j;
        }
    }
    //cout << "J: "<<j<<endl;
    *n = j;
}


// Falso si puja es menor que la actual
bool Monitor::Pujar(const int dinero, int id){
    unique_lock<mutex> lck(mtx);
    ++nPujasTotales;
    if(dinero < siguiente){
        return false;
    }
    else{
        actual=dinero;
        siguiente=dinero+randomS();
        id_ganador=id;
        return true;
    }
    // Despierto a todos los que estaban esperando
    ++nPujas;
    esperar.notify_all();
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


void Monitor::Entrar(int id){
    unique_lock<mutex> lck(mtx);
    int i = 0;
    // Añado al nuevo cliente
    while(clientList[i]!=0 && i < MAX){
        ++i;
    }
    clientList[i]=id;
    cout << "Entra: "<< clientList[i]<< " i: " << i << endl;

    ++nClientes;
};


void Monitor::Salir(int id){
    unique_lock<mutex> lck(mtx);
    int i = 0;
    // Busco cliente
    while(clientList[i]!=id && i < MAX){
        ++i;
    }
    // Lo borro
    cout << "Sale: "<< clientList[i]<<endl;
    clientList[i]=0;
    --nClientes;
    ocupado.notify_one();
};
