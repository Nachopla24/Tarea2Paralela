#include <iostream>
#include <ctime>
#include <math.h>
#include <mpi.h>

const int tamano = 10, MAXIMO = 10;//el tamaño del arreglo y los valores [0 y maximo[

using namespace std;

void llenarArreglo(int []);
void splitArreglo(int [], int [], int[]);
void enviarArreglos(int, int, int [], int[], float, float);

void verArreglos(int [], int [], int[]);
float promedioArreglo(int []);
float varianzaArreglo(int []);

int main(int argc, char** argv)
{
    MPI_Init(&argc,&argv);
    int size, rank;
    int arregloRandom[tamano];
    int fir[tamano/2], sec[tamano/2];
    float promProc1,promProc2;
    float promedio = 0, varianza = 0;

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    if (rank==0) {
      cout << "\nTarea 2 Computacion Paralela\n\n";
      cout << "Llenado.......\n";
      llenarArreglo(arregloRandom);
      splitArreglo(arregloRandom,fir,sec);
      enviarArreglos(rank,size,fir,sec,promProc1,promProc2);
      promedio = (promProc1+promProc2)/2;
      varianza = varianzaArreglo(arregloRandom);

      // cout << "El promedio es: " << promedio << endl;
      // cout << "La varianza es: " << varianza << endl;
      // cout << "La desviacion estandar es: " << sqrt(varianza) << endl;
    }
    MPI_Finalize();
    return 0;
}



void llenarArreglo(int arregloRandom[])
{
  srand( time( 0 ) );
  for ( int i = 0; i < tamano; ++i ){//LLena con numeros al azar (tamano) entre 0 y maximo
    arregloRandom[ i ] = rand() % MAXIMO;
  }
}

void verArreglos(int auxiliar[], int fir[], int sec[])
{
  int i;
  cout << "Arreglo Original: \n";
  for(i = 0; i < tamano; ++i){
    cout << "Valor: " << auxiliar[i] << endl;
  }

  cout << "Arreglo Primera Mitad: \n";
  for(i = 0; i < tamano/2; ++i){
    cout << "Valor: " << fir[i] << endl;
  }

  cout << "Arreglo Segunda Mitad: \n";
  for(i = 0; i < tamano/2; ++i){
    cout << "Valor: " << sec[i] << endl;
  }
}

void splitArreglo(int auxiliar[], int fir[], int sec[])
{
  int i;
  for(i = 0; i < (tamano/2); ++i){
    fir[i] = auxiliar[i];
  }
  for(i = tamano/2; i < tamano; ++i){
    sec[i - (tamano/2)] = auxiliar[i];
  }
}

void enviarArreglos(int rank, int size, int fir[], int sec[], float promProc1, float promProc2)
{
     if (rank == 0){
         promProc1 = promedioArreglo(fir);
         MPI_Send(sec, (tamano/2), MPI_INT, 1, 0, MPI_COMM_WORLD);
     }else if (rank == 1){
         MPI_Recv(sec, (tamano/2), MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         promProc2 = promedioArreglo(sec);
         }
         cout << "Promedio 1: " << promProc1 << endl;
         cout << "Promedio 2: " << promProc2 << endl;
}

float promedioArreglo(int arregloRandom[])
{
  float aux = 0;
  for ( int i = 0; i < tamano; ++i ){
    aux += arregloRandom[i];
  }
  aux = aux/tamano;
  return aux;
}

float varianzaArreglo(int arregloRandom[])
{
  float aux = 0, promedio = 0, numerador = 0;
  promedio = promedioArreglo(arregloRandom);
  for ( int i = 0; i < tamano; ++i ){
    numerador = arregloRandom[i]-promedio;
    aux += numerador*numerador;
  }
  aux = aux/tamano;
  return aux;
}
