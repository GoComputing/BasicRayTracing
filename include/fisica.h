#ifndef __FISICA_H__
#define __FISICA_H__

#include <geometria.h>

Vector3D Refractar(const Vector3D &direccion, const Vector3D &normal, double indice_refraccion_anterior, double indice_refraccion_actual);
Vector3D Reflexionar(const Vector3D &direccion, const Vector3D &normal);

#endif
