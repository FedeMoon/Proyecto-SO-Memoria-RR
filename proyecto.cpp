
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Proceso.h"
#include "GeneradorProcesos.h"

using namespace std;

int main() {

    srand(time(0));

    GeneradorProcesos generador(400, 10);

    vector<Proceso> procesos;

    procesos = generador.generarProcesos(5);

    cout << "===== PROCESOS GENERADOS =====" << endl;

    for (int i = 0; i < procesos.size(); i++) {
        procesos[i].imprimir();
        cout << endl;
    }

    cout << "\nTotal de procesos: " << procesos.size() << endl;

    return 0;
}
