//*****************************************************************
// File:   Gestor.cpp
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Subasta.cpp -c
//*****************************************************************
#include "Gestor.h"
Gestor::Gestor();{
    fin = false;
}
//Falta gestion valla
void anyadirValla::Gestor(Valla valla){
    // Si fin no añado ninguna más
       if(!fin) {
           q.push(valla);
        // s.signal()
       }
        
    
 }
 
 void apagar::Gestor(){
      Gestor.fin = true;
 }
 
 void iniciar::Gestor(bool& fin){
     // No acabar hasta que fin y se hallan mostrado todas las vallas
     while(!fin || !q.empty()){
         //mostrar imagen de la valla en cola
         // Si no se puede mostrar nada == > ESPERAR
         // s.wait()
     }
 }
