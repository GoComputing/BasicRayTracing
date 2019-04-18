#include <limits>
#include <cmath>
#include <escena.h>
#include <fisica.h>

bool imprimir = false;

ColorRGB::ColorRGB(double r, double g, double b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

ColorRGB Escena::CalcularColor(const Semirecta3D &rayo, double indice_refraccion_original, unsigned int max_profundidad, bool entrando) {
    const double BIAS = 0.0001;
    std::vector<Punto3D> intersecciones;
    double menor_distancia;
    const Modelo *a_imprimir = nullptr;
    double distancia;
    unsigned int i, j;
    ColorRGB color;
    Punto3D punto_interseccion;
    size_t id_parte_sel = -1;
    
    if(imprimir) {
        std::cout << "\nBARRERA. MÁXIMA PROFUNDIDAD: " << max_profundidad << std::endl;
    }
    
    menor_distancia = std::numeric_limits<double>::max();
    for(i = 0; i < modelos.size(); ++i) {
        intersecciones = modelos[i]->CalcularIntersecciones(rayo);
        j = 0;
        for(const Punto3D &interseccion : intersecciones) {
            distancia = Modulo(interseccion - rayo.origen);
            if(distancia < menor_distancia) {
                menor_distancia = distancia;
                a_imprimir = modelos[i];
                punto_interseccion = interseccion;
                id_parte_sel = j;
            }
            ++j;
        }
    }
    if(a_imprimir != nullptr) {
        if(imprimir) {
            std::cout << "ID triángulo: " << id_parte_sel << std::endl;
            if(dynamic_cast<const ModeloCompuesto*>(a_imprimir) != nullptr)
                std::cout << "Rebotado contra cubo" << std::endl;
            else
                std::cout << "Rebotado con otra cosa" << std::endl;
        }
        
        if((a_imprimir->esReflexivo() || a_imprimir->esTransparente()) && max_profundidad != 0) {
            Semirecta3D rayo_rebote;
            Vector3D desplazamiento;
            bool reflexivo = a_imprimir->esReflexivo();
            Vector3D normal = a_imprimir->Normal(id_parte_sel);
            if(!entrando)
                normal = VectorOpuesto(normal);
            if(a_imprimir->esReflexivo()) {
                rayo_rebote.directriz = Reflexionar(Normalizar(punto_interseccion - rayo.origen),
                                                    normal);
            } else {
                rayo_rebote.directriz = Refractar(Normalizar(punto_interseccion - rayo.origen),
                                                  normal,
                                                  entrando ? indice_refraccion_original : a_imprimir->getIndiceRefraccion(), 
                                                  entrando ? a_imprimir->getIndiceRefraccion() : indice_refraccion_original);
            }
            desplazamiento = rayo_rebote.directriz;
            desplazamiento.x *= BIAS;
            desplazamiento.y *= BIAS;
            desplazamiento.z *= BIAS;
            rayo_rebote.origen = punto_interseccion;
            rayo_rebote.origen.x += desplazamiento.x;
            rayo_rebote.origen.y += desplazamiento.y;
            rayo_rebote.origen.z += desplazamiento.z;
            
            if(imprimir) {
                std::cout << "Intersección: " << punto_interseccion << std::endl;
                std::cout << "Rayo:   " << rayo << std::endl;
                std::cout << "Normal: " << a_imprimir->Normal(punto_interseccion) << std::endl;
                std::cout << "Rebote: " << rayo_rebote << std::endl;
            }
            
            color = CalcularColor(rayo_rebote, indice_refraccion_original, max_profundidad-1, (reflexivo ? entrando : !entrando));
        } else {
            //double intensidad_color = a_imprimir->getColor() * ProductoEscalar(Normalizar(a_imprimir->Normal(punto_interseccion)),
            //                                                 Normalizar(Vector3D(1.0, -1.0, 1.0)));
            Vector2D coord_tex = a_imprimir->CalcularCoordenadaTextura(punto_interseccion);
            double intensidad_color = Modulo(coord_tex) / std::sqrt(2);
            //double intensidad_color = 511 * CalcularAngulo(Normalizar(a_imprimir->Normal(punto_interseccion)),
            //                             Vector3D(0.0, -1.0, 0.0)) / (2.0*M_PI);
            color = ColorRGB(intensidad_color, intensidad_color, intensidad_color);
        }
    } else {
        color = ColorRGB(1.0, 1.0, 1.0);
    }
    return color;
}
    
Escena::Escena(int tam_x, int tam_y, unsigned int max_frames) {
    Punto3D v1 = Punto3D(-2.0, -2.0, 0.0);
    Punto3D v2 = Punto3D(-2.0,  2.0, 0.0);
    Punto3D v3 = Punto3D( 2.0,  2.0, 0.0);
    Punto3D v4 = Punto3D( 2.0, -2.0, 0.0);
    Punto3D v5 = Punto3D(-2.0, -2.0, 2.0);
    Punto3D v6 = Punto3D(-2.0,  2.0, 2.0);
    Punto3D v7 = Punto3D( 2.0,  2.0, 2.0);
    Punto3D v8 = Punto3D( 2.0, -2.0, 2.0);
    Punto2D coord_0 = Punto2D(0.0, 0.0);
    Punto2D coord_1 = Punto2D(1.0, 0.0);
    Punto2D coord_2 = Punto2D(0.0, 1.0);
    Punto2D coord_3 = Punto2D(1.0, 1.0);
    Punto2D coords_tex[10][3] = {
                                 {coord_0, coord_1, coord_3},
                                 {coord_0, coord_3, coord_2},
                                 {coord_1, coord_3, coord_0},
                                 {coord_0, coord_3, coord_2},
                                 {coord_0, coord_1, coord_3},
                                 {coord_0, coord_3, coord_2},
                                 {coord_0, coord_1, coord_3},
                                 {coord_0, coord_3, coord_2}, 
                                 {coord_0, coord_1, coord_3},
                                 {coord_0, coord_3, coord_2}
    };
    
    modelos.push_back(new ModeloTriangulo(v1, v5, v6, coords_tex[0])); // -X
    modelos.push_back(new ModeloTriangulo(v1, v6, v2, coords_tex[1]));
    modelos.push_back(new ModeloTriangulo(v4, v3, v8, coords_tex[2])); // +X
    modelos.push_back(new ModeloTriangulo(v8, v3, v7, coords_tex[3]));
    modelos.push_back(new ModeloTriangulo(v1, v4, v8, coords_tex[4])); // -Y
    modelos.push_back(new ModeloTriangulo(v1, v8, v5, coords_tex[5]));
    modelos.push_back(new ModeloTriangulo(v6, v7, v3, coords_tex[6])); // +Y
    modelos.push_back(new ModeloTriangulo(v6, v3, v2, coords_tex[7]));
    modelos.push_back(new ModeloTriangulo(v5, v8, v7, coords_tex[8])); // +Z
    modelos.push_back(new ModeloTriangulo(v5, v7, v6, coords_tex[9]));
    modelos.push_back(new ModeloEsfera(0.5, Punto3D( 1.5, -1.5,  0.0)));
    modelos.push_back(new ModeloEsfera(0.5, Punto3D(-1.5, -1.5,  0.0)));
    modelos.push_back(new ModeloCompuesto(CargarPLY("res/modelos/diamond.ply")));
    
    modelos[0]->setColor(64);
    modelos[1]->setColor(64);
    modelos[2]->setColor(180);
    modelos[3]->setColor(180);
    modelos[4]->setColor(100);
    modelos[5]->setColor(100);
    modelos[6]->setColor(200);
    modelos[7]->setColor(200);
    modelos[8]->setColor(140);
    modelos[9]->setColor(140);
    modelos[10]->setColor(255);
    modelos[11]->setColor(128);
    modelos[12]->setColor(255);
    
    modelos[10]->esReflexivo(false);
    modelos[10]->esTransparente(true);
    modelos[10]->setIndiceRefraccion(1.1);
    
    modelos[11]->esReflexivo(true);
    modelos[11]->esTransparente(false);
    modelos[11]->setIndiceRefraccion(1.1);
    
    modelos[12]->esReflexivo(false);
    modelos[12]->esTransparente(true);
    modelos[12]->setIndiceRefraccion(1.2);
    
    this->tam_x = tam_x;
    this->tam_y = tam_y;
    this->max_frames = max_frames;
    min_x = -1.0;
    max_x =  1.0;
    min_y = -1.0;
    max_y =  1.0;
}

Escena::~Escena() {
    size_t i;
    for(i = 0; i < modelos.size(); ++i)
        delete modelos[i];
}
    
ColorRGB Escena::GenerarPixel(int x, int y, int id_frame) {
    double radio = 1.5;
    double angulo;
    Semirecta3D rayo;
    Punto3D destino;
    
    angulo = 2.0*M_PI*(double)id_frame / (double)max_frames + M_PI;
    dynamic_cast<ModeloEsfera *>(modelos[10])->setCentro(Punto3D( radio*std::cos(angulo), -1.5, radio*std::sin(angulo)));
    angulo += M_PI;
    dynamic_cast<ModeloEsfera *>(modelos[11])->setCentro(Punto3D( radio*std::cos(angulo), -1.5, radio*std::sin(angulo)));
    
    rayo.origen = Punto3D(0.0, 0.0, -3.3);
    destino = Punto3D(Interpolar(x, 0, tam_x, min_x, max_x),
                      Interpolar(y, 0, tam_y, min_y, max_y),
                      -2.0);
    rayo.directriz = destino - rayo.origen;
    
    imprimir = (x-1) == this->tam_x/2 && y == this->tam_y/2;
    return CalcularColor(rayo, 1.0, 64);
}
