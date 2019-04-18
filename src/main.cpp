#include <iostream>
#include <fstream>
#include <vector>
#include "mpi.h"

#include <escena.h>
#include <geometria.h>
#include <fisica.h>
#include <modelos.h>




void AlmacenarPGM(std::ostream &os, const std::vector<unsigned char> &buffer, size_t tam_x, size_t tam_y) {
    unsigned int i, j;
    os << "P2\n";
    os << tam_x << ' ' << tam_x << '\n';
    os << 255 << '\n';
    for(i = 0; i < tam_y; ++i) {
        for(j = 0; j < tam_x; ++j)
            os << (unsigned int)buffer[i*tam_x + j] << ' ';
        os << '\n';
    }
}

int NumPixelesProceso(size_t tam_x, size_t tam_y, int id_proceso, int num_procesos) {
    int tam = (int)(tam_x * tam_y) / num_procesos;
    if(id_proceso < (tam%num_procesos))
        tam += 1;
    return tam;
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        std::cerr << "Sintaxis:" << std::endl;
        std::cerr << "  " << argv[0] << " <ruta-carpeta-salida> <n-fotogramas>" << std::endl;
        return -1;
    }
    
    // Variables referentes al resultado
    constexpr int TAM_X = 2048;
    constexpr int TAM_Y = 2048;
    std::string ruta_carpeta_salida = std::string(argv[1]) + "/";
    unsigned int max_frames = std::atoi(argv[2]);
    std::vector<unsigned char> buffer;
    std::vector<unsigned char> imagen_final;
    unsigned int i, j, nframe;
    unsigned int nfil, ncol;
    
    Escena escena = Escena(TAM_X, TAM_Y, max_frames);
    
    // Variables referentes a la paralelización
    const int ID_MASTER = 0;
    int id_proceso;
    int num_procesos;
    double tini, tend;
    double acumulador; // Acumula el tiempo de ejecución del código paralelo (sin E/S)
    int num_pixeles;
    
    // Inicializamos MPI
    acumulador = tiempo_actual();
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_proceso);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procesos);
    
    // Calculamos el número de píxeles asignado a este proceso
    num_pixeles = NumPixelesProceso(TAM_X, TAM_Y, id_proceso, num_procesos);
    buffer = std::vector<unsigned char>(num_pixeles);
    acumulador = tiempo_actual() - acumulador;
    
    // Generamos 'max_frames' fotogramas
    for(nframe = 0; nframe < max_frames; ++nframe) {
        tini = tiempo_actual();
        
        // Calculamos los píxeles que nos han sido asignados
        for(i = id_proceso; i < buffer.size(); ++i) {
            // Para que la imagen no salga volteada, restamos TAM_Y-1 al valor original
            nfil = TAM_Y-1 - (i*num_procesos+id_proceso)/TAM_X;
            ncol = (i*num_procesos+id_proceso)%TAM_X;
            if(i%1000 == 0 && id_proceso == 0) {
                std::cout << (100.0*(double)i/(double)buffer.size()) << std::endl;
            }
            buffer[i] = escena.GenerarPixel(ncol, nfil, nframe).r * 255;
        }
        
        // Esperamos a que se genere por completo la imagen
        MPI_Barrier(MPI_COMM_WORLD);
        
        if(id_proceso == ID_MASTER) {
            // Obtenemos los datos
            imagen_final = std::vector<unsigned char>(TAM_X*TAM_Y);
            for(i = 0; i < (unsigned int)num_procesos; ++i) {
                MPI_Status estado;
                std::vector<unsigned char> fragmento;
                if(i != ID_MASTER) {
                    fragmento = std::vector<unsigned char>(NumPixelesProceso(TAM_X, TAM_Y, i, num_procesos));
                    MPI_Recv(fragmento.data(), fragmento.size(), MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, &estado);
                } else {
                    fragmento = buffer;
                }
                for(j = 0; j < fragmento.size(); ++j)
                    imagen_final[j*num_procesos + i] = fragmento[j];
            }
            
            // Solo el master cuenta el tiempo
            tend = tiempo_actual();
            acumulador += tend-tini;
            
            // Almacenamos la imagen
            std::string ruta = ruta_carpeta_salida+(nframe<10?std::string("0"):std::string(""))+std::to_string(nframe)+".pgm";
            std::ofstream archivo (ruta);
            AlmacenarPGM(archivo, imagen_final, TAM_X, TAM_Y);
            archivo.close();
            
            std::cout << "Terminado el fotograma " << (nframe+1) << " de " << max_frames << std::endl;
        } else {
            // Enviamos los datos generados al master
            MPI_Ssend(buffer.data(), buffer.size(), MPI_UNSIGNED_CHAR, ID_MASTER, 0, MPI_COMM_WORLD);
        }
    }
    
    MPI_Finalize();
    
    if(id_proceso == ID_MASTER)
        std::cout << acumulador << std::endl;
    return 0;
}
