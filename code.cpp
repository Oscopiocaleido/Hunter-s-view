#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <iostream>

using namespace std;

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

int main() {
    int N = 0, M = 0, T = 0;
    file_pgm img;

    cin>> N >> M >> T;

    ensamblar(N, img);

    return 0;
}
