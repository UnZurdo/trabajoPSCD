//*****************************************************************
// File:   Administrador.h
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
//*****************************************************************
#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H


#include <iostream>
#include <ctime>
#include <iomanip>
#include <queue>
#include "Monitor.h"
#include "Gestor.h"
#include "Subasta.h"

using namespace std;

class Administrador{
	Gestor* gestor;
	Subasta* subasta;

public:

	/*
	 * Pre: ---
	 * Post: Crea un objeto de la clase administrador
	 */
    Administrador(Gestor* gestor, Subasta* subasta);

	/*
	 * Pre: ---
	 * Post: Finaliza la subasta
	 */
    void apagarServidor();

	/*
	 * Pre: ---
	 * Post: Devuelve un cadena que contiene la información acerca
 	 *		 de la subastas en dicho instante
	 */
	string mostrarEstado();

	/*
	 * Pre: ---
	 * Post: Devuelve un cadena que contiene la información acerca
	 *		 de todas las subastas desde el momento que inicio el programa
	 */
	string mostrarHistoria();

	/*
	 * Pre: ---
	 * Post: Bucle infinito hasta que finaliza el programa en el que se leen
     * 		 las ordenes por la linea de comandos (ESTADO,
	 *		 HISTORICO o EXIT)
	 */
	void iniciarAdmin(bool& fin);

};


#endif
