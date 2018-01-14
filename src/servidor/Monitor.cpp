//*****************************************************************
// File:   Monitor.cpp
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
//*****************************************************************

#include "Monitor.h"

// Variable auxiliar para contabilizar el orden en el que se despiertan los procesos de 
// la funcion siguientePuja
int N=0;

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
    this->id_ganador=-1;
    // Inicializa el vector a 0
    for(int i = 0; i<MAX; ++i){
        this->clientList[i]=0;
    }
};

void Monitor::nuevo(int min){
    this->siguiente=min;
    this->actual=min;
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
      oss << "Todavia no hay ninguna puja" << endl
      <<"Puja iniciada a: "<<actual<<endl;
    }
    else{
      oss <<"Puja maxima actual: "<<actual << " del cliente "<<id_ganador <<endl
      <<"Puja siguiente: "<<siguiente<<endl<<"Número de clientes participando: "
      <<nClientes<<endl<< "Número de pujas actuales: "<<nPujasValidas<<endl
      << "Número de pujas totales: "<<nPujasTotales<<endl<<endl;
    }
    return oss.str();
};



void Monitor::siguientePuja(){
    unique_lock<mutex> lck(mtx);
    while(nPujas<nClientes){
        esperar.wait(lck);
    }
    ++N;
    // El último en despertarse
    if(N==nClientes){
        // Siguiente ronda
        nPujas=0;
        N=0;
        // Si todos los clientes han decidido PASAR
        if(nPASAR==nClientes){
            finSubastaActual=true;
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
    unique_lock<mutex> lck(mtx);
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


void Monitor::get_all_clients(int clients_fd[], int* n){
    unique_lock<mutex> lck(mtx);
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
    unique_lock<mutex> lck(mtx);
    return finSubastaActual;
};


bool Monitor::Pujar(const int dinero, int id){
    unique_lock<mutex> lck(mtx);
    // Despierta a todos los que estaban esperando
    ++nPujas;
    esperar.notify_all();
    if(dinero < siguiente){
        if(dinero==-1){
            // Si clientes PASAR
            ++nPASAR;
        }
        return false;
    }
    else{
        actual=dinero;
        siguiente=dinero+randomS()+(dinero/5);
        id_ganador=id;
        ++nPujasValidas;
        ++nPujasTotales;
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


void Monitor::Entrar(int id){
    unique_lock<mutex> lck(mtx);
    int i = 0;
    // Añado al nuevo cliente
    while(clientList[i]!=0 && i < MAX){
        ++i;
    }
    clientList[i]=id;
    ++nClientes;
};


void Monitor::Salir(int id){
    unique_lock<mutex> lck(mtx);
    int i = 0;
    // Busca cliente
    while(clientList[i]!=id && i < MAX){
        ++i;
    }
    // Lo borra
    clientList[i]=0;
    esperar.notify_all();
    --nClientes;
    ocupado.notify_one();
};
