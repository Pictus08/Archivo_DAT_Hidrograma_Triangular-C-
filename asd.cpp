#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

void cargarVariables(float &Qp, float &tc, float &Qb, int &tiempo)
{
    // Validación del valor del Caudal Punta
    do
    {
        cout << "Ingrese el valor del Caudal Punta -Qp-: ";
        cin >> Qp;
    } while (Qp <= 0);

    // Validación del valor del tiempo de concentración
    do
    {
        cout << "Ingrese el valor del Tiempo de Concentracion -tc- (en minutos): ";
        cin >> tc;
    } while (tc <= 0);

    // Validación del valor del Caudal Base
    do
    {
        cout << "Ingrese el Caudal Base -Qb- ( 0 en caso de no tener definido uno): ";
        cin >> Qb;
    } while (Qb < 0);

    // Validación del intervalo de tiempo para calcular el hidrograma
    do
    {
        cout << "Ingrese el intervalo de tiempo para calcular el hidrograma (en minutos): ";
        cin >> tiempo;
    } while (tiempo <= 0 || tiempo != static_cast<int>(tiempo)); // Validación de que tiempo sea un entero mayor a cero
}

vector<string> cargarVectorTiempo(float tiempoBase, int tiempo)
{
    vector<string> vectorTiempo;

    // Variables para el cálculo del tiempo
    int horas = 0;
    int minutos = 0;
    int longitud = tiempoBase / tiempo;

    // Bucle para cargar el vector de tiempo
    for (int i = 0; i < longitud; ++i)
    {
        // Formatear la salida en formato "HH:MM"
        stringstream ss;
        ss << setw(2) << setfill('0') << horas << ":" << setw(2) << setfill('0') << minutos;
        vectorTiempo.push_back(ss.str());

        minutos += tiempo; // Sumar el intervalo de tiempo en minutos

        horas = minutos / 60;   // Ajustar las horas
        minutos = minutos % 60; // Ajustar los minutos
    }

    return vectorTiempo;
}

vector<float> cargaVectorCaudales(float tiempoBase, int tiempo, float Qp, float tc, float factor, float Qb)
{
    vector<float> vectorCaudales;

    int longitud = tiempoBase / tiempo;
    int posqp = ceil((tc / tiempo));
    float pendiente01 = Qp / tc;
    int intervalo = 0;
    float pendiente02 = Qp / (factor * tc);

    int tcDiscreto = ((posqp - 1) * tiempo);
    float valor = 0;

    for (int i = 0; i < longitud; ++i)
    {

        if (i < posqp)
        {
            vectorCaudales.push_back(intervalo * pendiente01 + Qb);
        }
        if (i == posqp)
        {
            vectorCaudales.push_back(Qp + Qb);
        }
        if (i > posqp)
        {
            valor = pendiente02 * (factor * tc - (intervalo - tcDiscreto));

            if (valor >= Qb)
            {
                vectorCaudales.push_back(valor);
            }
            else
            {
                vectorCaudales.push_back(Qb);
            }
        }

        intervalo = intervalo + tiempo;
    }

    return vectorCaudales;
}

vector<vector<string>> crearMatrizTiempoCaudales(const vector<string> &vectorTiempo, const vector<float> &vectorCaudales)
{
    vector<vector<string>> matrizTiempoCaudales;

    // Comprobar si los vectores tienen la misma longitud
    if (vectorTiempo.size() != vectorCaudales.size())
    {
        cerr << "Error: Los vectores tienen longitudes diferentes." << endl;
        return matrizTiempoCaudales;
    }

    // Crear la matriz combinando los vectores
    for (size_t i = 0; i < vectorTiempo.size(); ++i)
    {
        matrizTiempoCaudales.push_back({vectorTiempo[i], to_string(vectorCaudales[i])});
    }

    return matrizTiempoCaudales;
}

void escribirArchivoDAT(const vector<vector<string>> &matriz, const string &nombreArchivo)
{
    ofstream archivo(nombreArchivo);

    if (!archivo)
    {
        cerr << "Error al abrir el archivo." << endl;
        return;
    }

    // Escribir los datos en el archivo
    for (const auto &fila : matriz)
    {
        for (const auto &elemento : fila)
        {
            archivo << elemento << " ";
        }
        archivo << endl;
    }

    archivo.close();
}

int main()
{

    float Qp = 0;
    float tc = 0;
    float Qb = 0;
    int tiempo = 0;
    float tiempoBase = 0;

    // Llamada a la función para cargar las variables
    cargarVariables(Qp, tc, Qb, tiempo);

    float factor; // Factor de descarga del hidrograma
    cout << "Ingrese el factor de descarga del hidrograma (1,5 o bien 1,65 del tiempo de concentracion por ejemplo): " << endl;
    do
    {
        cin >> factor;

        if (factor < 0)
        {
            cout << "El valor representa el tiempo de descarga del hidrograma, debe ser mayor a 0. Ingrese nuevamente: ";
        }

    } while (factor < 0);

    // Calcular tiempoBase
    tiempoBase = (1 + factor) * tc;
    // Discretiza la varibale tiempo base a valores según el intervalo de tiempo en que se quiera construir el hidrograma
    tiempoBase = ceil(tiempoBase / tiempo) * tiempo;

    // Llamar a la función para cargar el vector de tiempo
    vector<string> vectorTiempo = cargarVectorTiempo(tiempoBase, tiempo);

    // Llamar a la función que carga los caudales
    vector<float> vectorCaudales = cargaVectorCaudales(tiempoBase, tiempo, Qp, tc, factor, Qb);

    // Crear la matriz tiempo-caudales
    vector<vector<string>> matrizTiempoCaudales = crearMatrizTiempoCaudales(vectorTiempo, vectorCaudales);

    // Escribir la matriz en un archivo DAT
    escribirArchivoDAT(matrizTiempoCaudales, "datos.DAT");

    cout << "Archivo DAT generado exitosamente." << endl;

    system("pause");

    return 0;
}
