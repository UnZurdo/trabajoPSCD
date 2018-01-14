//*****************************************************************
// File:   Subasta.cpp
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
//*****************************************************************
#include "Subasta.h"
#include <ctime>
#include <cstdlib>

Semaphore Subasta::esperar(0);

int randomP(){
    srand (time(NULL));
    int random = rand() % 200 + 400;
    return random;
}

int randomT(){
    srand (time(NULL));
    int random = rand() % 6 + 2;
    return random;
}

int randomD(){
    srand (time(NULL));
    int random = rand() % 30 + 20;
    return random;
}


Subasta::Subasta(){
    this->fin=false;
    this->duracion=randomD();
    this->precioInicial=randomP();
    this->precioMinimo=randomP()+(randomP()/5);
    this->monitor= new Monitor(precioInicial);

    this->beneficios=0;
    this->nGanadores=0;
    this->nSubastas=1;
    this->nImagenes=0;
    this->tiempoTotal=duracion;
    this->tiempoMedio=tiempoTotal/nSubastas;
};


Subasta::Subasta(int duracion, int precioInicial, int precioMinimo){
    this->fin=false;
    this->duracion=duracion;
    this->precioInicial=precioInicial;
    this->precioMinimo=precioMinimo;
    this->monitor= new Monitor(precioInicial);

    this->beneficios=0;
    this->nGanadores=0;
    this->nSubastas=1;
    this->nImagenes=0;
    this->tiempoTotal=duracion;
    this->tiempoMedio=0;
};


void Subasta::nuevo(){
    this->fin=false;
    this->duracion=randomD();
    this->precioInicial=randomP();
    this->precioMinimo=randomP()+(randomP()/5);
    this->monitor->nuevo(precioInicial);

    this->nSubastas++;
    this->tiempoTotal+=duracion;
    this->tiempoMedio=tiempoTotal/nSubastas;

};


int Subasta::obtenerPujaMin(){
	return precioMinimo;
};


int Subasta::nVallas(){
    return nImagenes;
};


Monitor* Subasta::obtenerMonitor(){
    return monitor;
};


int Subasta::obtenerDuracion(){
	return duracion;
};


void Subasta::siguienteTurno(){
    monitor->siguientePuja();
    // Despierto cerrarsSubasta en caso finTurnos
    esperar.signal();
};


void Subasta::iniciar(string& estado){
    ostringstream oss;
    // SI FIN no creo subastas nuevas
    if(!fin){
    	oss <<"\n------- SUBASTA ABIERTA -------" <<endl <<"Valla Publicitaria de "<<duracion<<" segundos"<<endl
    	<< "Puja inicial en: " << precioInicial << " $"<<endl<<endl;
        estado=oss.str();
    }
};


void Subasta::finalizarSubasta(){
  fin=true;
  // Despierto cerrarsSubasta en caso FIN SUBASTA
};

// Precio al que se abre la subasta
int Subasta::pujaInicial(){
	return precioInicial;
};


bool Subasta::cerrarSubasta(int& user_id, string& estado){
    ostringstream oss;
    // Si aun no ha finalizado (todos han escrito PASAR) espero
    while(!monitor->Pasar()) esperar.wait();
    // Si hay ganador
	if(monitor->getId()!=-1 && monitor->pujaActual() > precioMinimo){
        if(!fin) oss <<endl<<"------- SUBASTA CONCLUIDA -------"<<endl; 
        else oss <<endl<<"------- SUBASTA CERRADA PERMANENTEMENTE -------"<<endl;
        oss <<"Ganador: "<<monitor->getId()<<" Puja cerrada a "<<monitor->pujaActual() <<"$"<<endl<<endl;
        estado=oss.str();
        cout << estado;
		beneficios+=monitor->pujaActual();
		++nImagenes;
		user_id = monitor->getId();
        // Añado nuevo ganador
        ganadores[nGanadores]=user_id;
        ++nGanadores;
        beneficios+=monitor->pujaActual();
		return false;
	}
	else{
        if(!fin) oss <<endl<<"------- SUBASTA CERRADA -------"<<endl;
        else oss <<endl<<"------- SUBASTA CERRADA PERMANENTEMENTE -------"<<endl;
		oss << "No hay ganador, puja minima de "<<precioMinimo<<" no superada."<<endl;
        estado=oss.str();
        cout << estado;
		return true;
	}
};


string Subasta::infoHistorica(){
    ostringstream oss;
    string lista_ganadores;
    for(int i = 0; i< nGanadores; ++i){
        lista_ganadores += to_string(ganadores[i])+", ";
    }
    oss << "Número de subastas realizadas: "<<nSubastas<<endl<<"Tiempo total ofertado: "<<tiempoTotal<<endl
    <<"Tiempo medio ofertado: "<<tiempoMedio<<endl<<"Beneficios obtenidos. "<<beneficios<<"$"<<endl
    << "Número de vallas contratadas: "<<nGanadores <<endl << "Lista de ganadores: " << lista_ganadores<<endl;
    return oss.str();
};
