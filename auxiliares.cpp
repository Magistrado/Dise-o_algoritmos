/*
    Nombre: Edgar Valderrama
            Daniel Leones       09-10977
    Fecha: 25/3/2017
    Descripción: Resolución de PRPP usando un algoritmo bab. Estructuras y 
    operaciones conexas del programa.
*/

using namespace std;

/*
    Funciones auxiliares para dividir lineas 
*/
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

/*
    Estructuras para las aristas
*/
struct Arista {
    int v1;
    int v2;
    int c;
    int b;
    int t;
    int ida;
    int vuelta;
};

/*
    Es usado para ordenar la lista de sucesores 
*/
class Comparar
{
public:
    bool operator()(Arista* a, Arista * b)
    {
        return a->b - a->c < b->b - b->c;
    }
};

/*
    Clase contenedor para los datos y operaciones comunes al grafo PRPP
*/

class Grafo
{
public:
    map<int, vector< Arista* > > graf;  // Contenedor del grafo
    int maxBen;                         // Maximo beneficio del grafo 

    Grafo() {
        maxBen = 0;
    }

    void obtSucesores(int v, vector<Arista * > &lsIncidentes) {
        lsIncidentes = vector<Arista*>(graf[v].begin(), graf[v].end());
        sort(lsIncidentes.begin(), lsIncidentes.end(), Comparar());
    }

    void cargarInstancias(string camino) 
    {
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

        for (int i = 0; i < nroLados; ++i)
        {
            in >> v1 >> v2 >> c >> b;
            //cout << v1 << " " << v2 << endl;
            maxBen += b - c;
            lado = new Arista { v1, v2, c, b, 0, 0, 0};
            graf[v1].push_back(lado);
            graf[v2].push_back(lado);
        }

        //in.clear();
        // Se usa para hacer avanzar el cursor sino no lee
        getline(in, entrada);
        getline(in, entrada);
        entradaDividida = split(entrada, ' ');

        if ( ! (stringstream(entradaDividida.back()) 
            >> nroLados) ) nroLados = 0;

        for (int i = 0; i < nroLados; ++i)
        {
            in >> v1 >> v2 >> c >> b;
            //cout << v1 << " " << v2 << endl;
            lado = new Arista { v1, v2, c, b, 0, 0, 0};
            maxBen += b - c;
            graf[v1].push_back(lado);
            graf[v2].push_back(lado);
        }
        in.close();
    }


    /*
        Cargador de instancias para el ejemplo del enunciado
    */
    void cargarEjem(string camino) {
        ifstream in(camino);
        int v1, v2, c, b;
        Arista* lado;
        while (in >> v1 >> v2 >> c >> b) {
            //cout << "Leido " << v1 << " " << v2 << endl;
            lado = new Arista { v1, v2, c, b, 0 };
            graf[v1].push_back(lado);
            graf[v2].push_back(lado);
        }
        in.close();
    }


    void imprimirGrafo() {
        for (auto it = graf.begin(); it != graf.end(); ++it) 
        {   
            cout << "Vertice: " << it->first << endl;
            for (auto i = it->second.begin(); i != it->second.end(); ++i)
            {
                cout << "\tv1: " << (*i)->v1 << " v2: " << (*i)->v2 << " Costo: " 
                    << (*i)->c << " Beneficio: "
                    << (*i)->b << " #: " << (*i)->t << " Ida: " << (*i)->ida
                    << " Vuelta: " << (*i)->vuelta << endl;
            }
        }
    }

    ~Grafo()
    {
        for (auto it = graf.begin(); it != graf.end(); ++it) {
            for (vector<Arista*>::iterator i = (it->second).begin(); 
                    i != (it->second).end(); ++i) 
            {
                if (*i == 0){
                    delete *i;
                    *i = 0;
                }
            }
        }
        cout << "Grafo destruido" << endl;
    }
};
