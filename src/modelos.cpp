#include <modelos.h>
#include <cmath>

Modelo::Modelo() {
    transparente = false;
    indice_refraccion = 1;
    color = 0;
}
    
bool Modelo::esTransparente() const {
    return transparente;
}
    
void Modelo::esTransparente(bool transparente) {
    this->transparente = transparente;
}
    
bool Modelo::esReflexivo() const {
    return reflexivo;
}
    
void Modelo::esReflexivo(bool reflexivo) {
    this->reflexivo = reflexivo;
}
    
double Modelo::getIndiceRefraccion() const {
    return indice_refraccion;
}
    
void Modelo::setIndiceRefraccion(double indice_refraccion) {
    this->indice_refraccion = indice_refraccion;
}
    
unsigned char Modelo::getColor() const {
    return color;
}
    
void Modelo::setColor(unsigned char color) {
    this->color = color;
}







ModeloTriangulo::ModeloTriangulo(const Punto3D &p1, const Punto3D &p2, const Punto3D &p3, const Punto2D coords_tex[3]) {
    triangulo.vertices[0] = p1;
    triangulo.vertices[1] = p2;
    triangulo.vertices[2] = p3;
    this->coords_tex[0] = coords_tex[0];
    this->coords_tex[1] = coords_tex[1];
    this->coords_tex[2] = coords_tex[2];
}
    
std::vector<Punto3D> ModeloTriangulo::CalcularIntersecciones(const Semirecta3D &semirecta) const {
    std::vector<Punto3D> res;
    Punto3D interseccion;
    interseccion = CalcularInterseccion(semirecta, triangulo);
    if(interseccion != PuntoInvalido())
        res.push_back(interseccion);
    return res;
}
    
Vector3D ModeloTriangulo::Normal(const Punto3D &/*punto_interseccion*/) const {
    return Normalizar(ProductoVectorial(triangulo.vertices[1] - triangulo.vertices[0], triangulo.vertices[2] - triangulo.vertices[0]));
}

Vector2D ModeloTriangulo::CalcularCoordenadaTextura(const Punto3D &punto_interseccion) const {
    Punto3D v0 = triangulo.vertices[0];
    Punto3D v1 = triangulo.vertices[1];
    Punto3D v2 = triangulo.vertices[2];
    Punto3D p  = punto_interseccion;
    double a_total = AreaTriangulo(triangulo);
    double a0 = AreaTriangulo(Triangulo3D(v0, v1, p)) / a_total;
    double a1 = AreaTriangulo(Triangulo3D(v1, v2, p)) / a_total;
    double a2 = AreaTriangulo(Triangulo3D(v0, v2, p)) / a_total;
    Vector2D res;
    
    res.x = a0*coords_tex[2].x + a1*coords_tex[0].x + a2*coords_tex[1].x;
    res.y = a0*coords_tex[2].y + a1*coords_tex[0].y + a2*coords_tex[1].y;
    
    return res;
}






ModeloEsfera::ModeloEsfera(double radio, const Punto3D &centro) {
    this->radio = radio;
    this->centro = centro;
}
    
std::vector<Punto3D> ModeloEsfera::CalcularIntersecciones(const Semirecta3D &semirecta) const {
    std::vector<Punto3D> res;
    Punto3D  interseccion;
    Plano3D planos_recta[2];
    Matriz3x3 trasladar;
    Matriz3x3 cambio_base;
    Vector3D centro_esfera;
    Vector3D eje_x;
    Vector3D eje_y;
    Vector3D eje_z;
    double distancia_origen;
    double diametro = 2.0 * radio;
    
    GenerarPlanosSemirecta(semirecta, planos_recta);
    eje_x = Normalizar(semirecta.directriz);
    eje_y = Normalizar(planos_recta[0].normal);
    eje_z = Normalizar(ProductoVectorial(eje_x, eje_y));
    cambio_base = Matriz3x3(eje_x, eje_y, eje_z);
    
    centro_esfera = MultiplicarMatrizPorVector(cambio_base, centro - semirecta.origen);
    distancia_origen = Modulo(Vector3D(0.0, centro_esfera.y, centro_esfera.z));
    
    if(distancia_origen < radio && centro_esfera.x >= radio) {
        double sol_x_1 =  std::sqrt(radio*radio - distancia_origen*distancia_origen) + centro_esfera.x;
        double sol_x_2 = -std::sqrt(radio*radio - distancia_origen*distancia_origen) + centro_esfera.x;
        res.push_back(Punto3D(sol_x_1, 0.0, 0.0));
        if(centro_esfera.x > diametro)
            res.push_back(Punto3D(sol_x_2, 0.0, 0.0));
        
        cambio_base = InversaMatriz(cambio_base);
        for(unsigned int i = 0; i < res.size(); ++i) {
            Vector3D punto_final;
            punto_final = MultiplicarMatrizPorVector(cambio_base, res[i] - Punto3D(0, 0, 0));
            res[i].x = punto_final.x + semirecta.origen.x;
            res[i].y = punto_final.y + semirecta.origen.y;
            res[i].z = punto_final.z + semirecta.origen.z;
        }
    }
    
    return res;
}

Vector3D ModeloEsfera::Normal(const Punto3D &punto_interseccion) const {
    Vector3D normal = Normalizar(punto_interseccion - centro);
    return normal;
}

Vector2D ModeloEsfera::CalcularCoordenadaTextura(const Punto3D &punto_interseccion) const {
    Vector3D respecto_origen = punto_interseccion - centro;
    double angulo_xz = CalcularAngulo(Vector3D(1.0, 0.0, 0.0), Vector3D(respecto_origen.x, 0.0, respecto_origen.z));
    double angulo_y  = CalcularAngulo(Vector3D(0.0, -1.0, 0.0), respecto_origen);
    Vector2D res;
    res.x = angulo_xz / (2.0 * M_PI);
    res.y = angulo_y  / (2.0 * M_PI);
    return res;
}

double ModeloEsfera::getRadio() const {
    return radio;
}
    
void ModeloEsfera::setRadio(double radio) {
    this->radio = radio;
}
    
Punto3D ModeloEsfera::getCentro() const {
    return centro;
}
    
void ModeloEsfera::setCentro(const Punto3D &centro) {
    this->centro = centro;
}
