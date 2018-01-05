//*****************************************************************
// File:   Valla.h
// Date:   december 2017
// Coms:   TP6 PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread Valla.cpp -c
//*****************************************************************
#ifndef VALLA_H
#define VALLA_H


#include <iostream>
#include <ctime>
#include <iomanip>
using namespace std;

struct Valla;
void crear(Valla& s);
// Guardar URL imagen ganador
void actualizarUrl(Valla& s, string url);
// Guardar path imagen
void actualizarPath(Valla& s, string path);
string obtenerUrl(const Valla& s);
string obtenerPath(const Valla& s);

struct Valla{
    friend void crear(Valla& s, string url, string path, int d)
    friend void actualizarUrl(Valla& s, string url);
    friend void actualizarPath(Valla& s, string path);
    friend string obtenerUrl(const Valla& s);
    friend string obtenerPath(const Valla& s);
    friend int marcarDuracion(Valla& s, int d);
    friend int obtenerDuracion(const Valla& s);

    private:
        // Hace falta convertir a char* para utilizar la funcion de downloadImage
        // Link: https://stackoverflow.com/questions/7352099/stdstring-to-char
        string URL;
        string path;
        int duracion;
};


#endif
