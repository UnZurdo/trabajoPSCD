/*
 *  Fichero de implementacion fecha.cc del módulo fecha
 */
#include "Fecha.h"
/*
 * Pre: d >= 1 y d <= 31, m >= 1 y m <= 12, a > 0
 * Post: Devuelve la fecha definida por d/m/a
 */
Fecha definirFecha (int d, int m, int a){
    Fecha nueva;
    nueva.dia=d;
    nueva.mes=m;
    nueva.anyo=a;
    return nueva;
}

/*
 * Pre: El valor de fecha tiene ocho cifras cuando se escribe en
 *      base 10: aaaammdd, donde aaaa define el año, mm el número
 *      del mes y dd el día del mes
 * Post: Devuelve la fecha definida por dia/mes/año
 */
Fecha definirFecha (int fecha){
    Fecha nueva;
    nueva.dia=fecha%100;
    nueva.mes=fecha/100%100;
    nueva.anyo=fecha/10000;
    return nueva;
}

/*
 * Pre: f es una fecha válida del calendario
 * Post: devuelve el número correspondiente al día de la fecha [f]
 */
int dia (const Fecha f){
    return f.dia;
}

/*
 * Pre: f es una fecha válida del calendario
 * Post: devuelve el número correspondiente al mes de la fecha [f]
 */
int mes (const Fecha f){
  return f.mes;
}

/*
 * Pre: f es una fecha válida del calendario
 * Post: devuelve el número correspondiente al año de la fecha [f]
 */
int anyo (const Fecha f) {
  return f.anyo;
}

bool operator==(const Fecha f1, const Fecha f2){

}


bool operator>>(const Fecha f1, const Fecha f2){

}


ostream& operator<<(ostream& os, const Fecha f){

}