//*****************************************************************
// File:   Monitor.cpp
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
//*****************************************************************

#include "Monitor.h"

atomic <int> N(0);

int randomS(){
    srand (time(NULL));
    int random = rand() % 100 + 10;
    return random;
}

Monitor::Monitor(int min){
    this->nClientes=0;
    this->nPujasTotales=0;
    this->nPujas=0;
    this->nPujasValidas=0;
    this->nPASAR=0;
    this->finSubastaActual=false;
    this->siguiente=min;
    this->actual=min;
    this->minSecreto = min +randomS();
    this->id_ganador=-1;
    // Inicializo vector a 0
    for(int i = 0; i<MAX; ++i){
        this->clientList[i]=0;
    }
};

void Monitor::nuevo(int min){
    this->siguiente=min;
    this->actual=min;
    this->minSecreto = min +randomS();
    this->nPujas=0;
    this->nPujasValidas=0;
    this->nPASAR=0;
    this->finSubastaActual=false;
    this->id_ganador=-1;
}

int Monitor::pujaActual(){
    unique_lock<mutex> lck(mtx);
    return actual;
};

string Monitor::estado(){
    unique_lock<mutex> lck(mtx);
    ostringstream oss;
    if (nPujasValidas==0){
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



void Monitor::siguientePuja(){
    unique_lock<mutex> lck(mtx);
    while(nPujas<nClientes){
        esperar.wait(lck);
    }
    ++N;
    // EL ULTIMO EN DESPERTARSE
    if(N==nClientes){
        // SIGUIENTE RONDA
        nPujas=0;
        N=0;
        if(nPASAR==nClientes){
            finSubastaActual=true;
            cout << "--> FIN SUBASTA ACTUAL"<<endl;
        }
        else nPASAR=0;
        esperando_ultimo.notify_all();
    }
    else esperando_ultimo.wait(lck);
};

void Monitor::bloquearSubasta(){
    unique_lock<mutex> lck(mtx);
    bloqueado.wait(lck);

};

void Monitor::desbloquearSubasta(){
    bloqueado.notify_all();
};

int Monitor::getId(){
    unique_lock<mutex> lck(mtx);
    return id_ganador;
};

bool Monitor::esta(int client_fd){
    unique_lock<mutex> lck(mtx);
    bool esta=false;
    int i = 0;
    while(!esta && i < MAX){
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
    *n = j;
}

bool Monitor::Pasar(){
    return finSubastaActual;
};

// Falso si puja es menor que la actual
bool Monitor::Pujar(const int dinero, int id){
    unique_lock<mutex> lck(mtx);
    ++nPujasTotales;
    // Despierto a todos los que estaban esperando
    ++nPujas;
    cout << "num PUJAS: "<< nPujas<<endl;
    esperar.notify_all();
    if(dinero < siguiente){
        if(dinero==-1){
            // Si clientes PASAR
            ++nPASAR;
        }
        return false;
    }
    else{
        ++nPujasValidas;
        actual=dinero;
        siguiente=dinero+randomS();
        if(actual > minSecreto){
            id_ganador=id;
            return true;
        }
        else return false;
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
    esperar.notify_all();
    --nClientes;
    ocupado.notify_one();
};
