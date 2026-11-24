
#ifndef GENERADORPROCESOS_H
#define GENERADORPROCESOS_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include "Proceso.h"

using namespace std;

class GeneradorProcesos {
private:
    int siguienteID;
    int tamMaximo;
    int cuantoMaximo;

public:
    GeneradorProcesos(int tam, int cuanto) {
        siguienteID = 1;
        tamMaximo = tam;
        cuantoMaximo = cuanto;
    }

    vector<Proceso> generarProcesos(int cantidad) {
        vector<Proceso> procesos;

        for (int i = 0; i < cantidad; i++) {

            int tam = 1 + rand() % tamMaximo;
            int cuanto = 1 + rand() % cuantoMaximo;

            Proceso p(siguienteID, tam, cuanto);

            procesos.push_back(p);

            siguienteID++;
        }

        return procesos;
    }
};

#endif
