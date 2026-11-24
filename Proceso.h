#ifndef PROCESO_H
#define PROCESO_H

#include <iostream>
using namespace std;

class Proceso {
public:
    int id;
    int tam;
    int cuanto;
    int cuantoInicial;
    int llegada;

    Proceso(int i = 0, int t = 0, int c = 0) {
        id = i;
        tam = t;
        cuanto = c;
        cuantoInicial = c;
        llegada = 0;
    }

    void imprimir() {
        cout << "[" << id << ", "
             << tam << " KB, "
             << cuanto << "]";
    }
};

#endif
