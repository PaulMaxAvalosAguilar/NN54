#include "mathHelp.h"
#include <math.h>

double degreesToRadians(double angle){
  return (angle *PI)/180;
}

double radiansToDegrees(double angle){
  return (angle *180)/PI;
}

double sinDegree(double angle){
  return sin(degreesToRadians(angle));
}

double cosDegree(double angle){
  return cos(degreesToRadians(angle));
}

double tanDegree(double angle){
  return tan(degreesToRadians(angle));
}

double asinDegree(double value){
  return radiansToDegrees(asin(value));
}

double acosDegree(double value){
  return radiansToDegrees(acos(value));
}

double atanDegree(double value){
  return radiansToDegrees(atan(value));
}

double opuestoFromHipotenusa(double angle, double hipotenusa){
  return sinDegree(angle) * hipotenusa;
}

double adyacenteFromHipotenusa(double angle, double hipotenusa){
  return cosDegree(angle) * hipotenusa;
}

double opuestoFromAdyacente(double angle, double adyacente){
  return tanDegree(angle) * adyacente;
}

double adyacenteFromOpuesto(double angle, double opuesto){
  return opuesto/tanDegree(angle);
}

double hipotenusaFromOpuesto(double angle, double opuesto){
  return opuesto/sinDegree(angle);
}

double hipotenusaFromAdyacente(double angle, double adyacente){
  return adyacente/cosDegree(angle);
}

double getTriangleMissingAngle(double knownAngle){
  return 180 - 90 - knownAngle;
}
