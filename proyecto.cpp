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
    void imprimir() const {
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
        return nullptr;
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

    void imprimirEstado() const {
        cout << "Memoria: \n";
        for(int i = 0; i < (int)b.size(); i++){
            if(b[i].libre){
                cout << "[0, " << b[i].tam << ", 0]";
            } else {
                b[i].prcs.imprimir();
            } 
            if (i + 1 < (int)b.size()) cout << " -> ";
        }
        cout << "\n";
    }
};

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
    bool estaVacia () const {
        return cola.empty();
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
    void imprimirCola(Memoria& mem) const {
        cout << "Cola Round Robin: \n";
        if(cola.empty()){
            cout << "Cola vacia\n";
            return;
        }
        for(int i = 0; i < (int)cola.size(); i++){
           Proceso* p = mem.buscar(cola[i]);
           if(p) {
                p->imprimir();
           } else {
            cout << "[?,?,?]";
           } 
            if(i + 1< (int)cola.size()) cout << " -> ";
        }
        cout << "\n";
    }
    
};

int main () {
    int tamMemoria = 1024;
    int modo = 2;
    int cuantoSis = 2;

    Memoria mem(tamMemoria, modo);
    ListaRR rr;

    Proceso llegadas[] {
        Proceso(1, 300, 5),
        Proceso(2, 200, 2),
        Proceso(3, 150, 4)
    };

    cout << "\n Admision Inicial: \n";
    for (int i = 0; i < 3; i++){
        if(mem.asignar(llegadas[i])){
            rr.encolarLista(llegadas[i].id);
            cout << "Entro: ";
            llegadas[i].imprimir();
            cout << "\n";
        } else {
            cout << "No cupo\n";
            llegadas[i].imprimir();
            cout << "\n";
        }
    }

    mem.imprimirEstado();
    rr.imprimirCola(mem);

    cout << "\n Ejecucion de turnos: \n";
    int turno = 1;
    while(!rr.estaVacia()){
        cout << "\n Turno: " << turno++ << "\n";
        rr.atenderProceso(mem, cuantoSis);
        mem.imprimirEstado();
        rr.imprimirCola(mem);
    }

    cout << "Procesos terminados: " << rr.getTerminados() << "\n";
    return 0;
}
