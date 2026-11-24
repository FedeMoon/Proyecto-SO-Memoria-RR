#include <iostream>
#include <queue>
using namespace std;

class Proceso {
public:
    int id, tam, cuanto, cuantoInicial, llegada;

    Proceso(int i = 0, int t = 0, int c = 0){
        id = i;
        tam = t;
        cuantoInicial = c;
        llegada = 0;
    }
    void imprimir(){
        cout << "[" << id << "," << tam << "," << cuanto << "]";
    }
};

struct Bloque { 
    int inicio, tam;
    bool libre;
    Proceso prcs;
};

class Memoria {
    int total, modo;
    vector<Bloque> b;

public:
    Memoria(int t, int m){
        total = t;
        modo = m;
        Bloque hueco = {0, t, true, Proceso()};
        b.push_back(hueco);
    }
    void asignar(Proceso p){
        int idx = -1;
        for (int i = 0; i < b.size(); i++){
            if(!b[i].libre || b[i].tam < p.tam) continue;
            if(modo == 1) {
                idx = i;
                break;
            }
        }
    }

    Proceso* buscar(int id){
        for (int i = 0; i < b.size(); i++)
            if(!b[i].libre && b[i].prcs.id == id) return &b[i].prcs;
        return 0;
    }
    void liberarMemoria(int id){
        for (int i = 0; i < b.size(); i++){
            if(!b[i].libre && b[i].prcs.id == id) b[i].libre = true;
        }
    }
    bool condesar(){
        bool unio = false;
        for(int i = 0; i + 1 < b.size(); i++){
            if(b[i].libre && b[i + 1].libre){
                b[i].tam += b[i + 1].tam;
                b.erase(b.begin() + i + 1);
                unio = true;
            } else i++;
        }
        return unio;
    }
};