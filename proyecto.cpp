#include <iostream>
#include <vector>
using namespace std;

#define MIN_SPLIT 32

class Proceso {
public:
    int id, tam, cuanto, cuantoInicial, llegada;

    Proceso(int i = 0, int t = 0, int c = 0){
        id = i;
        tam = t;
        cuanto = c;
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
    bool asignar(Proceso p){
        int idx = -1;
        for (int i = 0; i < b.size(); i++){
            if(!b[i].libre || b[i].tam < p.tam) continue;
            if(modo == 1) {//primer ajuste
                idx = i;
                break;
            }
            if(idx == -1 || b[i].tam > b[idx].tam) idx = i; //peor ajuste
        }

        if(idx == -1) return false;
        int sobra = b[idx].tam - p.tam;

        if(sobra >= MIN_SPLIT){ //hacer hueco
            Bloque resto = {b[idx].inicio + p.tam, sobra, true, Proceso()};
            b[idx].tam = p.tam;
            b.insert(b.begin() + idx + 1, resto);
        } //si sobra menos de 32 no se parte
        b[idx].libre = false;
        b[idx].prcs = p;
        return true;
    }

    Proceso* buscar(int id){
        for (int i = 0; i < b.size(); i++)
            if(!b[i].libre && b[i].prcs.id == id) return &b[i].prcs;
        return 0;
    }
    void liberar(int id){
        for (int i = 0; i < b.size(); i++){
            if(!b[i].libre && b[i].prcs.id == id) b[i].libre = true;
        }
    }
    bool condensar(){
        bool unio = false;
        for(int i = 0; i + 1 < b.size();){
            if(b[i].libre && b[i + 1].libre){
                b[i].tam += b[i + 1].tam;
                b.erase(b.begin() + i + 1);
                unio = true;
            } else i++;
        }
        return unio;
    }
};
