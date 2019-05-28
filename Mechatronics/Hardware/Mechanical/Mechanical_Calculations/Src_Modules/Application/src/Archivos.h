#ifndef ARCHIVOS_H
#define ARCHIVOS_H

typedef struct Datos_Sensor{
  double AnchodelTransistor;
  double AltodelTransistor;
  double GrosorempaqueSensor;
  double AnchoempaqueSensor;
  double AltoempaqueSensor;
  double GrosorEspaciodiodoTransistor;
  double GrosorEspacioExtremoATorre;
  double EspacioAlturaparaDisco;
} Datos_Sensor;

typedef struct Datos_Balero{
  double radiointerno;
  double radioexterno;
  double grosorBalero;
} Datos_Balero;

typedef struct Datos_Disco{
  double AnchoSeguridad;
  double AltoSeguridad;
  double agujerosDeseados;
  double gruesoDisco;
  //Datos en memoria calculados por el programa
  double radiointerno;
  double angulo_desfases;
}Datos_Disco;

typedef struct Datos_Soporte{
  double anguloParaAncho;
  double altoExtradeBase;
  double extradioExterno;
  //Datos en memoria calculados por el programa
  double grosorSoporte;
  double anchoSoporte;
}Datos_Soporte;

typedef struct Datos_PosSensores{
  //PosB = n1 * alpha(+/- 1/2*alpha) + x
  int n1;
  int n2;
  double x;
}Datos_PosSensores;

typedef struct Datos_Fleje{
    double grueso_fleje;
}Datos_Fleje;

typedef struct Datos_CajaResorte{
  double extraGrosorTambor;
  double grosorTapas;
  double radioCilInterno;
  double extradioexterno;
}Datos_CajaResorte;

typedef struct Datos_Flecha{
    double extraGruesoparaCuerda;
}Datos_Flecha;

Datos_Sensor sensorGlobal;
Datos_Balero baleroGlobal;
Datos_Disco discoGlobal;
Datos_Soporte soporteGlobal;
Datos_PosSensores posSensoresGlobal;
Datos_Fleje flejeGlobal;
Datos_CajaResorte cajaResorteGlobal;
Datos_Flecha flechaGlobal;

#endif
