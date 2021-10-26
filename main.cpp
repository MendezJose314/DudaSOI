#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <vector>
#include <algorithm>
#include <windows.h>
using namespace std;

//Prototipo Funciones
bool checarRepetido (vector<string>, string);
bool insertarAdyacencia(int, int, vector<vector<int>> &);

int buscarIndice(string, vector<string>);
int distanciaMinima (int[], bool[], int);

string mayusculas(string s);

void dijkstra(vector<vector<int>>, int, int, vector<string>);
void imprimirCamino (int[], int, vector<string>);
void imprimirDijkstra(int[], vector<string>, int, int, int[]);
void imprimirMatriz (vector<vector<int>> &, int);
void imprimirNombres (vector<string>);
void imprimirTodosCaminos(int, int, int, vector<vector<int>>, vector<string>, bool&);
void imprimirTodosCaminosAux(int, int, vector<vector<int>>, vector<string>, bool [], int[], int&, bool&);

void iniciarMatriz(vector<vector<int>> &, int);

int main()
{
    bool ruta = false;
    int numeroGrafos;
    int opcion1;
    string origen, destino;
    string texto;
    vector <string> nombres;

    ifstream archivoEntrada ("GrafoPrueba.txt");
    getline(archivoEntrada, texto);
    numeroGrafos = stoi(texto);
    vector <vector<int>> matrizAdyancencia(numeroGrafos, vector<int> (numeroGrafos, 0));
    iniciarMatriz(matrizAdyancencia, numeroGrafos);
    for (int i = 1; getline(archivoEntrada, texto); i++)
    {
        if (i <= numeroGrafos)
        {
            bool exito;
            exito = checarRepetido(nombres, mayusculas(texto));
            if (exito == false)
            {
                cout << "En la seccion de nombres del archivo hay un nombre repetido." << endl;
                return -2;
            }
            nombres.push_back(mayusculas(texto));
        }
        else
        {
            bool insertado;
            int num1, num2;
            num1 = buscarIndice(mayusculas(texto), nombres);
            getline(archivoEntrada, texto);
            num2 = buscarIndice(mayusculas(texto), nombres);
            insertado = insertarAdyacencia(num1, num2, matrizAdyancencia);
            if (insertado == false)
            {
                cout << "En la seccion de conexiones del archivo hay una conexion de o hacia un nodo inexistente." << endl;
                return -1;
            }
        }
    }
    do
    {
        system("cls");
        puts ("Bienvenido a Manejador de Grafos!");
        puts ("1.- Imprimir Matriz Adyacencias.");
        puts ("2.- Imprimir Vector de Nombres.");
        puts ("3.- Revisar camino mas corto a todos los destinos.");
        puts ("4.- Revisar todos los caminos a un destino.");
        puts ("0.- Salir.");
        cout << "Seleccione opcion deseada: ";
        cin >> opcion1;
        switch (opcion1)
        {
            case 1:
                system("cls");
                imprimirMatriz(matrizAdyancencia, numeroGrafos);
                cout << " " << endl;
            break;
            case 2:
                system ("cls");
                imprimirNombres(nombres);
                cout << " " << endl;
            break;
            case 3:
                system("cls");
                int indice;
                cout << "Indique que ciudad sera el origen: ";
                cin >> origen;
                indice = buscarIndice(mayusculas(origen), nombres);
                if (indice != -1)
                    dijkstra(matrizAdyancencia, numeroGrafos, indice, nombres);
                else
                    cout << "Esa ciudad no existe, por favor, intentelo de nuevo." << endl;
            break;
            case 4:
                system("cls");
                int indiceOrigen, indiceFinal;
                cout << "Indique que ciudad sera el origen: ";
                cin >> origen;
                indiceOrigen = buscarIndice(mayusculas(origen), nombres);
                cout << "Indique que ciudad sera el destino: ";
                cin >> destino;
                indiceFinal = buscarIndice(mayusculas(destino), nombres);
                if (indiceOrigen != -1 && indiceFinal != -1)
                {
                    imprimirTodosCaminos(indiceOrigen, indiceFinal, numeroGrafos, matrizAdyancencia, nombres, ruta);
                    if (ruta == false)
                        cout << "No hay ruta de " << nombres[indiceOrigen] << " a " << nombres[indiceFinal] << endl;
                }
                else
                    cout << "Alguna de las ciudades no existe, por favor intentelo de nuevo." << endl;
            break;
            case 0:
                cout << "De nada, vuelva pronto." << endl;
            break;
            default:
                cout << "Opcion no valida, intente de nuevo por favor." << endl;
            break;
        }
        system ("pause");
    }while(opcion1 != 0);
    return 0;
}

//Funciones

bool checarRepetido(vector<string> nombres, string texto)
{
    for (auto i = nombres.begin(); i != nombres.end(); ++i)
    {
        if (*i == texto)
            return false;
    }
    return true;
}

