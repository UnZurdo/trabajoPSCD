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

// Crea una nueva Subasta con valores generados aleatoriamente
Subasta::Subasta(){
    this->fin=false;
    this->duracion=randomD();
    this->precioInicial=randomP();
    this->precioMinimo=randomP()+(randomP()/5);
    this->monitor= new Monitor(precioInicial);

    this->beneficios=0;
    this->nSubastas=1;
    this->nImagenes=0;
    this->tiempoTotal=duracion;
    this->tiempoMedio=tiempoTotal/nSubastas;
};

// Crea una nueva Subasta con valores establecidos
Subasta::Subasta(int duracion, int precioInicial, int precioMinimo){
    this->fin=false;
    this->duracion=duracion;
    this->precioInicial=precioInicial;
    this->precioMinimo=precioMinimo;
    this->monitor= new Monitor(precioInicial);

    this->beneficios=0;
    this->nSubastas=1;
    this->nImagenes=0;
    this->tiempoTotal=duracion;
    this->tiempoMedio=0;
};

// Sobrescribo datos de la subasta actual con los de una nueva
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


// Precio PRIVADO minimo necesario para vender valla
int Subasta::obtenerPujaMin(){
	return precioMinimo;
};

int Subasta::nVallas(){
    return nImagenes;
};

Monitor* Subasta::obtenerMonitor(){
    return monitor;
};

// Tiempo que se mostrara la imagen
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
    	oss <<"\n--SUBASTA ABIERTA--" <<endl <<"Valla Publicitaria de "<<duracion<<" segundos"<<endl
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


// Actualiza datos, guarda datos ganador, los encola en el GESTOR de VALLAS
// y genera una nueva suabsta
// Parcial: si no hay ganador
bool Subasta::cerrarSubasta(int& user_id, string& estado){
    ostringstream oss;
    // Si aun no ha finalizado (todos han escrito PASAR) espero
    while(!monitor->Pasar()) esperar.wait();
    if(!fin){
        // Si hay ganador
    	if(monitor->getId()!=-1){
            oss <<"--SUBASTA CONCLUIDA--"<<endl << "Ganador: "<<monitor->getId()<<" Puja cerrada a "<<monitor->pujaActual() <<"$"<<endl;
            estado=oss.str();
            cout << estado;
    		beneficios+=monitor->pujaActual();
    		++nImagenes;
    		user_id = monitor->getId();
            beneficios+=monitor->pujaActual();
    		return false;
    	}
    	else{
    		oss <<"--SUBASTA CERRADA--"<<endl<< "No hay ganador, puja minima de "<<precioMinimo<<" no superada."<<endl;
            estado=oss.str();
            cout << estado;
    		return true;
    	}
    }
    else{
        oss <<endl<<"--SUBASTA CERRADA PERMANENTEMENTE--"<<endl;
        if(monitor->getId()!=-1){
            oss <<"Ganador: "<<monitor->getId()<<" Puja cerrada a "<<monitor->pujaActual() <<"$"<<endl<<endl;
            beneficios+=monitor->pujaActual();
            ++nImagenes;
            user_id = monitor->getId();
            beneficios+=monitor->pujaActual();
            estado=oss.str();
            cout << estado;
            return false;
        }
        else{
            oss <<"No hay ganador, puja minima de "<<precioMinimo<<" no superada."<<endl<<endl;
            estado=oss.str();
            cout << estado;
            return true;
        }
    }
};


string Subasta::infoHistorica(){
    ostringstream oss;
    oss << "Numero de subastas realizadas: "<<nSubastas<<endl<<"Tiempo total ofertado: "<<tiempoTotal<<endl
    <<"Tiempo medio ofertado: "<<tiempoMedio<<endl<<"Beneficios obtenidos. "<<beneficios<<"$"<<endl;
    cout << oss.str();
    return oss.str();
};
