#include <iostream>
#include <vector>
using namespace std;

class ListaRR {
    vector<int> cola;
    int terminados;
public:
    ListaRR() : terminados(0) {}
    int getTerminados ()const{
        return this->terminados;
    }
    void encolarLista(int id){
        cola.push_back(id);
    }
    void atenderProceso(Memoria& mem, int cuantoSis){
        if(cola.empty()){
            cout << "CPU ociosa\n";
            return;
        } 

        int id = cola[0];
        cola.erase(cola.begin());

        Proceso *p = mem.buscar(id);
        if(!p) {
            cout << "Error: Proceso con id: " << id << " no encontrado en la memoria\n";
            return;
        }
        cout << "Atendiendo proceso ";
        p->imprimir();
        cout << "\n";

        int uso = (p->cuanto < cuantoSis) ? p->cuanto : cuantoSis;
        p->cuanto -= uso;
        if(p->cuanto > 0) cola.push_back(id);
        if(p->cuanto == 0) {
            terminados++;
            cout  << "Descargando proceso [" << p->id << "," << p->tam << ",0]";  
            mem.liberar(id);
            if(mem.condensar()) cout << "Condesando huecos\n";
        }
    }
    void imprimirCola() const {
        cout << "Cola Round Robin: \n";
        if(cola.empty()){
            cout << "Cola vacia\n";
            return;
        }
        for(int i = 0; i < (int)cola.size(); i++){
            cout << "ID: " << cola[i];
            if(i < (int)cola.size()) cout << "->";
        }
        cout << "\n";
    }
    
};