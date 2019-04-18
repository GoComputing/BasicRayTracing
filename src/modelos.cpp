#include <herramientas.h>
#include <modelos.h>
#include <fstream>
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
    this->ultimo_punto_interseccion = p1;
}

ModeloTriangulo::ModeloTriangulo(const Triangulo3D &t, const Punto2D coords_tex[3])
    : ModeloTriangulo(t.vertices[0], t.vertices[1], t.vertices[2], coords_tex)
{
    
}
    
std::vector<Punto3D> ModeloTriangulo::CalcularIntersecciones(const Semirecta3D &semirecta) const {
    std::vector<Punto3D> res;
    Punto3D interseccion;
    interseccion = CalcularInterseccion(semirecta, triangulo);
    if(interseccion != PuntoInvalido())
        res.push_back(interseccion);
    ultimo_punto_interseccion = interseccion;
    return res;
}
    
Vector3D ModeloTriangulo::Normal(const Punto3D &/*punto_interseccion*/) const {
    return Normalizar(ProductoVectorial(triangulo.vertices[1] - triangulo.vertices[0], triangulo.vertices[2] - triangulo.vertices[0]));
}

Vector3D ModeloTriangulo::Normal(size_t id_interseccion) const {
    assert(id_interseccion == 0 && ultimo_punto_interseccion != PuntoInvalido());
    return this->Normal(ultimo_punto_interseccion);
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

Triangulo3D& ModeloTriangulo::GetTriangulo() {
    return triangulo;
}

const Triangulo3D& ModeloTriangulo::GetTriangulo() const {
    return triangulo;
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
    
    ultimas_intersecciones = res;
    return res;
}

Vector3D ModeloEsfera::Normal(const Punto3D &punto_interseccion) const {
    Vector3D normal = Normalizar(punto_interseccion - centro);
    return normal;
}

Vector3D ModeloEsfera::Normal(size_t id_interseccion) const { 
    assert(id_interseccion < ultimas_intersecciones.size());
    return Normal(ultimas_intersecciones[id_interseccion]);
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







size_t ModeloCompuesto::IndiceTrianguloInterseccion(const Punto3D &punto_interseccion) const {
    bool encontrado = false;
    size_t pos_encontrado = triangulos.size();
    size_t i;
    for(i = 0; i < triangulos.size() && !encontrado; ++i) {
        if(Triangulo3D_ContienePunto(triangulos[i].GetTriangulo(), punto_interseccion)) {
            pos_encontrado = i;
            encontrado = true;
        }
    }
    return pos_encontrado;
}

ModeloCompuesto::ModeloCompuesto() {
    
}
    
std::vector<Punto3D> ModeloCompuesto::CalcularIntersecciones(const Semirecta3D &semirecta) const {
    std::vector<Punto3D> resultado;
    size_t i, j;
    id_triangulos_ultimas_intersecciones = std::vector<size_t>();
    for(i = 0; i < triangulos.size(); ++i) {
        std::vector<Punto3D> intersecciones = triangulos[i].CalcularIntersecciones(semirecta);
        for(j = 0; j < intersecciones.size(); ++j) {
            id_triangulos_ultimas_intersecciones.push_back(i);
            resultado.push_back(intersecciones[j]);
        }
    }
    return resultado;
}

Vector3D ModeloCompuesto::Normal(const Punto3D &punto_interseccion) const {
    size_t indice = IndiceTrianguloInterseccion(punto_interseccion);
    if(indice >= triangulos.size())
        throw std::runtime_error("Un modelo no contiene un punto de intersección (ModeloCompuesto)");
    return NormalTriangulo(triangulos[indice].GetTriangulo());
}

Vector3D ModeloCompuesto::Normal(size_t id_interseccion) const {
    assert(id_interseccion < id_triangulos_ultimas_intersecciones.size());
    size_t id_triangulo = id_triangulos_ultimas_intersecciones[id_interseccion];
    return NormalTriangulo(triangulos[id_triangulo].GetTriangulo());
}

Vector2D ModeloCompuesto::CalcularCoordenadaTextura(const Punto3D &punto_interseccion) const {
    Punto3D proyeccion = Punto3D(punto_interseccion.x, punto_interseccion.y, 0.0);
    double pitch = CalcularAngulo(Vector3D(punto_interseccion), Vector3D(proyeccion));
    Vector3D eje_x = Vector3D(1.0, 0.0, 0.0);
    double yaw = CalcularAngulo(Vector3D(proyeccion), eje_x);
    double u = yaw / (2.0 * M_PI);
    double v = pitch / (2.0 * M_PI);
    return Vector2D(u, v);
}
    
void ModeloCompuesto::AniadirTriangulo(const Triangulo3D &triangulo) {
    Punto2D coords_tex[3] = {Punto2D(0.0, 0.0)};
    triangulos.push_back(ModeloTriangulo(triangulo, coords_tex));
}

void ModeloCompuesto::AniadirTriangulo(const Punto3D &p1, const Punto3D &p2, const Punto3D &p3) {
    Punto2D coords_tex[3] = {Punto2D(0.0, 0.0)};
    triangulos.push_back(ModeloTriangulo(p1, p2, p3, coords_tex));
}












static void AsignarComponente(double valor, Punto3D &p, char componente) {
    if(componente == 'x')
        p.x = valor;
    else if(componente == 'y')
        p.y = valor;
    else if(componente == 'z')
        p.z = valor;
    else
        throw std::runtime_error("Componente no reconocido");
}

ModeloCompuesto CargarPLY(const std::string &ruta) {
    std::ifstream archivo = std::ifstream(ruta);
    std::vector<Punto3D> vertices;
    std::string comando;
    bool leyendo_cabecera = true;
    size_t num_vertices = 0;
    size_t num_caras = 0;
    char orden[3];
    size_t tam_orden = 0;
    std::string tipo_num_elementos;
    std::string tipo_componente_elem;
    Punto3D punto;
    size_t i;
    double v1, v2, v3;
    size_t num_componentes;
    size_t id_1, id_2, id_3;
    ModeloCompuesto resultado;
    if(!archivo.is_open())
        throw std::runtime_error("Error leyendo modelo en '"+ruta+"'");
    
    // LECTURA DE CABECERA
    archivo >> comando;
    if(comando != "ply")
        throw std::runtime_error("Firma de modelo inválida en '"+ruta+"'");
    while(leyendo_cabecera) {
        archivo >> comando;
        if(comando == "end_header") {
            leyendo_cabecera = false;
        } else if(comando == "format") {
            IgnorarLinea(archivo);
        } else if(comando == "comment") {
            IgnorarLinea(archivo);
        } else if(comando == "element") {
            archivo >> comando;
            if(comando == "vertex")
                archivo >> num_vertices;
            else if(comando == "face")
                archivo >> num_caras;
            else
                throw std::runtime_error("Parámetro de 'element' inválido en '"+ruta+"'");
        } else if(comando == "property") {
            archivo >> comando;
            if(comando == "float32" || comando == "float") {
                archivo >> comando;
                orden[tam_orden++] = comando[0];
            } else if(comando == "list") {
                bool invalido;
                archivo >> tipo_num_elementos >> tipo_componente_elem >> comando;
                invalido = (tipo_num_elementos != "uint8" && tipo_num_elementos != "uchar");
                invalido = invalido || (tipo_componente_elem != "uint" && tipo_componente_elem != "uint32" && tipo_componente_elem != "int32");
                invalido = invalido || (comando != "vertex_indices");
                if(invalido) {
                    std::cerr << tipo_num_elementos << ' ' << tipo_componente_elem << ' ' << comando << std::endl;
                    throw std::runtime_error("Error en tipo de elemento en '"+ruta+"'");
                }
            } else {
                throw std::runtime_error("Parámetro de 'property' inválido en '"+ruta+"'");
            }
        } else {
            throw std::runtime_error("Comando '"+comando+"'no reconocido en '"+ruta+"'");
        }
    }
    if(tam_orden != 3)
        throw std::runtime_error("Orden no establecido en '"+ruta+"'");
    if(num_vertices == 0 || num_caras == 0)
        throw std::runtime_error("Número inválido de vértices/caras en '"+ruta+"'");
    
    
    //std::cout << num_vertices << ' ' << num_caras << std::endl;
    // LECTURA DE VÉRTICES
    vertices.resize(num_vertices);
    for(i = 0; i < num_vertices; ++i) {
        archivo >> v1 >> v2 >> v3;
        AsignarComponente(v1, punto, orden[0]);
        AsignarComponente(v2, punto, orden[1]);
        AsignarComponente(v3, punto, orden[2]);
        vertices[i] = punto;
    }
    
    
    // LECTURA DE CARAS
    for(i = 0; i < num_caras; ++i) {
        archivo >> num_componentes;
        if(num_componentes != 3)
            throw std::runtime_error("Número inválido de componentes por elemento en '"+ruta+"'");
        archivo >> id_1 >> id_2 >> id_3;
        if(id_1 >= vertices.size() || id_2 >= vertices.size() || id_3 >= vertices.size())
            throw std::runtime_error("Cara inválida (fuera de rango) en '"+ruta+"'");
        resultado.AniadirTriangulo(vertices[id_1], vertices[id_2], vertices[id_3]);
        //Triangulo3D t = Triangulo3D(vertices[id_2], vertices[id_1], vertices[id_3]);
        //std::cout << NormalTriangulo(t) << std::endl;
    }
    return resultado;
}
