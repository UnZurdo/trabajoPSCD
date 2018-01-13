//*****************************************************************
// File:   Valla.h
// Date:   Enero 2018
// Authors: García Hernández, Alberto 741363
//          Generelo Gimeno, Jorge 737317
//          Gómez Lahera, Miguel 741302
//*****************************************************************
#ifndef VALLA_H
#define VALLA_H


#include <iostream>
#include <ctime>
#include <iomanip>
using namespace std;

struct Valla;
/*
 * Pre: ---
 * Post: Crea una valla en la que se almacena la url, el path
 *       y su duración
 */
void crear(Valla& s);

/*
 * Pre: ---
 * Post: Actualiza la url
 */
void actualizarUrl(Valla& s, string url);

/*
 * Pre: ---
 * Post: Actualiza el path
 */
void actualizarPath(Valla& s, string path);

/*
 * Pre: ---
 * Post: Devuelve una cadena que equivale a la url
 */
string obtenerUrl(const Valla& s);

/*
 * Pre: ---
 * Post: Devuelve una cadena que equivale a el path
 */
string obtenerPath(const Valla& s);

struct Valla{
    friend void crear(Valla& s, string url, string path, int d);
    friend void actualizarUrl(Valla& s, string url);
    friend void actualizarPath(Valla& s, string path);
    friend string obtenerUrl(const Valla& s);
    friend string obtenerPath(const Valla& s);
    friend void marcarDuracion(Valla& s, int d);
    friend int obtenerDuracion(const Valla& s);

private:
    string URL;
    string path;
    int duracion;
};


#endif
