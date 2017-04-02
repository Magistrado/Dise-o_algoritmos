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


int beneficio_maximo = 0;
vector<Arista*> mejorSol;
vector<Arista*> solParcial;
Grafo instancia;

int calcularBeneficioCamino(vector<Arista *>camino){
    int costoArista = 0;

    for (int i=0; i<camino.size();i++){
        if (camino[i]->ida == 1 && camino[i]->vuelta == 1){
            costoArista += - (camino[i]->c);
        } else {
            costoArista += (camino[i]->b) - (camino[i]->c);
        }
    }
    return costoArista;
}

int calcularBeneficio(const vector<Arista *> &la) {
    int acum = 0;
    for (auto i = la.cbegin(); i != la.cend(); ++i) {
        acum += (*i)->b - ((*i)->c);
    }

    return acum;
}

bool produceBeneficio(vector<Arista *>ciclo) {
    return calcularBeneficio(ciclo) > 0;
}

bool cicloNeg(int v, Arista *e){
    vector<Arista *> ciclo;
    int vProx;

    // Vertice posiblemente que se repita en solParcial
    if (v == e->v1) vProx = e->v2; else vProx = e->v1;

    for (auto j = solParcial.rbegin(); j != solParcial.rend(); ++j)
    {
        ciclo.push_back(*j);
        if (v == (*j)->v1) v = (*j)->v2; else v = (*j)->v1;
        if(vProx == v) {
            if (!produceBeneficio(ciclo)){
                return true;
            }
        }
    }
    return false;
}

bool presenteSolParcial(Arista * e) {
    bool presente = false;

    for (auto i = solParcial.begin(); i != solParcial.end(); ++i) {
        if (((e->v1 == (*i)->v1) && (e->v2 == (*i)->v2)) || ((e->v1 == (*i)->v2) && (e->v2 == (*i)->v1)))
        {
            // Encontramos la arista en solParcial
            presente = true;
            break;
        }
    }

    if (!presente) {
        // La arista no se encuentra en la solParcial
        return false;
    } else {
        // Se encontro la arista al menos una vez.
        if (e->b == 0) { // La arista se recorre por segunda vez
            return false;
        } else {
            return true;
        }
    }
    return true;
}

bool repiteCiclo(int v, Arista* e) {
    Arista * e1;
    int vProx;
    if (v == e->v1) vProx = e->v2; else vProx = e->v1;

    for (auto j = solParcial.rbegin(); j != solParcial.rend(); ++j)
    {
        if (v == (*j)->v1) v = (*j)->v2; else v = (*j)->v1;
        if(vProx == v) {
            e1 = *j;
            if ((e->b - e->c) < (e1->b - e1->c))
                return false;
            else
                return true;
        }
    }
    return false;
}

bool cumpleAcotamiento(Arista* e) {
    int beneficio_e = e->b - e->c;

    int maxBeneficio = beneficio_maximo - max(0,beneficio_e) + calcularBeneficio(solParcial) + beneficio_e;
    if (maxBeneficio <= calcularBeneficio(mejorSol)) 
        return false;
    else
        return true;
}

int verticeMasExterno() 
{
    int vActual = 1;
    for (auto i = solParcial.begin(); i != solParcial.end(); ++i)
    {
        if (vActual == (*i)->v1){
            vActual = (*i)->v2;
        } else
        {
            vActual = (*i)->v1;
        }
    }
    return vActual;
}

void buscarProfun() 
{
    // Lista de sucesores 
    vector<Arista*> ladosAdyacentes;
    Arista * last;
    int v = verticeMasExterno();
    int aux = 0;

    if (v == 1)
    {
        if (calcularBeneficio(solParcial) > calcularBeneficio(mejorSol))
        {
            mejorSol = solParcial;
        }
    }

    ladosAdyacentes = instancia.obtSucesores(v);
    for (auto e = ladosAdyacentes.begin(); e != ladosAdyacentes.end(); ++e)
    {
        if (
            (!presenteSolParcial(*e))
            && (!cicloNeg(v,*e))
            && (!repiteCiclo(v,*e)) 
            && cumpleAcotamiento(*e)
            ) 
        {
            (*e)->t += 1;

            if ((*e)->t == 1){
                aux = (*e)->b - (*e)->c;
            } else {
                aux = - (*e)->c;
            }

            beneficio_maximo -= max(0, aux);
            solParcial.push_back(*e);
            buscarProfun();
        }
    }

    last = solParcial.back();
    beneficio_maximo += max(0, last->b - last->c);
    solParcial.pop_back();
}

void solver_bab(vector<Arista*> &solInicial)
{
    mejorSol = solInicial;
    solParcial.push_back(solInicial[0]);
    // Se usa como deposito 0 en lugar de 'd'
    beneficio_maximo = instancia.maxBen;
    buscarProfun();
}


int main(int argc, char const *argv[])
{
    vector<Arista *> caminoAristasGreedy;
    int greedy = 0;

    instancia.cargarInstancias(argv[1]);
    
    greedy = solver_greedy(instancia.graf, 1, caminoAristasGreedy);

    // Garantizo que no hay ciclos negativos.
    eliminarCiclosNeg(caminoAristasGreedy);

 

    solver_bab(caminoAristasGreedy);

    cout << "\nGreedy: " << greedy << endl;

    for (auto a=caminoAristasGreedy.begin(); a!=caminoAristasGreedy.end(); ++a){
        cout << "Camino " << (*a)->v1 << " - " << (*a)->v2 << " valores " << (*a)->c << " - " << (*a)->b << endl;
    }


    cout << "\nBAB\nBeneficio hallado: " << calcularBeneficioCamino(mejorSol) << endl;



    for (auto a = mejorSol.begin(); a != mejorSol.end(); ++a)
    {
        cout << "Camino " << (*a)->v1 << " - " << (*a)->v2 << " valores " << (*a)->c << " - " << (*a)->b << endl;
    }

    return 0;
}
