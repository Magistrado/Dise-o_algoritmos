#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <queue>

using namespace std;

struct Arista {
    int v1;
    int v2;
    int c;
    int b;
    int t;
};


class Comparar
{
public:
    bool operator()(pair<int,int> a, pair<int,int> b)
    {
        return a.first < b.first;
    }
};



void imprimirVector(vector<Arista*> &adya) {
    for (vector<Arista*>::iterator i = adya.begin(); i != adya.end(); ++i)
    {
        cout << "    v1: " << (*i)->v1 << " v2: " << (*i)->v2 << " Costo: " 
            << (*i)->c << " Beneficio: "
            << (*i)->b << " #: " << (*i)->t<< endl;
    }
}

void imprimirCola(pair<int,int> it) {
    cout << "ben=" << it.first << " v2=" << it.second << '\n';
}

void borrarAristas(vector<Arista*> &v) {
    for (vector<Arista*>::iterator i = v.begin(); i != v.end(); ++i) delete *i;
}


int main(int argc, const char *argv[]) {

    ifstream in(argv[1]);
    int verticeObj = 0;
    //ofstream out("resultados.txt"); 
    int v1, v2, c, b, maxB;
    Arista* lado;
    map<int, vector< Arista* > > grafo;
    priority_queue< pair<int,int>, // (beneficio, vertice_2) 
        vector< pair<int,int> >, Comparar > ladosMaximos;
    //Iteradores 
    map<int, vector< Arista* > >::iterator it;
    vector< Arista* >::iterator itv;
    //vector<int> camino;
    int visitado[7] = {0, 0, 0, 0, 0, 0, 0 };

    if ( ! (stringstream(argv[2]) >> verticeObj) ) verticeObj = 0;

    // Descartar las primeras lineas de albaida
    //getline(in, strInput);
    //getline(in, strInput);
    while (in >> v1 >> v2 >> c >> b) {
        //cout << v1 << " " << v2 << " " << c << " " << b << endl;
        lado = new Arista { v1, v2, c, b, 0 };
        grafo[v1].push_back(lado);
        grafo[v2].push_back(lado);
        //lado = { v1, c, b, 1 };
    }

    cout << "Imprimir mi grafo" << endl;
    for (it = grafo.begin(); it != grafo.end(); ++it) { // calls a_map.begin() and a_map.end()
        cout << it->first << '\n';
        imprimirVector(it->second);
        //borrarAristas(it->second);
    }


    // Prueba del algoritmo greedy
    cout << "Objetivo=" << verticeObj << endl;
    int acumulado = 0, vertice = 1, ben = 0;
    do
    {   
        cout << "\nVertice:"  << vertice << " acumulado: " << acumulado << endl;
        for ( itv = grafo[vertice].begin(); itv != grafo[vertice].end(); ++itv)
        {   
            {
                if ((*itv)->t == 0) {
                    ben = (*itv)->b-((*itv)->c);

                } else {
                    ben = -((*itv)->c*(*itv)->t);
                    cout << "prueba" << endl;
                }

                (*itv)->t += 1;
                if ((*itv)->v2 == vertice && (*itv)->v1 != vertice)
                    ladosMaximos.push(pair<int,int>(ben,(*itv)->v1));

                if ((*itv)->v1 == vertice && (*itv)->v2 != vertice)
                    ladosMaximos.push(pair<int,int>(ben,(*itv)->v2));
            }
        }

        // Imprimir el resultado

        //visitado[vertice] += 1;

        
        cout << "(b=" << ladosMaximos.top().first << ",(" << vertice 
            << ", " << ladosMaximos.top().second << "))" << endl;

        acumulado += ladosMaximos.top().first;
        vertice = ladosMaximos.top().second;

        while (!ladosMaximos.empty()) {
            imprimirCola(ladosMaximos.top());
            ladosMaximos.pop();
        }

        //cout << visitado[verticeObj] << endl;
    } while ( vertice != verticeObj);

    cout << "Maximo beneficio: " << acumulado << endl;

    // Liberar memoria
    for (it = grafo.begin(); it != grafo.end(); ++it) {
        borrarAristas(it->second);
    }
    cout << "Memoria liberada" << endl;

    in.close();
    //out.close();
    return 0;
}
