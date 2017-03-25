/*
    Nombre: Edgar Valderrama
            Daniel Leones       09-10977
    Fecha: 17/2/2017
    Descripción: Resolución de PRPP usando un algoritmo basado en branch and bound.

    Ejecución: ./solver_prpp /camino/a/instancia 1
    Colocar 1 como segundo arg dado que se requiere que retorne al mismo punto donde
    parte. 
*/
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>
#include "auxiliares.cpp"

using namespace std;

bool produceBeneficio(vector<Arista *>ciclo) {
    int costoArista = 0;

    for (int i=0; i< static_cast<int>(ciclo.size());i++){
        if (ciclo[i]->ida == 1 && ciclo[i]->vuelta == 1){
            costoArista += (ciclo[i]->b) - 2 * (ciclo[i]->c);
        } else {
            costoArista += (ciclo[i]->b) - (ciclo[i]->c);
        }
    }
    return costoArista > 0;
}


int calcularBeneficio(vector<Arista *> &la) {
    int acum = 0;
    for (auto i = la.begin(); i != la.end(); ++i) {   
        acum += (*i)->b - (*i)->c;
    }
    return acum;
}

bool cicloNeg(Arista * e, vector<Arista *> solParcial){
    vector<Arista *> ciclo;
    for (int j=1;j < static_cast<int>(solParcial.size());j++){
        ciclo.push_back(solParcial[j]);
        if(e->v2 == solParcial[j]->v2){
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

/*bool repiteCiclo(int nodoProx, vector<Arista*> ladosMaximos, Arista* e, vector<int> solParcial) {
    Arista * e1;
    bool cent = false;
    for (auto i = solParcial.begin(); i != solParcial.end(); ++i) {
        if (nodoProx == (*i)->v1) || (nodoProx == (*i)->v2))
        {
            e1 = *i;
            break;
        }

    }

    for (auto i = ladosMaximos.begin(); i != ladosMaximos.end(); ++i) {
        if (((e->v1 == (*i)->v1) && (e->v2 == (*i)->v2))
            || ((e->v1 == (*i)->v2) && (e->v2 == (*i)->v1)))
        {
            e1 = *i;
            break;
        }
    }
}*/

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
                /*&& (!cicloNeg(*e,solParcial))
                && (!repiteCiclo(ladosMaximos,*e,solParcial)) */
                && cumpleAcotamiento(g,*e,solParcialArista,mejorSol)
                ) 
            {
                (*e)->t += 1;
                g.maxBen -= max(0, (*e)->b - (*e)->c);
                if ((*e)->v1 == v)
                    solParcial.push_back((*e)->v2);
                else
                    solParcial.push_back((*e)->v1);

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

    Grafo instancia;
    instancia.cargarEjem(argv[1]);
    //instancia.cargarInstancias(argv[1]);
    instancia.imprimirGrafo();
    return 0;
}
