#ifndef MATHHELP_H
#define MATHHELP_H

double degreesToRadians(double angle);

double radiansToDegrees(double angle);

double sinDegree(double angle);
double cosDegree(double angle);
double tanDegree(double angle);

double asinDegree(double value);
double acosDegree(double value);
double atanDegree(double value);

double opuestoFromHipotenusa(double angle, double hipotenusa);
double adyacenteFromHipotenusa(double angle, double hipotenusa);
double opuestoFromAdyacente(double angle, double adyacente);
double adyacenteFromOpuesto(double angle, double opuesto);
double hipotenusaFromOpuesto(double angle, double opuesto);
double hipotenusaFromAdyacente(double angle, double adyacente);

double getTriangleMissingAngle(double knownAngle);

#endif
