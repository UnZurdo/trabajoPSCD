//*****************************************************************
// File:   Gestor.cpp
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
//*****************************************************************
#include "Gestor.h"


Gestor::Gestor(){
    this->fin = false;
    this->contador=0;
    this->s.setInitValue(0);
    this->turno.setInitValue(1);
    this->gestorCerrado.setInitValue(0);
};


void Gestor::anyadirValla(Valla valla){
    // Si no llega a fin sigue añadiendo
    if(!fin) {
        q.push(valla);
        s.signal();
    }
};


void Gestor::apagar(){
    fin = true;
    // Despierta a los dos procesos
    s.signal();
    s.signal();
    // Bloquea hasta que acaban los procesos
    gestorCerrado.wait(2);
};

string Gestor::estado(){
    ostringstream oss;
    oss << "Número de vallas mostradas: "<<contador<<endl<<"Número de vallas pendientes por mostrar: "<<q.size()<<endl;
    return oss.str();
}

void Gestor::iniciar(){
    // Tamaños de ventana para las vallas publicitarias
    const int VALLA_WIDTH = 800;
    const int VALLA_HEIGHT = 800;

    // Objeto para descargar imágenes
    ImageDownloader downloader;
    // No acaba hasta que fin valga true y se hallan mostrado todas las vallas

    // Mostrar imagen de la valla en cola
    // Si no se puede mostrar nada == > ESPERAR
    s.wait();
    while(!fin || !q.empty()){
        Valla valla;
        //Entra en SC
        turno.wait();
        if(!q.empty()){
            valla = q.front();
            q.pop();
        }
        turno.signal();
        // Transforma string a c_str()
        string URL = obtenerUrl(valla);
        // Guardado en la carpeta imagenes
        string path = "imagenes/"+obtenerPath(valla);

        string mensaje = "GESTOR==> URL: " + URL + "   path: " + path + "\n";
        cout << mensaje;

        char *cURL = new char[URL.length() + 1];
        strcpy(cURL, URL.c_str());

        char *cPATH = new char[path.length() + 1];
        strcpy(cPATH, path.c_str());

        // Descarga una imagen de Internet
        downloader.downloadImage(cURL, cPATH);

        // Crea una valla publicitaria con una imagen
        char name[50] = "valla";
        char indice = contador+'0';
        strcat(name, &indice);
        strcat(name, ".jpg\0");
        ++contador;

        cimg_library::CImg<unsigned char> img_principal(cPATH);
        cimg_library::CImgDisplay imagen(img_principal.resize(VALLA_WIDTH,VALLA_HEIGHT), name);

        imagen.resize(VALLA_WIDTH,VALLA_HEIGHT);
        imagen.move(0,0); // Esquina superior izquierda

        // Mostrar imagen
        imagen.wait(obtenerDuracion(valla)*1000);
        imagen.close();
        cout << "Cierro ventana valla..."<<endl;
        // Libera memoria
        delete [] cPATH;
        delete [] cURL;

        // Espera a que haya vallas 
        s.wait();

    }
    cout << "------- GESTOR CERRADO -------"<<endl;
    gestorCerrado.signal();
};
