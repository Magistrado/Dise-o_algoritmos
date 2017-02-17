#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <queue>

using namespace std;

void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

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

void imprimirCamino(vector<int> &camino) {
    cout << "Camino:";
    for (vector<int>::iterator i = camino.begin(); i != camino.end(); ++i)
    {
        cout << " " << *i;
    }
    cout << endl;
}

void imprimirParCola(const pair<int, Arista *> &it) {
    cout << "(ben=" << it.first << ", (" << (it.second)->v1 
        << "-->" << (it.second)->v2 << ")" << ",#:" 
        << (it.second)->t << ")\n";
}

void borrarAristas(vector<Arista*> &v) {
    for (vector<Arista*>::iterator i = v.begin(); i != v.end(); ++i) {
        if (*i == nullptr){
            delete *i;
            *i = nullptr;
        }
    }
}

void cargarEjem(string camino, map<int, vector< Arista* > > &grafo) {
    ifstream in(camino);
    int v1, v2, c, b;
    Arista* lado;
    while (in >> v1 >> v2 >> c >> b) {
        lado = new Arista { v1, v2, c, b, 0 };
        grafo[v1].push_back(lado);
        grafo[v2].push_back(lado);
    }
    in.close();
}


void cargarInstancias(string camino, map<int, vector< Arista* > > &grafo) {

    ifstream in(camino);
    string entrada, entrada2;
    vector<string> entradaDividida;
    Arista* lado;
    int nroLados, v1, v2, c, b;

    getline(in, entrada);
    getline(in, entrada);   
    entradaDividida = split(entrada, ' ');
    if ( ! (stringstream(entradaDividida.back()) 
        >> nroLados) ) nroLados = 0;

    cout << "Lados Requeridos: " << nroLados << endl;
    for (int i = 0; i < nroLados; ++i)
    {
        in >> v1 >> v2 >> c >> b;
        //cout << v1 << " " << v2 << endl;
        lado = new Arista { v1, v2, c, b, 0 };
        grafo[v1].push_back(lado);
        grafo[v2].push_back(lado);
    }

    //in.clear();
    // Se usa para hacer avanzar el cursor sino no lee
    getline(in, entrada);
    getline(in, entrada);
    entradaDividida = split(entrada, ' ');

    if ( ! (stringstream(entradaDividida.back()) 
        >> nroLados) ) nroLados = 0;

    cout << "Lados No Requeridos: " << nroLados << endl;
    for (int i = 0; i < nroLados; ++i)
    {
        in >> v1 >> v2 >> c >> b;
        //cout << v1 << " " << v2 << endl;
        lado = new Arista { v1, v2, c, b, 0 };
        grafo[v1].push_back(lado);
        grafo[v2].push_back(lado);
    }
    in.close();
}

int solver(map<int, vector< Arista* > > &grafo, int verticeObj,
                        vector<int> &camino) 
{

    int acumulado = 0, vertice = 1, ben = 0;
    priority_queue< pair<int,Arista * >, // (beneficio calculado , _) 
        vector< pair<int, Arista * > >, Comparar > ladosMaximos;
    //Iteradores 
    vector< Arista* >::iterator itv;

    camino.push_back(vertice);
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
        
        acumulado += ladosMaximos.top().first;
        // Contar visita para tal lado
        ladosMaximos.top().second->t += 1;

        cout << "Proximo: ";
        if (ladosMaximos.top().second->v2 == vertice ) {
            cout << "(" << ladosMaximos.top().second->v1 << "<--" 
                      << ladosMaximos.top().second->v2 << ")" << endl;
            vertice = ladosMaximos.top().second->v1;
        } else if (ladosMaximos.top().second->v1 == vertice) {
            cout << "(" << ladosMaximos.top().second->v1 << "-->" 
                      << ladosMaximos.top().second->v2 << ")" << endl;
            vertice = ladosMaximos.top().second->v2;
        }

        camino.push_back(vertice);

        cout << "Cola de prioridades: " << endl;
        while (!ladosMaximos.empty()) {
            imprimirParCola(ladosMaximos.top());
            ladosMaximos.pop();
        }

    } while ( vertice != verticeObj);

    return acumulado;
}


int main(int argc, const char *argv[]) {

    //ofstream out("resultados.txt"); 
    int verticeObj = 0;
    map<int, vector< Arista* > > grafo;
    map<int, vector< Arista* > >::iterator it;
    vector<int> camino;

    if ( ! (stringstream(argv[2]) >> verticeObj) ) verticeObj = 0;

    cout << "Objetivo=" << verticeObj << endl;
    //cargarEjem(argv[1],grafo);
    cargarInstancias(argv[1],grafo);
    // Prueba del algoritmo greedy
    cout << "\nMaximo beneficio: " << solver(grafo,verticeObj,camino) << endl;

    // Resultado del camino
    imprimirCamino(camino);

    cout << "\nDEBUGGING\nImprimir mi grafo" << endl;
    for (it = grafo.begin(); it != grafo.end(); ++it) { // calls a_map.begin() and a_map.end()
        cout << it->first << '\n';
        imprimirVector(it->second);
    }

    // Liberar memoria
    for (it = grafo.begin(); it != grafo.end(); ++it) {
        borrarAristas(it->second);
    }
    cout << "\nMemoria liberada" << endl;



    
    //out.close();
    return 0;
}
