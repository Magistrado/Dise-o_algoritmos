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
    bool operator()(pair<int, Arista * > a, pair<int, Arista *> b)
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

/*string imprimirArista (const Arista &i) {
    cout << "(v1: " << i.v1 << ",v2: " << i.v2 << ",Costo: " 
            << i.c << ", Beneficio: "
            << i.b << ",#: " << i.t << ")" << endl;
}*/

void imprimirParCola(const pair<int, Arista *> &it) {
    cout << "(ben=" << it.first << ", (" << (it.second)->v1 
        << "-->" << (it.second)->v2 << ")" << ",#:" 
        << (it.second)->t << ")\n";
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
    priority_queue< pair<int,Arista * >, // (beneficio, vertice_2) 
        vector< pair<int, Arista * > >, Comparar > ladosMaximos;
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
    }



    // Prueba del algoritmo greedy
    int acumulado = 0, vertice = 1, ben = 0;
    bool final = false;
    cout << "\nObjetivo=" << verticeObj << endl;
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
                }

                ladosMaximos.push(pair<int, Arista *>(ben,*itv));
            }
        }

        // Imprimir el resultado
        //cout << "Lado escogido: ";
        //imprimirParCola(ladosMaximos.top());
        
        acumulado += ladosMaximos.top().first;
        // Contar visita para tal lado

        ladosMaximos.top().second->t += 1;
        cout << "Proximo: ";
        if (ladosMaximos.top().second->v2 == vertice ) {
            //&& ladosMaximos.top().second->v1 != vertice)
            cout << "(" << ladosMaximos.top().second->v1 << "<--" 
                      << ladosMaximos.top().second->v2 << ")" << endl;
            vertice = ladosMaximos.top().second->v1;
        } else if (ladosMaximos.top().second->v1 == vertice) {
            //&& ladosMaximos.top().second->v2 != vertice)
            cout << "(" << ladosMaximos.top().second->v1 << "-->" 
                      << ladosMaximos.top().second->v2 << ")" << endl;
            vertice = ladosMaximos.top().second->v2;
        }

        cout << "Cola de prioridades: " << endl;
        while (!ladosMaximos.empty()) {
            imprimirParCola(ladosMaximos.top());
            ladosMaximos.pop();
        }

    } while ( vertice != verticeObj);

    cout << "Maximo beneficio: " << acumulado << endl;
    cout << "\nImprimir mi grafo" << endl;
    for (it = grafo.begin(); it != grafo.end(); ++it) { // calls a_map.begin() and a_map.end()
        cout << it->first << '\n';
        imprimirVector(it->second);
        // Liberar memoria
    }

    for (it = grafo.begin(); it != grafo.end(); ++it) {
        borrarAristas(it->second);
    }
    cout << "Memoria liberada" << endl;

    in.close();
    //out.close();
    return 0;
}
