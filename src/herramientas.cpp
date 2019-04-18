#include <herramientas.h>
#include <limits>
#include <cmath>

bool FullCompareNotEqual(double v1, double v2) {
    bool n1 = std::isnan(v1);
    bool n2 = std::isnan(v2);
    bool res;
    if(n1 && n2)
        res = false;
    else
        res = v1 != v2;
    return res;
}

double Interpolar(double x, double min, double max, double min_inter, double max_inter) {
    double valor_normalizado = (x - min) / (max - min);
    return valor_normalizado * (max_inter - min_inter) + min_inter;
}

double tiempo_actual() {
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec + t.tv_nsec/1000000000.0;
}

void IgnorarHasta(std::ifstream &is, char delim) {
    is.ignore(std::numeric_limits<std::streamsize>::max(), delim);
}

void IgnorarLinea(std::ifstream &is) {
    IgnorarHasta(is, '\n');
}
