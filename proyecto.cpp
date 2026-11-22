#include <iostream>
#include <cstdlib>
using namespace std;

#define CUANTO 2

struct Proceso {
    int id,
    tamano,
    cuanto;
};

class Bloque{
public:
    Proceso prcs;
    Bloque *sig, *ant;
};


class Memoria {
private:
    Bloque *inicio, *fin;
public:
    Bloque *listaProcesos(Bloque *incio, Bloque *fin){
        Bloque *temp = inicio;
        for(temp != nullptr){
            if(temp->prcs.cuanto == 0){
                Bloque *nuevo = new Bloque;

                nuevo->sig = nullptr;
                nuevo->ant = fin;
                
                if(fin != nullptr) {
                    fin->sig = nuevo;
                } else {
                    inicio = nuevo;
                }
                fin = nuevo;
            }
            
        }//for 
    } // listaProcesos
};