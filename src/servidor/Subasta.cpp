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
    int random = rand() % 40 + 10;
    return random;
}

int randomD(){
    srand (time(NULL));
    int random = rand() % 15 + 5;
    return random;
}

// Crea una nueva Subasta con valores generados aleatoriamente
Subasta::Subasta(){
    this->fin=false;
    this->tInicial=randomT();
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
Subasta::Subasta(int tInicial, int duracion, int precioInicial, int precioMinimo){
    this->fin=false;
    this->tInicial=tInicial;
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
    this->tInicial=randomT();
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


// Fecha final en la que se cerrara la subasta
int Subasta::cierreSubasta(){
	return duracion;
};

// Static function
void Subasta::handle_timer(int signo){
    signal(SIGALRM, handle_timer);
    esperar.signal();
};

void Subasta::iniciar(string& estado){
    ostringstream oss;
    if(!fin){
        signal(SIGALRM, handle_timer);
    	tiempoRestante=time(NULL)+tInicial;
        // Set alarm
        alarm(tInicial);

    	oss << "--SUBASTA ABIERTA--" <<endl <<"Valla Publicitaria de "<<duracion<<"segundos"<<endl
    	<< "abierta durante: "<<tInicial<< "segundos"<<endl
    	<< "Puja inicial: " << precioInicial<<endl;
        estado=oss.str();
        cout << estado;
    }
    else{
        oss << "SUBASTA CERRADA PERMANENTEMENTE" <<endl;
        estado=oss.str();
        cout << estado;
    }

};

int Subasta::obtenerDuracionSubasta(){
    return tInicial;
}

bool Subasta::finTiempo(){
 return time(NULL) >= tiempoRestante;
};

void Subasta::finalizarSubasta(){
  fin=true;
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
    // Si aun no ha finalizaod espero
    if(!finTiempo()) esperar.wait();
	cout << "Tiempo agotado"<<endl;
    // Si hay ganador
	if(monitor->getId()!=-1){
        oss <<"--SUBASTA CONCLUIDA--"<<endl << "Ganador: "<<monitor->getId()<<"Puja cerrada a "<<monitor->pujaActual() <<"€"<<endl;
        estado=oss.str();
        cout << estado;
		beneficios+=monitor->pujaActual();
		++nImagenes;
		user_id = monitor->getId();
		
        //delete monitor;
		return true;
	}
	else{
		oss <<"--SUBASTA CERRADA--"<<endl<< "No hay ganador, puja minima de "<<precioMinimo<<" no superada."<<endl;
        estado=oss.str();
        cout << estado;
		
        //delete monitor;
		return false;
	}
};


string Subasta::infoHistorica(){
    ostringstream oss;
    cout << "Numero de subastas realizadas: "<<nSubastas<<endl<<"Tiempo total ofertado: "<<tiempoTotal<<endl
    <<"Tiempo medio ofertado: "<<tiempoMedio<<endl<<"Beneficios obtenidos. "<<beneficios<<"€"<<endl;

    return oss.str();
};
