/*
 *  Fichero de interfaz fecha.h del módulo fecha
 */

#ifndef FECHA_H
#define FECHA_H

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

/*
 * Un dato de tipo Fecha permite representar cualquier fecha
 * del calendario
 */
struct Fecha {
    private:
      int dia;
      int mes;
      int anyo;
    public:
        // Funciones con acceso a la estructura interna anterior
	friend Fecha definirFecha (int d, int m, int a);
	friend Fecha definirFecha (int fecha);
	friend int dia (const Fecha f);
	friend int mes (const Fecha f);
	friend int anyo (const Fecha f);
  friend bool operator==(const Fecha f1, const Fecha f2);
  friend bool operator>>(const Fecha f1, const Fecha f2);
  friend ostream& operator<<(ostream& os, const Fecha f);
};

/*
 * Pre: d >= 1 y d <= 31, m >= 1 y m <= 12, a > 0
 * Post: Devuelve la fecha definida por d/m/a
 */
Fecha definirFecha (int d, int m, int a);

/*
 * Pre: El valor de fecha tiene ocho cifras cuando se escribe en
 *      base 10: aaaammdd, donde aaaa define el año, mm el número
 *      del mes y dd el día del mes
 * Post: Devuelve la fecha definida por dia/mes/año
 */
Fecha definirFecha (int fecha);

/*
 * Pre: f es una fecha válida del calendario
 * Post: devuelve el número correspondiente al día de la fecha [f]
 */
int dia (const Fecha f);

/*
 * Pre: f es una fecha válida del calendario
 * Post: devuelve el número correspondiente al mes de la fecha [f]
 */
int mes (const Fecha f);

/*
 * Pre: f es una fecha válida del calendario
 * Post: devuelve el número correspondiente al año de la fecha [f]
 */
int anyo (const Fecha f) ;

bool operator==(const Fecha f1, const Fecha f2);


bool operator>>(const Fecha f1, const Fecha f2);


ostream& operator<<(ostream& os, const Fecha f);


#endif