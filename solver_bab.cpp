/*
    Nombre: Edgar Valderrama    09-10870
            Daniel Leones       09-10977
    Fecha: 25/3/2017
    Descripción: Resolución de PRPP usando un algoritmo basado en branch and bound.
    Ejecución: ./solver_bab /camino/a/instancia 
*/
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>
#include "auxiliares.cpp"
#include "solver_prpp.cpp"

using namespace std;

bool produceBeneficio(vector<Arista *>ciclo) {
    int costoArista = 0;

    for (int i=0; i< static_cast<int>(ciclo.size());i++){
        if (ciclo[i]->t == 2){
            costoArista += - 2 * (ciclo[i]->c);
        } else {
            costoArista += (ciclo[i]->b) - (ciclo[i]->c);
        }
    }
    return costoArista > 0;
}


int calcularBeneficio(vector<Arista *> &la) {
    int acum = 0;
    for (auto i = la.begin(); i != la.end(); ++i) {   
        acum += (*i)->b - ((*i)->c*(*i)->t);
    }
    return acum;
}

bool cicloNeg(int v, Arista * e, vector<Arista *> &solParcialArista, vector<int> &solParcial){
    vector<Arista *> ciclo;
    int vProx;
    if (v == e->v1) vProx = e->v2; else vProx = e->v1;

    for (int j= static_cast<int>(solParcial.size()) - 1 ; j >= 0; j--){
        ciclo.push_back(solParcialArista[j]);
        if(vProx == solParcial[j]) {
            if (!produceBeneficio(ciclo)){
                return true;
            }
        }
    }
    return false;
}

bool presenteSolParcial(Arista * e, vector<Arista *> solParcial) {
    bool presente = false;
    int cont_e = 0;
    for (auto i = solParcial.begin(); i != solParcial.end(); ++i) {
        if (((e->v1 == (*i)->v1) && (e->v2 == (*i)->v2))
            || ((e->v1 == (*i)->v2) && (e->v2 == (*i)->v1)))
        {
            presente = true;
            cont_e += 1;
        }
    }

    if (!presente) {
        return false;
    } else if (cont_e > 1) {
        if (e->t > 1) {
            return false;
        } else {
            return true;
        }
    }
    return true;
}

bool repiteCiclo(int v, Arista* e, vector<Arista*> &solParcialArista, 
                 vector<int> &solParcialNodos) {
    Arista * e1;

    int vProx;
    if (v == e->v1) vProx = e->v2; else vProx = e->v1;

    for (int j= static_cast<int>(solParcialNodos.size()) - 1 ; j >= 0; j--) {
        if(vProx == solParcialNodos[j]) {
            e1 = solParcialArista[j];
            cout << "Elegido\tv1: " << e1->v1 << " v2: " << e1->v2 << " Costo: " 
                    << e1->c << " Beneficio: "
                    << e1->b << " #: " << e1->t << " Ida: " << e1->ida
                    << " Vuelta: " << e1->vuelta << endl;
            if ((e->b - e->c) < (e1->b - e1->c))
                return false;
            else
                return true;
        }
    }
    return false;
}

bool cumpleAcotamiento(Grafo &g, Arista* e, vector<Arista*> &solParcial, 
                        vector<Arista * > &mejorSol) {
    int beneficio_e = e->b - e->t;
    int maxBeneficio = g.maxBen - max(0,beneficio_e) 
        + calcularBeneficio(solParcial) + beneficio_e;
    if (maxBeneficio <= calcularBeneficio(mejorSol)) 
        return false;
    else
        return true;
}

void solver_bab(Grafo &g, vector<Arista*> &solInicial, 
                int benSolInicial, vector<Arista *> &solOpt) 
{
    // Lista de sucesores 
    vector<Arista*> ladosMaximos;
    // Pila para control del orden de éxpansión
    vector<int> solParcial;
    // Camino de la solucion hallada del algoritmo
    vector<Arista*> solParcialArista;
    vector<int> solParcialNodos;

    vector<Arista*> mejorSol = solInicial;
    Arista* ultimoLado;
    int v;
    // Se usa como deposito 0 en lugar de 'd'
    solParcial.push_back(0);

    while (!solParcial.empty()) 
    {
        v = solParcial.back();

        if (v == 0) 
        {               
            if (calcularBeneficio(solParcialArista) 
                > calcularBeneficio(mejorSol)) mejorSol = solParcialArista;
        }

        g.obtSucesores(v,ladosMaximos);
        for (auto e = ladosMaximos.begin(); e != ladosMaximos.end(); ++e)
        {
            if ( 
                (!presenteSolParcial(*e,solParcialArista)) 
                && (!cicloNeg(v,*e,solParcialArista,solParcial))
                && (!repiteCiclo(v,*e,solParcialArista,solParcial)) 
                && cumpleAcotamiento(g,*e,solParcialArista,mejorSol)
                ) 
            {
                (*e)->t += 1;
                g.maxBen -= max(0, (*e)->b - (*e)->c);
                if ((*e)->v1 == v) {
                    solParcial.push_back((*e)->v2);
                }
                else {
                    solParcial.push_back((*e)->v1);
                }

                solParcialArista.push_back(*e);
                break;
            }
        }
        ultimoLado = solParcialArista.back();
        solParcialArista.pop_back();
        g.maxBen += max(0, ultimoLado->b - ultimoLado->c);
    }
    solOpt = mejorSol;
}


int main(int argc, char const *argv[])
{
    cout << "RoadHouse";

    Grafo instancia;
    vector<Arista *> caminoAristasGreedy;
    vector<Arista *> caminoOptimo;
    int greedy=0;

    instancia.cargarEjem(argv[1]);
    instancia.cargarInstancias(argv[1]);
    // instancia.imprimirGrafo();
    
    greedy = solver_greedy(instancia.graf, 1, caminoAristasGreedy);

    for (auto a=caminoAristasGreedy.begin(); a!=caminoAristasGreedy.end(); ++a){
        cout << "Camino " << (*a)->v1 << " - " << (*a)->v2 << endl;
    }

    solver_bab(instancia, caminoAristasGreedy, greedy, caminoOptimo);
    
    instancia.imprimirGrafo();

    // Arista* lado = new Arista {6,3,2,4,0,0,0};
    // vector<int> nodoPrueba { 1, 2, 3, 4, 5, 6};
    // vector<Arista*> aristasPrueba 
    // {
    //     new Arista { 1,2,2,4,0,0,0 },
    //     new Arista { 2,3,2,4,0,0,0 },
    //     new Arista { 3,4,1,4,0,0,0 },
    //     new Arista { 4,5,2,4,0,0,0 },
    //     new Arista { 5,6,2,4,0,0,0 }
    // };

    // cout << "Resultado: " << 
    //     repiteCiclo(6,lado,aristasPrueba,nodoPrueba ) << endl;

    // delete lado;
    // for (vector<Arista*>::iterator i = aristasPrueba.begin(); 
    //                 i != aristasPrueba.end(); ++i) 
    // {
    //     if (*i == 0){
    //         delete *i;
    //         *i = 0;
    //     }
    // }

    return 0;
}