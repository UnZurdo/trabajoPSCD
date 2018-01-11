//*****************************************************************
// File:   Gestor.cpp
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Gestor.cpp -c
//*****************************************************************
#include "Gestor.h"


Gestor::Gestor(){
  this->fin = false;
  this->contador=0;
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

string Gestor::estado(){
    ostringstream oss;
    oss << "Numero de vallas mostradas: "<<contador<<endl<<"Numero de vallas pendientes por mostrar: "<<q.size()<<endl;
    cout << oss.str();
    return oss.str();
}

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
    // Las guardo en la carpeta imagenes
    string path = "imagenes/"+obtenerPath(valla);

    cout << "GESTOR==> "<< "URL: " << URL << "   path: " << path <<endl;

    char *cURL = new char[URL.length() + 1];
    strcpy(cURL, URL.c_str());

    char *cPATH = new char[path.length() + 1];
    strcpy(cPATH, path.c_str());

    // Descargamos una imagen de Internet
    downloader.downloadImage(cURL, cPATH);

    cout << "GESTOR==>IMAGEN DESCRGADA"<<endl;

    // Creamos una valla publicitaria con una imagen
    char name[50] = "valla";
    char indice = contador+'0';
    strcat(name, &indice);
    strcat(name, ".jpg\0");
    ++contador;

    cimg_library::CImg<unsigned char> img_principal(cPATH);
    cimg_library::CImgDisplay imagen(img_principal.resize(VALLA_WIDTH,VALLA_HEIGHT), name);
    cout << "GESTOR==>VALLA CREADA"<<endl;
    imagen.resize(VALLA_WIDTH,VALLA_HEIGHT);
    imagen.move(0,0); // Esquina superior izquierda

    // Mostrar imagen durante 5 segundos
    imagen.wait(obtenerDuracion(valla)*1000);
    cout << "Cierro ventana valla..."<<endl;
    // Libero memoria
    delete [] cPATH;
    delete [] cURL;


 }
};
