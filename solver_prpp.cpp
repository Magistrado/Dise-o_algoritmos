/*
    Nombre: Edgar Valderrama    09-10870
            Daniel Leones       09-10977
    Fecha: 17/2/2017
    Descripción: Resolución de PRPP usando un algoritmo greedy.

    Ejecución: ./solver_greedy_prpp /camino/a/instancia 1
    Colocar 1 como segundo arg dado que se requiere que retorne al mismo punto donde
    parte. 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <queue>

using namespace std;

/*
    Función objeto para ordenamiento en la cola de prioridades usada por el algoritmo greedy
*/

class CompararGreddy
{
public:
    bool operator()(pair<int, Arista * > a, pair<int, Arista *> b)
    {
        return a.first < b.first;
    }
};

/*
    Algoritmo greedy a probar. Se apoya en una cola de prioridades para
    ordenar las aristas de mayor beneficio calculado incluyendo costos negativos. 
    Al tomar una decisión, pasa a otro vertice y borra la cola. 

    Retorna: el mayor beneficio alcanzado al regresar a 1.
*/

int solver_greedy(map<int, vector< Arista* > > &grafo, int verticeObj, vector<Arista *>&caminoAristas) 
{
    int acumulado = 0, vertice = 1, ben = 0;
    priority_queue< pair<int,Arista * >, // (beneficio calculado , _) 
        vector< pair<int, Arista * > >, CompararGreddy > ladosMaximos;
    //Iteradores 
    vector< Arista* >::iterator itv;
    Arista *arista;
       
    do
    {   
        //cout << "\nVertice:"  << vertice << " acumulado: " << acumulado << endl;
        for ( itv = grafo[vertice].begin(); itv != grafo[vertice].end(); ++itv)
        {   
            {
                if ((*itv)->t == 0) {
                    ben = (*itv)->b-((*itv)->c);
                } else {
                    ben = -((*itv)->c*(*itv)->t);
                }
                
                // Si ya fui y vine por el mismo camino no puedo volver a tomarlo.
                // Si fui de ida no puedo volver de ida, si fui de vuelta no puedo
                // volver a ir de vuelta por ese camino.
                if ((*itv)->v1 == vertice && (*itv)->ida != 1){
		  // Va de ida
		  ladosMaximos.push(pair<int, Arista *>(ben,*itv));
		} else if ((*itv)->v2 == vertice && (*itv)->vuelta != 1){
		  // Va de vuelta
		  ladosMaximos.push(pair<int, Arista *>(ben,*itv));
		} else if((*itv)->ida == 1 && (*itv)->vuelta == 1){
		}
            }
        }
        
        acumulado += ladosMaximos.top().first;
        // Contar visita para tal lado
        ladosMaximos.top().second->t += 1;
	
	
        //cout << "Proximo: ";
        if (ladosMaximos.top().second->v2 == vertice ) {
                //cout << "(" << ladosMaximos.top().second->v1 << "<--" 
                //          << ladosMaximos.top().second->v2 << ")" << endl;
                vertice = ladosMaximos.top().second->v1;
    	    ladosMaximos.top().second->vuelta = 1;
            arista = new Arista();
            arista->v1 = ladosMaximos.top().second->v2;
            arista->v2 = ladosMaximos.top().second->v1;
            arista->c = ladosMaximos.top().second->c;
            arista->b = ladosMaximos.top().second->b;
            arista->t = ladosMaximos.top().second->t;
            arista->ida = ladosMaximos.top().second->ida;
            arista->vuelta = ladosMaximos.top().second->vuelta;
    	} else if (ladosMaximos.top().second->v1 == vertice) {
                //cout << "(" << ladosMaximos.top().second->v1 << "-->" 
                //          << ladosMaximos.top().second->v2 << ")" << endl;
                vertice = ladosMaximos.top().second->v2;
    	    ladosMaximos.top().second->ida = 1;
            arista = new Arista();
            arista->v1 = ladosMaximos.top().second->v1;
            arista->v2 = ladosMaximos.top().second->v2;
            arista->c = ladosMaximos.top().second->c;
            arista->b = ladosMaximos.top().second->b;
            arista->t = ladosMaximos.top().second->t;
            arista->ida = ladosMaximos.top().second->ida;
            arista->vuelta = ladosMaximos.top().second->vuelta;
        }

        caminoAristas.push_back(arista);

        while (!ladosMaximos.empty()) {
            //imprimirParCola(ladosMaximos.top());
            ladosMaximos.pop();
        }

    } while ( vertice != verticeObj);

    return acumulado;
}

bool produceBeneficio_greedy(vector<Arista *>ciclo){
    int costoArista = 0;

    for (int i=0; i<ciclo.size();i++){

        if (ciclo[i]->ida == 1 && ciclo[i]->vuelta == 1){
            costoArista += - (ciclo[i]->c);
        } else {
            costoArista += (ciclo[i]->b) - (ciclo[i]->c);
        }
    }
    return costoArista > 0;
}

void eliminarCiclosNeg(vector<Arista *> &camino){
    Arista *eNext;

    vector<Arista *> ciclo;

    for (int i=1;i<camino.size();i++){
        eNext = camino[i];
        ciclo.clear();
        for (int j=i+1;j<camino.size();j++){
            ciclo.push_back(camino[j]);
            if(eNext->v2 == camino[j]->v2){
                if (!produceBeneficio_greedy(ciclo)){
                    camino.erase(camino.begin()+(i+1), camino.begin()+j);
                    break;
                }
            }
        }
    }
}

/*
    Programa principal.

*/
/*
int main(int argc, const char *argv[]) {

    //ofstream out("resultados.txt"); 
    int verticeObj = 0;
    map<int, vector< Arista* > > grafo;
    map<int, vector< Arista* > >::iterator it;
    vector<int> camino;
    vector<Arista *> caminoAristas;

    if ( ! (stringstream(argv[2]) >> verticeObj) ) verticeObj = 0;

    //cargarEjem(argv[1],grafo);
    cargarInstancias(argv[1],grafo);

    // Prueba del algoritmo greedy
    solver_greedy(grafo,verticeObj,camino,caminoAristas);
    // Resultado del camino
    //imprimirCamino(camino);

    // Mejorando la solucion
    eliminarCiclosNeg(caminoAristas);
    imprimirSolucion(caminoAristas);

    // Liberar memoria
    for (it = grafo.begin(); it != grafo.end(); ++it) {
        borrarAristas(it->second);
    }
    
    //out.close();
    return 0;
}
*/
