//*****************************************************************
// File:   Valla.cpp
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
//*****************************************************************

#include "Valla.h"

void crear(Valla& s, string url, string path, int d){
    s.URL=url;
    s.path= path;
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
