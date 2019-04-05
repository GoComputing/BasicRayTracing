#ifndef __ESCENA_H__
#define __ESCENA_H__

#include <modelos.h>
#include <vector>

union ColorRGB {
    double r;
    double g;
    double b;
    
    ColorRGB() = default;
    ColorRGB(double r, double g, double b);
};

class Escena {
private:
    std::vector<Modelo *> modelos;
    int tam_x;
    int tam_y;
    double min_x;
    double max_x;
    double min_y;
    double max_y;
    int max_frames;
    
protected:
    ColorRGB CalcularColor(const Semirecta3D &rayo, double indice_refraccion_original, unsigned int max_profundidad, bool entrando = true);
    
public:
    Escena(int tam_x, int tam_y, unsigned int max_frames);
    ~Escena();
    
    ColorRGB GenerarPixel(int x, int y, int id_frame);
};

#endif
