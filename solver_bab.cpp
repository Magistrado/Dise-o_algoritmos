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


int calcularBeneficio(const vector<Arista *> &la) {
    int acum = 0;
    for (auto i = la.cbegin(); i != la.cend(); ++i) {
        if ((*i)->t == 1)
            acum += (*i)->b - ((*i)->c);
        else
            acum += - ((*i)->c)*((*i)->t);
    }
    return acum;
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

bool repiteCiclo(int v, Arista* e) {
    Arista * e1;
    int vProx;
    if (v == e->v1) vProx = e->v2; else vProx = e->v1;

    for (auto j = solParcial.rbegin(); j != solParcial.rend(); ++j)
    {
        if (v == (*j)->v1) v = (*j)->v2; else v = (*j)->v1;
        if(vProx == v) {
            e1 = *j;
            /*cout << "Elegido\tv1: " << e1->v1 << " v2: " << e1->v2 << " Costo: " 
                    << e1->c << " Beneficio: "
                    << e1->b << " #: " << e1->t << " Ida: " << e1->ida
                    << " Vuelta: " << e1->vuelta << endl;*/
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
    int maxBeneficio = beneficio_maximo - max(0,beneficio_e) 
        + calcularBeneficio(solParcial) + beneficio_e;
    /*cout << "beneficio_e: " << beneficio_e << " maxBeneficio: " 
        << maxBeneficio << " mejorSol " << calcularBeneficio(mejorSol) << endl;*/
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
        if (vActual == (*i)->v1) vActual = (*i)->v2; else vActual = (*i)->v1;
    }
    return vActual;
}

void buscarProfun() 
{
    // Lista de sucesores 
    vector<Arista*> ladosMaximos;
    int v = verticeMasExterno();
    cout << "\n\nNodo V " << v << endl;

    if (v == 0) 
    {
        cout << "Oviamente entra aca " << calcularBeneficio(solParcial) << "  " << calcularBeneficio(mejorSol) << endl;
        if (calcularBeneficio(solParcial) > calcularBeneficio(mejorSol))
        {
            cout << "Entro en el if pa ve" << endl;
            mejorSol = solParcial;
        }
    }
    cout << "Antes de las aydacencias" << endl;
    instancia.obtSucesores(v,ladosMaximos);

    /*for (auto vect=ladosMaximos.begin(); vect!=ladosMaximos.end(); ++vect)
    {
        cout << "Vector adyacente " << (*vect)->v1 << "  -  " << (*vect)->v2 << endl;
    }*/

    for (auto e = ladosMaximos.begin(); e != ladosMaximos.end(); ++e)
    {
        cout << "Vector adyacente " << (*e)->v1 << "  -  " << (*e)->v2 << endl;
        cout << "Presente sol parcial " << !presenteSolParcial(*e) << endl;
        cout << "Ciclo neg " << !cicloNeg(v,*e) << endl;
        cout << "Repite Ciclo " << !repiteCiclo(v,*e) << endl;
        cout << "Acotamiento " << cumpleAcotamiento(*e) << endl;

        if (
            (!presenteSolParcial(*e))
            && (!cicloNeg(v,*e))
            && (!repiteCiclo(v,*e)) 
            && cumpleAcotamiento(*e)
            ) 
        {
            (*e)->t += 1;
            beneficio_maximo -= max(0, (*e)->b - (*e)->c);
            solParcial.push_back(*e);
            buscarProfun();
        }
    }

    if (!solParcial.empty()) {
        beneficio_maximo += max(0, solParcial.back()->b - solParcial.back()->c);
        solParcial.pop_back();
    }
}

void solver_bab(vector<Arista*> &solInicial)
{
    mejorSol = solInicial;
    // Se usa como deposito 0 en lugar de 'd'
    beneficio_maximo = instancia.maxBen;
    buscarProfun();
}


int main(int argc, char const *argv[])
{
    vector<Arista *> caminoAristasGreedy;
    int greedy = 0;

    //instancia.cargarEjem(argv[1]);
    instancia.cargarInstancias(argv[1]);
    // instancia.imprimirGrafo();
    
    greedy = solver_greedy(instancia.graf, 1, caminoAristasGreedy);

    // Garantizo que no hay ciclos negativos.
    eliminarCiclosNeg(caminoAristasGreedy);

 

    solver_bab(caminoAristasGreedy);

    cout << "\nGreedy: " << greedy << endl;

    for (auto a=caminoAristasGreedy.begin(); a!=caminoAristasGreedy.end(); ++a){
        cout << "Camino " << (*a)->v1 << " - " << (*a)->v2 << endl;
    }


    cout << "\nBAB\nBeneficio hallado: " << calcularBeneficio(mejorSol) 
        << endl;



    for (auto a = mejorSol.begin(); a != mejorSol.end(); ++a)
    {
        cout << "Camino " << (*a)->v1 << " - " << (*a)->v2 << endl;
    }
    
    //instancia.imprimirGrafo();

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
