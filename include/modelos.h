#ifndef __MODELOS_H__
#define __MODELOS_H__

#include <vector>
#include <geometria.h>

class Modelo {
private:
    bool transparente;
    double indice_refraccion;
    unsigned char color;
    bool reflexivo;
    
public:
    Modelo();
    virtual ~Modelo() {
        
    }
    
    virtual std::vector<Punto3D> CalcularIntersecciones(const Semirecta3D &semirecta) const = 0;
    virtual Vector3D Normal(const Punto3D &punto_interseccion) const = 0;
    virtual Vector2D CalcularCoordenadaTextura(const Punto3D &punto_interseccion) const = 0;
    
    bool esTransparente() const;
    void esTransparente(bool transparente);
    
    bool esReflexivo() const;
    void esReflexivo(bool reflexivo);
    
    double getIndiceRefraccion() const;
    void setIndiceRefraccion(double indice_refraccion);
    
    unsigned char getColor() const;
    void setColor(unsigned char color);
};


class ModeloTriangulo : public Modelo {
private:
    Triangulo3D triangulo;
    Punto2D coords_tex[3];
    
public:
    ModeloTriangulo(const Punto3D &p1, const Punto3D &p2, const Punto3D &p3, const Punto2D coords_tex[3]);
    
    std::vector<Punto3D> CalcularIntersecciones(const Semirecta3D &semirecta) const;
    Vector3D Normal(const Punto3D &punto_interseccion) const;
    Vector2D CalcularCoordenadaTextura(const Punto3D &punto_interseccion) const;
};

class ModeloEsfera : public Modelo {
private:
    double radio;
    Punto3D centro;
    
public:
    ModeloEsfera(double radio, const Punto3D &centro);
    
    std::vector<Punto3D> CalcularIntersecciones(const Semirecta3D &semirecta) const;
    Vector3D Normal(const Punto3D &punto_interseccion) const;
    Vector2D CalcularCoordenadaTextura(const Punto3D &punto_interseccion) const;
    
    double getRadio() const;
    void setRadio(double radio);
    
    Punto3D getCentro() const;
    void setCentro(const Punto3D &centro);
};

#endif
