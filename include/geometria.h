#ifndef __GEOMETRIA_H__
#define __GEOMETRIA_H__

#include <iostream>
#include <herramientas.h>

struct Punto3D;

struct Vector2D {
    double x;
    double y;
    
    Vector2D() = default;
    Vector2D(double x, double y);
};

struct Vector3D {
    double x;
    double y;
    double z;
    
    Vector3D() = default;
    Vector3D(double x, double y, double z);
    Vector3D(const Punto3D &p);
};

struct Punto2D {
    double x;
    double y;
    
    Punto2D() = default;
    Punto2D(double x, double y);
    Punto2D(const Punto3D &p);
    
    Punto2D& operator=(const Punto3D &p);
};

struct Punto3D {
    double x;
    double y;
    double z;
    
    Punto3D() = default;
    Punto3D(const Vector3D &v);
    Punto3D(double x, double y, double z);
    
    bool operator!=(const Punto3D &p) const;
};


struct Semirecta3D {
    Vector3D directriz;
    Punto3D origen;
};

struct Triangulo2D {
    Punto2D vertices[3];
};

struct Triangulo3D {
    Punto3D vertices[3];
    
    Triangulo3D() = default;
    Triangulo3D(const Punto3D &p1, const Punto3D &p2, const Punto3D &p3);
};

struct Plano3D {
    Vector3D normal;
    double referencia;
};

struct Matriz3x3 {
    double elem[3][3];
    
    Matriz3x3() = default;
    Matriz3x3(const Vector3D &fila_1, const Vector3D &fila_2, const Vector3D &fila_3);
};



Vector3D operator-(const Punto3D &p1, const Punto3D &p2);
Vector2D operator-(const Punto2D &p1, const Punto2D &p2);



Vector3D NormalTriangulo(const Triangulo3D &t);



Vector3D ProductoVectorial(const Vector3D &v1, const Vector3D &v2);
double MultiplicarFilaPorVector(const double fila[3], const Vector3D &v);
Vector3D MultiplicarMatrizPorVector(const Matriz3x3 &m, const Vector3D &v);
Vector3D Normalizar(const Vector3D &v);
double ProductoEscalar(const Vector3D &v1, const Vector3D &v2);
double Modulo(const Vector2D &v);
double Modulo(const Vector3D &v);
Vector3D VectorOpuesto(const Vector3D &v);
Vector3D RotarVector(double angulo, const Vector3D &eje_rotacion, const Vector3D &v);
Vector3D MultiplicarVectorPorConstante(const Vector3D &v, double valor);
Vector3D SumarVectores(const Vector3D &v1, const Vector3D &v2);



Plano3D GenerarPlano(const Vector3D &v1, const Vector3D &v2, const Punto3D &referencia);
Plano3D GenerarPlanoTriangulo(const Triangulo3D &t);
void GenerarPlanosSemirecta(const Semirecta3D &semirecta, Plano3D res[2]);
Punto3D ConvertirA_PlanoXY(const Punto3D &p, const Triangulo3D &plano);
Triangulo2D ConvertirA_PlanoXY(const Triangulo3D &t);



Matriz3x3 TraspuestaMatriz(const Matriz3x3 &m);
double DeterminanteSubmatriz(const Matriz3x3 &m, unsigned int fil_eliminada, unsigned int col_eliminada);
double DeterminanteMatriz(const Matriz3x3 &m);
Matriz3x3 AdjuntaMatriz(const Matriz3x3 &m);
void MultiplicarMatrizPorConstante(Matriz3x3 &m, double c);
Matriz3x3 InversaMatriz(const Matriz3x3 &m);



double CalcularAngulo(const Vector3D &v1, const Vector3D &v2);
double CalcularAngulo(const Vector2D &v1, const Vector2D &v2);
bool PuntoDentroSemirecta(const Punto3D &p, const Semirecta3D &s);
Punto3D PuntoInvalido();
Punto3D CalcularInterseccion(const Semirecta3D &semirecta, const Triangulo3D &triangulo);
Punto3D ResolverEcuacion(const Plano3D &p1, const Plano3D &p2, const Plano3D &p3);
double Grados(double radianes);
bool Triangulo2D_ContienePunto(const Triangulo2D &t, const Punto2D &p);
bool Triangulo3D_ContienePunto(const Triangulo3D &t, const Punto3D &p);
double AreaTriangulo(const Triangulo3D &t);



std::ostream& operator<<(std::ostream &os, const Punto2D &p);
std::ostream& operator<<(std::ostream &os, const Punto3D &p);
std::ostream& operator<<(std::ostream &os, const Vector2D &v);
std::ostream& operator<<(std::ostream &os, const Vector3D &v);
std::ostream& operator<<(std::ostream &os, const Semirecta3D &s);
std::ostream& operator<<(std::ostream &os, const Triangulo2D &t);
std::ostream& operator<<(std::ostream &os, const Triangulo3D &t);
std::ostream& operator<<(std::ostream &os, const Plano3D &p);
std::ostream& operator<<(std::ostream &os, const Matriz3x3 &m);

#endif
