#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <iostream>

using namespace std;

struct Circulo{
    int color = 0;

    //(Para calcular centro y diametro)
    int min_fila = 999;
    int max_fila = -1;
    int min_col = 999;
    int max_col = -1;

    //Variables calculadas
    int centro_fila = 0;
    int centro_col = 0;
    int radio = 0;
    int diamtro = 0;
    int circu = 0;
    double pi = 0.0;
    int area_normalizada = 0;
};

struct file_pgm{
    int ancho = 0, alto = 0, valor_max = 0;
    int pixeles[256][256];
};

void objective_file(string file, file_pgm &img){
    ifstream archivo;
    archivo.open(file.c_str());
    if(!archivo.is_open()) return;

    string aux;

    archivo >> aux;
    archivo >> img.ancho;
    archivo >> img.alto;
    archivo >> img.valor_max;

    for(int i = 0; i < img.alto; i++){
        for(int j = 0; j < img.ancho; j++){
            archivo >> img.pixeles[i][j];
        }
    }
}

void ensamblar(int N, file_pgm &img){
    string objectives = "objectives";
    objectives = objectives + to_string(N);
    objectives = objectives + ".pgm";

    objective_file(objectives, img);
}

void analizar_circulos(file_pgm &img, Circulo lista[]){
    int n_encontrados = 0;

    for(int i = 0; i < img.alto; i++){
        for(int j = 0; j < img.ancho; j++){
            int pixel = img.pixeles[i][j];

            if(pixel == 0) continue;

            int index = -1;

            for(int p = 0; p < n_encontrados; p++){
                if(lista[p].color == pixel){
                    index = p;
                    break;
                }
            }

            if(index == -1){
                index = n_encontrados;
                n_encontrados ++;

                lista[index].color = pixel;
                lista[index].min_fila = i;
                lista[index].max_fila = i;
                lista[index].min_col = j;
                lista[index].max_col = j;
            }else {
                if(i < lista[index].min_fila) lista[index].min_fila = i;
                if(i > lista[index].max_fila) lista[index].max_fila = i;

                if(j < lista[index].min_col) lista[index].min_col = j;
                if(j > lista[index].max_col) lista[index].max_col = j;
            }
        }
    }
}

void calcular(file_pgm &img, Circulo lista[], int M, int T){

    //Ordenar colores
    for(int i = 0; i < M - 1; i++){
        for(int j = 0; j < M - i - 1; j++){
            if(lista[j].color > lista[j+1].color){
                Circulo aux = lista[j];
                lista[j] = lista[j+1];
                lista[j+1] = aux;
            }
        }
    }

    //Calcular
    double suma_pi = 0.0;
}

int main() {
    int N = 0, M = 0, T = 0;
    file_pgm img;

    cin>> N >> M >> T;

    ensamblar(N, img);

    Circulo circulos[32];
    analizar_circulos(img, circulos);

    calcular(img, circulos, M, T);
    return 0;
}
