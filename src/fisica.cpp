#include <cmath>
#include <fisica.h>

Vector3D Refractar(const Vector3D &direccion, const Vector3D &normal, double indice_refraccion_anterior, double indice_refraccion_actual) {
    Vector3D opuesto = VectorOpuesto(direccion);
    Vector3D normal_opuesta = Normalizar(VectorOpuesto(normal));
    double ang_dir = CalcularAngulo(opuesto, normal);
    double nuevo_ang = std::fmod(std::asin(indice_refraccion_anterior * std::sin(ang_dir) / indice_refraccion_actual), 2.0*M_PI);
    Vector3D eje_rotacion = Normalizar(ProductoVectorial(normal_opuesta, opuesto));
    Vector3D res;
    if(nuevo_ang < 0.0)
        nuevo_ang += 2.0*M_PI;
    res = RotarVector(-nuevo_ang, eje_rotacion, normal_opuesta);
    return res;
}

Vector3D Reflexionar(const Vector3D &direccion, const Vector3D &normal) {
    Vector3D normalizado = Normalizar(normal);
    Vector3D opuesto = Normalizar(VectorOpuesto(direccion));
    double angulo = CalcularAngulo(normalizado, opuesto);
    Vector3D componente_x = MultiplicarVectorPorConstante(normalizado, std::cos(angulo));
    Vector3D opuesto_componente_x = VectorOpuesto(componente_x);
    Vector3D componente_y = SumarVectores(opuesto, opuesto_componente_x);
    Vector3D opuesto_componente_y = VectorOpuesto(componente_y);
    Vector3D reflejo = SumarVectores(componente_x, opuesto_componente_y);
    return reflejo;
}
