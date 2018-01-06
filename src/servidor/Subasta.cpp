#include "Subasta.h"

int randomP(){
    srand (time(NULL));
    int randomP = rand() % 200 + 400;
    return randomP;
}

int randomT(){
    srand (time(NULL));
    int randomT = rand() % 40 + 10;
    return randomT;
}

int randomD(){
    srand (time(NULL));
    int randomP = rand() % 15 + 5;
    return randomP;
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
//Subasta::Subasta(int tInicial, int duracion, int precioInicial, int precioMinimo){
//    this->fin=false;
//    this->tInicial=tInicial;
//    this->duracion=duracion;
//    this->precioInicial=precioInicial;
//    this->precioMinimo=precioMinimo;
//    this->monitor= new Monitor(precioInicial);
//
//    this->beneficios=0;
//    this->nSubastas=1;
//    this->nImagenes=0;
//    this->tiempoTotal=duracion;
//    this->tiempoMedio=0;
//};

// Sobrescribo datos de la subasta actual con los de una nueva
void Subasta::nuevo(int min){
    this->fin=false;
    this->tInicial=randomT();
    this->duracion=randomD();
    this->precioInicial=randomP();
    this->precioMinimo=randomP()+(randomP()/5);
    this->monitor= new Monitor(precioInicial);

    this->nSubastas++;
    this->tiempoTotal+=duracion;
    this->tiempoMedio=tiempoTotal/nSubastas;

};


// Precio PRIVADO minimo necesario para vender valla
int Subasta::obtenerPujaMin(){
	return precioMinimo;
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

void Subasta::iniciar(){
	tiempoRestante=time(NULL)+tInicial;
	cout << "Subasta iniciada para Valla Publicitaria de "<<duracion<<"segundos"<<endl
	<< "abierta durante: "<<tInicial<< "segundos"<<endl
	<< "Puja inicial: " << precioInicial<<endl;

};

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
bool Subasta::cerrarSubasta(int& user_id){
	cout << "Tiempo agotado"<<endl;
	if(monitor->getId()!=-1){
		cout <<"Ganador: "<<monitor->getId()<<"Puja cerrada a "<<monitor->pujaActual() <<"€"<<endl;
		beneficios+=monitor->pujaActual();
		++nImagenes;
		user_id = monitor->getId();
		delete monitor;
		return false;
	}
	else{
		cout << "Ho hay ganador, puja minima de "<<precioMinimo<<" no superada."<<endl;
		delete monitor;
		return true;
	}
};


string Subasta::infoHistorica(){
    ostringstream oss;
    cout << "Numero de subastas realizadas: "<<nSubastas<<endl<<"Tiempo total ofertado: "<<tiempoTotal<<endl
    <<"Tiempo medio ofertado: "<<tiempoMedio<<endl<<"Beneficios obtenidos. "<<beneficios<<"€"<<endl;

    return oss.str();
};