bool insertarAdyacencia(int num1, int num2, vector<vector<int>> &matriz)
{
    if (num1 == -1 || num2 == -1)
        return false;
    matriz[num1][num2] = 1;
    return true;
}

int buscarIndice(string nombre, vector<string> nombres)
{
    int num = 0;
    for (auto i = nombres.begin(); i != nombres.end(); ++i)
    {
        if (*i == nombre)
            return num;
        num++;
    }
    return -1;
}

int distanciaMinima(int distancia[], bool aux[], int cuantos)
{
    int minimo = INT_MAX, indiceMinimo;
    for (int i = 0; i < cuantos; i++)
    {
        if(aux[i] == false && distancia[i] <= minimo)
        {
            minimo = distancia[i];
            indiceMinimo = i;
        }
    }
    return indiceMinimo;
}

string mayusculas(string s)
{
    transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return toupper(c); });
    return s;
}

void dijkstra(vector<vector<int>> matriz, int cuantos, int indiceOrigen, vector<string> nombres)
{
    int distancia[cuantos];
    int padre[cuantos];
    bool aux[cuantos];

    for (int i = 0; i < cuantos; ++i)
    {
        padre[i] = -1;
        distancia[i] = INT_MAX;
        aux[i] = false;
    }

    distancia[indiceOrigen] = 0;
    for (int i = 0; i < cuantos - 1; ++i)
    {
        int u = distanciaMinima(distancia, aux, cuantos);
        aux[u] = true;
        for (int j = 0; j < cuantos; ++j)
        {
            if(!aux[j] && matriz[u][j] && distancia[u] != INT_MAX && distancia[u] + matriz[u][j] < distancia[j])
            {
                padre[j] = u;
                distancia[j] = distancia[u] + matriz[u][j];
            }
        }
    }
    imprimirDijkstra(distancia, nombres, indiceOrigen, cuantos, padre);
}

void imprimirCamino(int padre[], int posicion, vector<string> nombres)
{
    if(padre[posicion] < 0)
        return;
    imprimirCamino(padre, padre[posicion], nombres);
    cout << "-->" << nombres[posicion];
}

void imprimirDijkstra(int distancia[], vector<string> nombres, int indice, int cuantos, int padre[])
{
    cout << "Ciudad \t\t\t            Distancia desde " << nombres[indice] << " \t Camino" << endl;
    for (int i = 0; i < cuantos; ++i)
    {
        printf ("\n");
        if (distancia[i] != INT_MAX)
        {
            cout << nombres[indice] << " --> " << nombres[i] << " \t\t\t          " << distancia[i] << "\t\t  " << nombres[indice];
            imprimirCamino(padre, i, nombres);
            cout << "" << endl;
        }
        else
           cout << nombres[i] << " \t\t" << "No hay ruta hacia esta ciudad" << "\t  " << "Al no haber ruta, no se muestra un camino" << endl;
    }

}

void imprimirMatriz(vector<vector<int>> &matriz, int cuantos)
{
    for (int i = 0; i < cuantos; ++i)
    {
        cout << endl;
        for (int j = 0; j < cuantos; ++j)
            cout << " | " << matriz[i][j] << " | ";
    }
}

void imprimirNombres(vector<string> nombres)
{
    for (auto i = nombres.begin(); i != nombres.end(); ++i)
        cout << *i << endl;
}

void iniciarMatriz(vector<vector<int>> &matriz, int cuantos)
{
    for (int i = 0; i < cuantos; ++i)
    {
        for (int j = 0; j < cuantos; ++j)
            matriz[i][j] = 0;
    }
}

void imprimirTodosCaminos(int origen, int destino, int cuantos, vector<vector<int>> matriz, vector<string> nombres, bool& ruta)
{
    bool* visitados = new bool[cuantos];
    int* camino = new int [cuantos];
    int indiceCamino = 0;

    for(int i = 0; i < cuantos; ++i)
        visitados[i] = false;
    imprimirTodosCaminosAux(origen, destino, matriz, nombres, visitados, camino, indiceCamino, ruta);
}

void imprimirTodosCaminosAux(int origen, int destino, vector<vector<int>> matriz, vector<string> nombres, bool visitados[], int camino[], int& indiceCamino, bool& ruta)
{
    visitados[origen] = true;
    camino[indiceCamino] = origen;
    indiceCamino++;
    if(origen == destino)
    {
        ruta = true;
        cout << nombres[camino[0]];
        for(int i = 1; i < indiceCamino; ++i)
            cout << " --> " << nombres[camino[i]];
        cout << endl;
    }
    else
    {
        int j = 0;
        for(auto i = matriz[origen].begin(); i != matriz[origen].end(); ++i, ++j)
        {
            if(!visitados[j] && *i != 0)
                imprimirTodosCaminosAux(j, destino, matriz, nombres, visitados, camino, indiceCamino, ruta);
        }
    }

    indiceCamino--;
    visitados[origen] = false;
}
