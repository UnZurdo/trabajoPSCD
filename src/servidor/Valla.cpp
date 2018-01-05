//*****************************************************************
// File:   Valla.h
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Valla.cpp -c
//*****************************************************************

#include <iostream>
#include <ctime>
#include <iomanip>
using namespace std;

void crear::Valla(Valla& s){
}


void actualizarUrl::Valla(Valla& s, string url){
    s.URL = url;
}


void actualizarPath::Valla(Valla& s, string path){
    s.path = path;
}

string obtenerUrl(const Valla& s){
   return s.URL;
}
string obtenerPath(const Valla& s){
  return s.path;
}
