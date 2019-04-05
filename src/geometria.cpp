#include <cmath>
#include <geometria.h>

Vector3D::Vector3D(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}



Punto2D::Punto2D(double x, double y) {
    this->x = x;
    this->y = y;
}



Punto3D::Punto3D(const Vector3D &v) {
    x = v.x;
    y = v.y;
    z = v.z;
}
    
Punto3D::Punto3D(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
    
bool Punto3D::operator!=(const Punto3D &p) const {
    return FullCompareNotEqual(x, p.x) || FullCompareNotEqual(y, p.y) || FullCompareNotEqual(z, p.z);
}


Triangulo3D::Triangulo3D(const Punto3D &p1, const Punto3D &p2, const Punto3D &p3) {
    vertices[0] = p1;
    vertices[1] = p2;
    vertices[2] = p3;
}


Matriz3x3::Matriz3x3(const Vector3D &fila_1, const Vector3D &fila_2, const Vector3D &fila_3) {
    elem[0][0] = fila_1.x;
    elem[0][1] = fila_1.y;
    elem[0][2] = fila_1.z;
        
    elem[1][0] = fila_2.x;
    elem[1][1] = fila_2.y;
    elem[1][2] = fila_2.z;
        
    elem[2][0] = fila_3.x;
    elem[2][1] = fila_3.y;
    elem[2][2] = fila_3.z;
}








Vector3D operator-(const Punto3D &p1, const Punto3D &p2) {
    Vector3D res;
    res.x = p1.x - p2.x;
    res.y = p1.y - p2.y;
    res.z = p1.z - p2.z;
    return res;
}

Vector2D operator-(const Punto2D &p1, const Punto2D &p2) {
    Vector2D res;
    res.x = p1.x - p2.x;
    res.y = p1.y - p2.y;
    return res;
}






Vector3D ProductoVectorial(const Vector3D &v1, const Vector3D &v2) {
    Vector3D res;
    res.x = (v1.y * v2.z) - (v1.z * v2.y);
    res.y = (v1.z * v2.x) - (v1.x * v2.z);
    res.z = (v1.x * v2.y) - (v1.y * v2.x);
    return res;
}

double MultiplicarFilaPorVector(const double fila[3], const Vector3D &v) {
    return fila[0]*v.x + fila[1]*v.y + fila[2]*v.z;
}

Vector3D MultiplicarMatrizPorVector(const Matriz3x3 &m, const Vector3D &v) {
    Vector3D res;
    res.x = MultiplicarFilaPorVector(m.elem[0], v);
    res.y = MultiplicarFilaPorVector(m.elem[1], v);
    res.z = MultiplicarFilaPorVector(m.elem[2], v);
    return res;
}

Vector3D Normalizar(const Vector3D &v) {
    Vector3D res = v;
    double modulo = Modulo(v);
    res.x /= modulo;
    res.y /= modulo;
    res.z /= modulo;
    return res;
}

double ProductoEscalar(const Vector3D &v1, const Vector3D &v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

double Modulo(const Vector2D &v) {
    return Modulo(Vector3D(v.x, v.y, 0.0));
}

double Modulo(const Vector3D &v) {
    return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector3D VectorOpuesto(const Vector3D &v) {
    Vector3D res = v;
    res.x *= -1;
    res.y *= -1;
    res.z *= -1;
    return res;
}

Vector3D RotarVector(double angulo, const Vector3D &eje_rotacion, const Vector3D &v) {
    Vector3D u = Normalizar(eje_rotacion);
    Matriz3x3 m_rot;
    Vector3D res;
    double cos_ang = std::cos(angulo);
    double sin_ang = std::sin(angulo);
    
    m_rot.elem[0][0] = cos_ang + u.x*u.x * (1.0 - cos_ang);
    m_rot.elem[0][1] = u.x*u.y * (1.0 - cos_ang) - u.z*sin_ang;
    m_rot.elem[0][2] = u.x*u.z * (1.0 - cos_ang) + u.y*sin_ang;
    
    m_rot.elem[1][0] = u.x*u.y * (1.0 - cos_ang) + u.z*sin_ang;
    m_rot.elem[1][1] = cos_ang + u.y*u.y * (1.0 - cos_ang);
    m_rot.elem[1][2] = u.y*u.z * (1.0 - cos_ang) - u.x*sin_ang;
    
    m_rot.elem[2][0] = u.x*u.z * (1.0 - cos_ang) - u.y*sin_ang;
    m_rot.elem[2][1] = u.y*u.z * (1.0 - cos_ang) + u.x*sin_ang;
    m_rot.elem[2][2] = cos_ang + u.z*u.z * (1.0 - cos_ang);
    
    res = MultiplicarMatrizPorVector(m_rot, v);
    return res;
}

Vector3D MultiplicarVectorPorConstante(const Vector3D &v, double valor) {
    Vector3D res = v;
    res.x *= valor;
    res.y *= valor;
    res.z *= valor;
    return res;
}

Vector3D SumarVectores(const Vector3D &v1, const Vector3D &v2) {
    Vector3D res;
    res.x = v1.x + v2.x;
    res.y = v1.y + v2.y;
    res.z = v1.z + v2.z;
    return res;
}












Plano3D GenerarPlano(const Vector3D &v1, const Vector3D &v2, const Punto3D &referencia) {
    Plano3D res;
    Punto3D r = referencia;
    res.normal = ProductoVectorial(v1, v2);
    res.referencia = (res.normal.x * r.x) + (res.normal.y * r.y) + ( + res.normal.z * r.z);
    return res;
}

Plano3D GenerarPlanoTriangulo(const Triangulo3D &t) {
    Plano3D res;
    Vector3D v1 = t.vertices[1] - t.vertices[0];
    Vector3D v2 = t.vertices[2] - t.vertices[0];
    res = GenerarPlano(v1, v2, t.vertices[0]);
    return res;
}

void GenerarPlanosSemirecta(const Semirecta3D &semirecta, Plano3D res[2]) {
    double dx, dy, dz;
    double ox, oy, oz;
    bool dx_zero, dy_zero, dz_zero;
    dx = semirecta.directriz.x;
    dy = semirecta.directriz.y;
    dz = semirecta.directriz.z;
    ox = semirecta.origen.x;
    oy = semirecta.origen.y;
    oz = semirecta.origen.z;
    
    auto calcular_xy = [&]() -> Plano3D
                       {
                           Plano3D plano;
                           plano.normal.x = dy;
                           plano.normal.y = -dx;
                           plano.normal.z = 0.0;
                           plano.referencia = dy*ox - dx*oy;
                           return plano;
                       };
    
    auto calcular_xz = [&]() -> Plano3D
                       {
                           Plano3D plano;
                           plano.normal.x = dz;
                           plano.normal.y = 0.0;
                           plano.normal.z = -dx;
                           plano.referencia = dz*ox - dx*oz;
                           return plano;
                       };
    
    auto calcular_yz = [&]() -> Plano3D
                       {
                           Plano3D plano;
                           plano.normal.x = 0.0;
                           plano.normal.y = dz;
                           plano.normal.z = -dy;
                           plano.referencia = dz*oy - dy*oz;
                           return plano;
                       };
    
    dx_zero = std::fabs(dx) <= EPSILON;
    dy_zero = std::fabs(dy) <= EPSILON;
    dz_zero = std::fabs(dz) <= EPSILON;
    if(dx_zero && dy_zero) {
        res[0].normal = Vector3D(1, 0, 0);
        res[0].referencia = ox;
        res[1].normal = Vector3D(0, 1, 0);
        res[1].referencia = oy;
    } else if(dx_zero && dz_zero) {
        res[0].normal = Vector3D(1, 0, 0);
        res[0].referencia = ox;
        res[1].normal = Vector3D(0, 0, 1);
        res[1].referencia = oz;
    } else if(dy_zero && dz_zero) {
        res[0].normal = Vector3D(0, 1, 0);
        res[0].referencia = oy;
        res[1].normal = Vector3D(0, 0, 1);
        res[1].referencia = oz;
    } else if(dx_zero) {
        res[0].normal = Vector3D(1, 0, 0);
        res[0].referencia = ox;
        res[1] = calcular_yz();
    } else if(dy_zero) {
        res[0].normal = Vector3D(0, 1, 0);
        res[0].referencia = oy;
        res[1] = calcular_xz();
    } else if(dz_zero) {
        res[0].normal = Vector3D(0, 0, 1);
        res[0].referencia = oz;
        res[1] = calcular_xy();
    } else {
        res[0] = calcular_xy();
        res[1] = calcular_xz();
    }
}

Punto2D ConvertirA_PlanoXY(const Punto3D &p, const Triangulo3D &plano) {
    Punto3D origen = plano.vertices[0];
    Vector3D eje_x = plano.vertices[1] - plano.vertices[0];
    Vector3D eje_y = plano.vertices[2] - plano.vertices[0];
    Vector3D eje_z = ProductoVectorial(eje_x, eje_y);
    Matriz3x3 matriz_cambio_base;
    Punto3D res_3D;
    Punto2D res_2D;
    eje_y = ProductoVectorial(eje_z, eje_x);
    eje_x = Normalizar(eje_x);
    eje_y = Normalizar(eje_y);
    eje_z = Normalizar(eje_z);
    matriz_cambio_base = Matriz3x3(eje_x, eje_y, eje_z);
    res_3D = MultiplicarMatrizPorVector(matriz_cambio_base, p - origen);
    res_2D.x = res_3D.x;
    res_2D.y = res_3D.y;
    return res_2D;
}

Triangulo2D ConvertirA_PlanoXY(const Triangulo3D &t) {
    Triangulo2D res;
    unsigned int i;
    for(i = 0; i < 3; ++i)
        res.vertices[i] = ConvertirA_PlanoXY(t.vertices[i], t);
    return res;
}









Matriz3x3 TraspuestaMatriz(const Matriz3x3 &m) {
    Matriz3x3 res;
    unsigned int i, j;
    for(i = 0; i < 3; ++i)
        for(j = 0; j < 3; ++j)
            res.elem[i][j] = m.elem[j][i];
    return res;
}

double DeterminanteSubmatriz(const Matriz3x3 &m, unsigned int fil_eliminada, unsigned int col_eliminada) {
    unsigned int min_fil = 0;
    unsigned int min_col = 0;
    unsigned int max_fil = 2;
    unsigned int max_col = 2;
    if(fil_eliminada == 0)
        min_fil = 1;
    else if(fil_eliminada == 2)
        max_fil = 1;
    
    if(col_eliminada == 0)
        min_col = 1;
    else if(col_eliminada == 2)
        max_col = 1;
    
    return m.elem[min_fil][min_col]*m.elem[max_fil][max_col] -
        m.elem[min_fil][max_col]*m.elem[max_fil][min_col];
}



double DeterminanteMatriz(const Matriz3x3 &m) {
    return m.elem[0][0] * DeterminanteSubmatriz(m, 0, 0) - 
        m.elem[0][1] * DeterminanteSubmatriz(m, 0, 1) +
        m.elem[0][2] * DeterminanteSubmatriz(m, 0, 2);
}

Matriz3x3 AdjuntaMatriz(const Matriz3x3 &m) {
    Matriz3x3 res;
    unsigned int i, j;
    int signo;
    for(i = 0; i < 3; ++i) {
        for(j = 0; j < 3; ++j) {
            if((i+j)%2 == 0)
                signo = 1;
            else
                signo = -1;
            res.elem[i][j] = signo * DeterminanteSubmatriz(m, i, j);
        }
    }
    return res;
}

void MultiplicarMatrizPorConstante(Matriz3x3 &m, double c) {
    unsigned int i, j;
    for(i = 0; i < 3; ++i)
        for(j = 0; j < 3; ++j)
            m.elem[i][j] *= c;
}

Matriz3x3 InversaMatriz(const Matriz3x3 &m) {
    Matriz3x3 traspuesta = TraspuestaMatriz(m);
    double determinante = DeterminanteMatriz(m);
    Matriz3x3 adjunta = AdjuntaMatriz(traspuesta);
    MultiplicarMatrizPorConstante(adjunta, 1.0 / determinante);
    return adjunta;
}












double CalcularAngulo(const Vector3D &v1, const Vector3D &v2) {
    double producto_escalar = ProductoEscalar(v1, v2);
    double modulo_1 = Modulo(v1);
    double modulo_2 = Modulo(v2);
    double valor = producto_escalar / (modulo_1 * modulo_2);
    if(valor < -1.0)
        valor = -1.0;
    else if(valor > 1.0)
        valor = 1.0;
    double angulo = std::fmod(std::acos(valor), 2.0*M_PI);
    if(angulo < 0.0)
        angulo += 2.0*M_PI;
    return angulo;
}

double CalcularAngulo(const Vector2D &v1, const Vector2D &v2) {
    double angulo_v1 = std::atan2(v1.y, v1.x);
    double angulo_v2 = std::atan2(v2.y, v2.x);
    double res = std::fmod(angulo_v2 - angulo_v1, 2.0*M_PI);
    if(res < 0)
        res += 2.0*M_PI;
    return res;
}

bool PuntoDentroSemirecta(const Punto3D &p, const Semirecta3D &s) {
    Vector3D dir_punto = p - s.origen;
    Vector3D dir_recta = s.directriz;
    double angulo = CalcularAngulo(dir_punto, dir_recta);
    return std::fabs(angulo) <= EPSILON;
}

Punto3D PuntoInvalido() {
    Punto3D res;
    res.x = NAN;
    res.y = NAN;
    res.z = NAN;
    return res;
}

Punto3D CalcularInterseccion(const Semirecta3D &semirecta, const Triangulo3D &triangulo) {
    Plano3D planos_recta[2];
    Plano3D plano_tri;
    Punto3D punto_interseccion;
    Triangulo2D triangulo_2D;
    Punto2D punto_interseccion_2D;
    bool existe_interseccion;
    double angulo;
    
    plano_tri = GenerarPlanoTriangulo(triangulo);                                         // OK
    GenerarPlanosSemirecta(semirecta, planos_recta);                                      // OK
    punto_interseccion = ResolverEcuacion(planos_recta[0], planos_recta[1], plano_tri);   // OK
    triangulo_2D = ConvertirA_PlanoXY(triangulo);                                         // OK
    punto_interseccion_2D = ConvertirA_PlanoXY(punto_interseccion, triangulo);            // OK
    existe_interseccion = Triangulo2D_ContienePunto(triangulo_2D, punto_interseccion_2D); // OK
    angulo = CalcularAngulo(punto_interseccion - semirecta.origen, semirecta.directriz);
    
    if(!existe_interseccion || (angulo > M_PI/2 && angulo < 3*M_PI/2)) {
        punto_interseccion = PuntoInvalido();
    }
    return punto_interseccion;
}

Punto3D ResolverEcuacion(const Plano3D &p1, const Plano3D &p2, const Plano3D &p3) {
    Matriz3x3 matriz = Matriz3x3(p1.normal, p2.normal, p3.normal);
    Matriz3x3 inversa = InversaMatriz(matriz);
    Vector3D constantes = Vector3D(p1.referencia, p2.referencia, p3.referencia);
    Vector3D solucion = MultiplicarMatrizPorVector(inversa, constantes);
    Punto3D solucion_p;
    solucion_p.x = solucion.x;
    solucion_p.y = solucion.y;
    solucion_p.z = solucion.z;
    return solucion_p;
}

double Grados(double radianes) {
    return 180.0 * radianes / M_PI;
}

bool Triangulo2D_ContienePunto(const Triangulo2D &t, const Punto2D &p) {
    Vector2D arista_base;
    double angulo_restriccion;
    double angulo_p;
    bool res = false;
    arista_base = t.vertices[1] - t.vertices[0];
    angulo_restriccion = CalcularAngulo(arista_base,
                                        t.vertices[2] - t.vertices[0]);
    angulo_p = CalcularAngulo(arista_base, p - t.vertices[0]);
    if(angulo_p >= 0 && angulo_p <= angulo_restriccion) {
        arista_base = t.vertices[0] - t.vertices[1];
        angulo_restriccion = CalcularAngulo(arista_base,
                                            t.vertices[2] - t.vertices[1]);
        angulo_p = CalcularAngulo(arista_base, p - t.vertices[1]);
        res = angulo_p >= 0 && angulo_p >= angulo_restriccion;
    }
    return res;
}

double AreaTriangulo(const Triangulo3D &t) {
    Triangulo2D t_2D = ConvertirA_PlanoXY(t);
    return t_2D.vertices[1].x * t_2D.vertices[2].y / 2.0;
}















std::ostream& operator<<(std::ostream &os, const Punto2D &p) {
    os << '{' << p.x << ',' << p.y << '}';
    return os;
}

std::ostream& operator<<(std::ostream &os, const Punto3D &p) {
    os << '{' << p.x << ',' << p.y << ',' << p.z << '}';
    return os;
}

std::ostream& operator<<(std::ostream &os, const Vector2D &v) {
    os << '{' << v.x << ',' << v.y << '}';
    return os;
}

std::ostream& operator<<(std::ostream &os, const Vector3D &v) {
    os << '{' << v.x << ',' << v.y << ',' << v.z << '}';
    return os;
}

std::ostream& operator<<(std::ostream &os, const Semirecta3D &s) {
    os << "Dir: " << s.directriz << ", Ori: " << s.origen;;
    return os;
}

std::ostream& operator<<(std::ostream &os, const Triangulo2D &t) {
    os << t.vertices[0] << ", " << t.vertices[1] << ", " << t.vertices[2];
    return os;
}

std::ostream& operator<<(std::ostream &os, const Triangulo3D &t) {
    os << t.vertices[0] << ", " << t.vertices[1] << ", " << t.vertices[2];
    return os;
}

std::ostream& operator<<(std::ostream &os, const Plano3D &p) {
    os << p.normal << ", " << p.referencia;
    return os;
}

std::ostream& operator<<(std::ostream &os, const Matriz3x3 &m) {
    unsigned int i, j;
    for(i = 0; i < 3; ++i) {
        os << "\n";
        for(j = 0; j < 3; ++j)
            os << "   " << m.elem[i][j];
    }
    return os;
}
