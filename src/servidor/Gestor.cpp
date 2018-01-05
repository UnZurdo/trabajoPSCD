//*****************************************************************
// File:   Gestor.cpp
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Subasta.cpp -c
//*****************************************************************

#include <iostream>
#include <ctime>
#include <iomanip>
#include <queue>
#include "ImageDownloader.hpp"
#include "CImg.h"
#include "Valla.h"


Gestor:;Gestor();{
    fin = false;
    //CREAR VALLA
}
//Falta gestion valla
void anyadirValla::Gestor(Valla valla){
 
    if (!(Gestor.q).empty()){
       Gestor.push(valla);
    }
    
    
 }
 
 void apagar::Gestor(){
      Gestor.fin = true;
 }
 
 void iniciar(bool& fin){
     while(!fin){
         //mostrar imagen de la valla en cola
     }
 }
