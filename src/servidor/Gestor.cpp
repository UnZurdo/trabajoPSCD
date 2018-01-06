//*****************************************************************
// File:   Gestor.cpp
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Subasta.cpp -c
//*****************************************************************
#include "Gestor.h"


Gestor::Gestor(){
  this->fin = false;
  this->contador = 0;
  this->s.setInitValue(0);
};


//Falta gestion valla
void Gestor::anyadirValla(Valla valla){
  // Si fin no añado ninguna más
  if(!fin) {
    q.push(valla);
    s.signal();
  } 
        
    
 };
 
void Gestor::apagar(){
  fin = true;
};

void Gestor::iniciar(){
  // Tamaños de ventana para las vallas publicitarias
  const int VALLA_WIDTH = 800;
  const int VALLA_HEIGHT = 800;

  // Creamos el objeto para descargar imágenes
  ImageDownloader downloader;
 // No acabar hasta que fin y se hallan mostrado todas las vallas
 while(!fin || !q.empty()){
    //mostrar imagen de la valla en cola
    // Si no se puede mostrar nada == > ESPERAR
    s.wait();
    Valla valla = q.front();
    q.pop();

    // transformo string a c_str()
    string URL = obtenerUrl(valla);
    string path = obtenerPath(valla);

    char *cURL = new char[URL.length() + 1];
    strcpy(cURL, URL.c_str());

    char *cPATH = new char[URL.length() + 1];
    strcpy(cPATH, URL.c_str());

    delete [] cPATH;
    delete [] cURL;

    // Descargamos una imagen de Internet
    downloader.downloadImage(cURL, cPATH);


    // Creamos una valla publicitaria con una imagen
    char name[] = "Valla ";
    char indice = contador+'0';
    strcat(name, &indice);
    ++contador;

    cimg_library::CImg<unsigned char> img_principal(cPATH);
    cimg_library::CImgDisplay imagen(img_principal.resize(VALLA_WIDTH,VALLA_HEIGHT), name);
    imagen.resize(VALLA_WIDTH,VALLA_HEIGHT);
    imagen.move(0,0); // Esquina superior izquierda

    // Mostrar imagen durante 5 segundos
    imagen.wait(5000);


 }
};
