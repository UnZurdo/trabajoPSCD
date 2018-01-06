//*****************************************************************
// File:   Valla.h
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Valla.cpp -c
//*****************************************************************

#include "Valla.h"

void crear(Valla& s, string url, string path, int d){
    s.URL=url;
    s.path= "../../imagenes/" + path;
    s.duracion=d;
}


void actualizarUrl(Valla& s, string url){
    s.URL = url;
}


void actualizarPath(Valla& s, string path){
    s.path = path;
}

string obtenerUrl(const Valla& s){
   return s.URL;
}
string obtenerPath(const Valla& s){
  return s.path;
}

void marcarDuracion(Valla& s, int d){
   s.duracion = d;
}
    
int obtenerDuracion(const Valla& s){
    return s.duracion;
    
}
