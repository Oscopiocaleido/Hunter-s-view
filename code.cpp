#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <iostream>

using namespace std;

int mejor_suma = -1;
int mejor_cantidad = 9999;
int mejor_oscuridad = 9999;
bool seleccion_actual[32];
bool mejor_seleccion[32];

struct Circulo{
    int color = 0;

    //(Para calcular centro y diametro)
    int min_fila = 9999;
    int max_fila = -1;
    int min_col = 9999;
    int max_col = -1;

    //Variables calculadas
    int centro_fila = 0;
    int centro_col = 0;
    int radio = 0;
    int diametro = 0;
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
                n_encontrados++;

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

void calcular(file_pgm &img, Circulo lista[], int M){

    //1.Ordenar
    for(int i = 0; i < M - 1; i++){
        for(int j = 0; j < M - i - 1; j++){
            if(lista[j].color > lista[j+1].color){
                Circulo aux = lista[j];
                lista[j] = lista [j+1];
                lista[j+1] = aux;
            }
        }
    }

    double sum_pi = 0.0;

    //2. Calcular Propiedades
    for(int k = 0; k < M; k++){
        lista[k].diametro = (lista[k].max_col - lista[k].min_col) + 1;
        lista[k].radio = (lista[k].diametro + 1) / 2;
        lista[k].centro_fila = (lista[k].min_fila + lista[k].max_fila) / 2;
        lista[k].centro_col = (lista[k].min_col + lista[k].max_col) / 2;

        int borde = 0;

        for(int i = lista[k].min_fila; i <= lista[k].max_fila; i++){
            for(int j = lista[k].min_col; j <= lista[k].max_col; j++){
                if(img.pixeles[i][j] == lista[k].color){
                    if(i == 0 || i == img.alto - 1 || j == 0 || j == img.ancho - 1 ||
                        img.pixeles[i-1][j] != lista[k].color || img.pixeles[i+1][j] != lista[k].color ||
                        img.pixeles[i][j-1] != lista[k].color || img.pixeles[i][j+1] != lista[k].color ){
                            borde++;
                    }
                }
            }
        }
        lista[k].circu = borde;
        lista[k].pi = 0.0;

        if(lista[k].diametro > 0)lista[k].pi = (double)lista[k].circu / lista[k].diametro;

        sum_pi += lista[k].pi;
    }

    //3. Imprimir Áreas
    double pi_promedio = 0.0;
    if(M > 0) pi_promedio = sum_pi / M;

    for(int k = 0; k < M; k++){
        double area = pi_promedio * (lista[k].radio * lista[k].radio);
        lista[k].area_normalizada = (int)ceil(area);
        cout<< lista[k].area_normalizada << "u"<<endl;
    }
    cout<<endl;
}

void optimizacion_mates2(int idx, int M, int T, Circulo lista[], int suma_actual, int cant_actual, int oscuridad_actual){
    if(suma_actual > T)return;

    bool es_mejor = false;

    if(suma_actual > mejor_suma) es_mejor = true;

    if(suma_actual == mejor_suma){
        if(cant_actual < mejor_cantidad){
            es_mejor = true;
        }
        else if(cant_actual == mejor_cantidad){
            if(oscuridad_actual < mejor_oscuridad){
                es_mejor = true;
            }
        }
    }

    if(es_mejor){
        mejor_cantidad = cant_actual;
        mejor_suma = suma_actual;
        mejor_oscuridad = oscuridad_actual;

        for(int i = 0; i < M; i++){
            mejor_seleccion[i] = seleccion_actual[i];
        }
    }

    if(idx == M)return;

    if(suma_actual + lista[idx].area_normalizada <= T){
        seleccion_actual[idx] = true;

        int nueva_oscuridad = (cant_actual == 0) ? idx : oscuridad_actual;

        optimizacion_mates2(idx + 1, M, T, lista, suma_actual + lista[idx].area_normalizada, cant_actual + 1, nueva_oscuridad);

        seleccion_actual[idx] = false;
    }
    optimizacion_mates2(idx + 1, M, T, lista, suma_actual, cant_actual, oscuridad_actual);
}

int main() {
    int N = 0, M = 0, T = 0;
    file_pgm img;

    cin>> N >> M >> T;

    ensamblar(N, img);

    Circulo circulos[32];
    analizar_circulos(img, circulos);

    calcular(img, circulos, M);

    for(int i = 0; i < 32; i++){
        seleccion_actual[i] = false;
        mejor_seleccion[i] = false;
    }

    optimizacion_mates2(0, M, T, circulos, 0, 0, 9999);

    bool primero = true;
    for(int i = 0; i < M; i++){
        if(mejor_seleccion[i] == true){
            if(!primero) cout << ",";
            cout<<"Circulo" << (i+1);
            primero = false;
        }
    }
    cout<<endl;

    return 0;
}
