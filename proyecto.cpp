#include <iostream>
#include <ctime>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
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
            cout  << "\nDescargando proceso [" << p->id << "," << p->tam << ",0]\n";  
            mem.liberar(id);
            if(mem.condensar()) cout << "\nCondesando huecos\n";
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

class Simulador {
private:
    Memoria mem;
    ListaRR cola;
    int siguienteId, modo, tamMem, cuantoMax, cuantoSis, tamProceso, ciclos;
    Proceso pendiente;
    

    Proceso generar() {
        Proceso p(siguienteId, rand() % tamProceso + 1, rand() % cuantoMax + 1);
        siguienteId++;
        return p;
    }


public:
    Simulador (int tM, int modo, int cM, int cS, int tP) : mem(tM, modo) {
        siguienteId = 1;
        cuantoMax = cM;
        cuantoSis = cS;
        tamMem = tM;
        tamProceso = tP; 
        ciclos = 0;
        pendiente = generar();
    }

    void ciclo(){
        ciclos++;
        cout << "Ciclos: " << ciclos << "\n";
        
        if(mem.asignar(pendiente)){
            cola.encolarLista(pendiente.id);
            cout << "Entro: ";
            pendiente.imprimir();
            cout  << "\n";
            pendiente = generar();
        } else {
            cout << "Proceso: ";
            pendiente.imprimir();
            cout << " NO cupo en la memoria, espera\n";
        }

        mem.imprimirEstado();

        if(cola.estaVacia()){
            cout << "CPU vacia\n";
        } else {    
            cola.atenderProceso(mem, cuantoSis);
            cola.imprimirCola(mem);
            mem.imprimirEstado();
            cout << "\n";
        }
    }

    void estadisticas() {
        cout << "Procesos termiandos: " << cola.getTerminados();
    }

};

int main () {
    srand(time(NULL));
    cout << "Simulador de memoria y planificador Round Robin\n";
    
    int tamMemoria, modo, cuantoSis, cuantoPrcs, tamPrcs;

    // tamano de la memoria
    do {
        cout << "Ingrese el tamano de la memoria: \n 1. 1 MB \n 2. 4 MB \n 3. 8 MB \n";
        cin >> tamMemoria;

        if(tamMemoria < 1 || tamMemoria > 3) {
            cout << "Opcion invalida\n";
        }
    } while (tamMemoria < 1 || tamMemoria > 3);
    tamMemoria = (tamMemoria == 1) ? 1024 : (tamMemoria == 2) ? 4096 : 8192; 
    
    // algoritmo de colocacion
    do {
        cout << "Ingrese el alogritmo de colocacion: \n 1. Primer Ajuste \n 2. Peor Ajuste\n";
        cin >> modo;

        if(modo < 1 || modo > 2) {
            cout << "Opcion invalida\n";
        }
    } while (modo < 1 || modo > 2);

    // cuanto del sistema
    do {
        cout << "Ingrese el cuanto del sistema (positivo y entero): ";
        cin >> cuantoSis;

        if(cuantoSis < 1) {
            cout << "Opcion invalida\n";
        }
    } while (cuantoSis < 1);

    // cuanto de los procesos
    do {
        cout << "Ingrese el cuanto para los procesos (positivo y entero): ";
        cin >> cuantoPrcs;

        if(cuantoPrcs < 1) {
            cout << "Opcion invalida\n";
        }
    } while (cuantoPrcs < 1);

    // tamano maximo de los procesos
    do {
        cout << "Ingrese el tamano maximo de los procesos (KB): (Max: " << tamMemoria << ")";
        cin >> tamPrcs;

        if(tamPrcs < 1 || tamPrcs > tamMemoria) {
            cout << "Opcion invalida\n";
        }
    } while (tamPrcs < 1 || tamPrcs > tamMemoria);

    
    
    Simulador sim(tamMemoria, modo, cuantoPrcs, cuantoSis, tamPrcs);
    for (int i = 0; i < 10; i++){
        sim.ciclo();
         this_thread::sleep_for(chrono::seconds(2));
    }
    sim.estadisticas();

    return 0;
}



