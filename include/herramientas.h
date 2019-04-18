#ifndef __HERRAMIENTAS_H__
#define __HERRAMIENTAS_H__

#include <fstream>
#include <time.h>

#define EPSILON (1e-10)

bool FullCompareNotEqual(double v1, double v2);
double Interpolar(double x, double min, double max, double min_inter, double max_inter);
double tiempo_actual();
void IgnorarHasta(std::ifstream &is, char delim);
void IgnorarLinea(std::ifstream &is);

#endif
