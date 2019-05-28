#include "Archivos.h"
#include "Calculations.h"
#include "mathHelp.h"
#include <math.h>
#include <stdio.h>

void Disk(int print){
    //Dependencias
    double *anchosensor = &sensorGlobal.AnchodelTransistor;
    double *altosensor = &sensorGlobal.AltodelTransistor;
    double *diotrans = &sensorGlobal.GrosorEspaciodiodoTransistor;
    double *altoMaximoDisco = &sensorGlobal.EspacioAlturaparaDisco;
    double *gruesoEmpaque = &sensorGlobal.GrosorempaqueSensor;
    double *medidaseguridad = &discoGlobal.AnchoSeguridad;
    double *altoseguridad = &discoGlobal.AltoSeguridad;
    double *agujeros_desea = &discoGlobal.agujerosDeseados;
    double *gruesoDisco = &discoGlobal.gruesoDisco;
    double *radioBalero = &baleroGlobal.radioexterno;

    //Variables de uso propio
    double ancho_desfases;
    double ancho_agujeros;
    double espacio_agujeros;
    double angulo_desfases;

    double arco_interno;
    double perim_interno;
    double radio_interno;

    double arco_externo;
    double perim_externo;
    double radio_externo;

    double radiocilindrosujecion;
    double gruesocilindrosujecion;

    //MEDIDAS GENERALES
    ancho_desfases = *anchosensor + *medidaseguridad;
    ancho_agujeros = ancho_desfases *2;
    espacio_agujeros = ancho_agujeros;
    //MEDIDAS INTERNAS
    arco_interno = ancho_agujeros;
    perim_interno = *agujeros_desea * 2 * ancho_agujeros;
    radio_interno = (perim_interno)/(2*PI);
    angulo_desfases = (180*arco_interno)/(PI*radio_interno);
    //MEDIDAS EXTERNAS
    radio_externo = radio_interno + *altosensor + *altoseguridad;
    arco_externo = 2*PI*radio_externo*(angulo_desfases/360);
    perim_externo = 2*PI*radio_externo;
    //Radio cilindro sujeción
    radiocilindrosujecion = radio_interno/3;
    gruesocilindrosujecion = 4* *gruesoDisco;


    if(print == 1){

        printf("\nMEDIDAS GENERALES DISCO DENTADO\n");
        //impresion de medidas basicas
        printf("Ancho de los desfases %.4fmm\n", ancho_desfases);
        printf("Ancho de los agujeros %.4fmm\n", ancho_agujeros);
        printf("Espacio entre agujeros %.4fmm\n", espacio_agujeros);
        printf("Angulo entre desfases %.4f grados\n", angulo_desfases);

        printf("\nMEDIDAS INTERNAS\n");
        //impresion de medidas internas
        printf("Arco interno de los agujeros %.4fmm\n", arco_interno);
        printf("Perimetro interno de los agujeros %.6fmm\n",
               perim_interno);
        printf("Radio interno del circulo %.4fmm\n", radio_interno);

        printf("\nMEDIDAS EXTERNAS\n");
        //impresion de medidas externas
        printf("Arco externo de los agujeros %.4fmm\n", arco_externo);
        printf("Perimetro externo de los agujeros %.4fmm\n",perim_externo);
        printf("Radio externo del circulo %.4fmm\n", radio_externo);

	printf("\nGROSOR DISCO\n");
        //impresion de medidas externas
        printf("Grosor disco %.4fmm\n", *gruesoDisco);


        printf("\nMEDIDAS CILINDRO PARA SUJECIÓN\n");
        printf("El medio cilindro de 180 grados debe tener un radio de %.4f\n",
               radiocilindrosujecion);
        printf("El medio cilíndro de 180 grados debe tener un grueso de %.4f\n",
               gruesocilindrosujecion);

        //MANEJO DE ERRORES
        printf("\n--CHECKING FOR POSSIBLE ERRORS--\n");
        printf("\nGROSOR DEL DISCO\n");
        if(*gruesoDisco > *diotrans){
            printf("ERROR: El grueso del disco de %.4fmm es mayor que el espacio\n"
                   "entre diodo y transistor de  %.4fmm\n", *gruesoDisco, *diotrans);        }

        printf("\nDIENTES DEL DISCO\n");
        if((radio_externo - radio_interno) > *altoMaximoDisco){
            printf("ERROR: La medida de alto de los dientes de %.4f del disco es mayor que el espacio\n"
                   "disponible en el sensor de %.4f\n", radio_externo-radio_interno,
                   sensorGlobal.EspacioAlturaparaDisco);
        }
	
        printf("\nTAMAÑO DEL DISCO\n");
        if(*radioBalero > radio_interno){
            printf("ERROR: El radio del disco interno de %.4f es demasiado pequeño para el balero de %.4f\n"
                   "Aumenta la medida de seguridad del archivo disco o bien\n"
                   "Aumenta el numero de dientes deseados o\n"
                   "Consigue un balero más pequeño\n", radio_interno, *radioBalero);
        }

        printf("\nGRUESO CILINDRO PARA SUJECION\n");
        if(gruesocilindrosujecion >= (*gruesoEmpaque/2)){
            printf("ERROR: El grueso del medio cilíndro de 180 grados de %.4f es mayor o igual\n"
                   "que la mitad del grueso del empaque de %.4f\n"
                   "diminuye el tamaño", gruesocilindrosujecion, (*gruesoEmpaque/2));
        }
        printf("\n");
    }
    //comunicación de cálculos
    discoGlobal.radiointerno = radio_interno;
    discoGlobal.angulo_desfases = angulo_desfases;
}

void Soportes(int print){
    //Dependencias
    double *altoExtraBase = &soporteGlobal.altoExtradeBase;
    double *anguloTotal = &soporteGlobal.anguloParaAncho;
    double *extradioExterno = &soporteGlobal.extradioExterno;
    double *radioInternoDisco = &discoGlobal.radiointerno;
    double *altoSensor = &sensorGlobal.AltoempaqueSensor;
    double *grosorBalero = &baleroGlobal.grosorBalero;
    double *radioextBalero = &baleroGlobal.radioexterno;


    //variables de uso propio
    double grosor_cilindro;
    double radiointerno;
    double radioexterno;
    double grosorsoporte;
    double catetopuesto;
    double anchosoporte;
    double altosoporte;

    grosor_cilindro = *grosorBalero;
    radiointerno = *radioextBalero;
    radioexterno = radiointerno + *extradioExterno;
    grosorsoporte = grosor_cilindro;
    catetopuesto = opuestoFromHipotenusa(*anguloTotal/2.0,radioexterno);
    anchosoporte = catetopuesto*2;
    altosoporte = *altoSensor + *radioInternoDisco
            + *altoExtraBase - radioexterno;

    if(print == 1){
        printf("\nMEDIDAS SOPORTE\n");
        printf("Grosor cilindro %.4fmm\n", grosor_cilindro);
        printf("Internal circle radius %.4fmm\n",radiointerno);
        printf("External circle radius %.4fmm\n",radioexterno);
        printf("Grosor soporte %.4fmm\n", grosorsoporte);
        printf("Ancho del soporte es de %.4fmm\n", anchosoporte);
        printf("Internal cilinder to origin legth %.4fmm\n", altosoporte);
        printf("\n");


        //MANEJO DE ERRORES
        printf("\n--CHECKING FOR POSSIBLE ERRORS--\n");
        printf("\nALTURA DEL SOPORTE\n");
        if((altosoporte <= 1)){
            printf("ERROR: EL contenedor del resorte no puede tener un soporte de %.4f\n"
                   "el contenedor es demasiado grande\n",altosoporte);
        }
        printf("\n");

    }
    //comunicación de calculos
    soporteGlobal.grosorSoporte = grosorsoporte;
    soporteGlobal.anchoSoporte = anchosoporte;
}

void SoporteSensores(int print){
  //Dependencias
  int *n1 = &posSensoresGlobal.n1;
  int *n2 = &posSensoresGlobal.n2;
  double *x = &posSensoresGlobal.x;
  double *radioInternoDisco = &discoGlobal.radiointerno;
  double *angulo = &discoGlobal.angulo_desfases;
  double *anchoSensor = &sensorGlobal.AnchoempaqueSensor;
  double *altoempaqueSensor = &sensorGlobal.AltoempaqueSensor;
  double *anchoSopBaleros = &soporteGlobal.anchoSoporte;
  double *grosorSoportes = &sensorGlobal.GrosorEspacioExtremoATorre;
  
  //   angle alpha
  // a *  
  // d * *  Hi
  // y *  *  po
  // a *   *  te
  // c *    *  nu
  // e *     *  sa
  // n *      * 
  // t *       *
  // e ********** angle beta
  //   opuesto


  //   *  
  //   * *      *       Li  *       
  //   *  *    **       ne **       
  //   * A *  *B*       3 * *
  //   **********        *  *
  //        **  *       *   *
  //       *B*  *      *    *
  //      ****  *   Li*     *Li
  //      *     *   ne*     *ne
  //      *******   1 *******2
  //                   Line4
  
  //variables de uso propio
  //Soporte sensor 1
  double s1angleAlphaTriangleA;
  double s1angleBetaTriangleA;
  double s1hipotenusaTriangleA;
  double s1opuestoTriangleA;
  double s1adyacenteTriangleA;

  double s1angleAlphaTriangleB;
  double s1angleBetaTriangleB;
  double s1hipotenusaTriangleB;
  double s1opuestoTriangleB;
  double s1adyacenteTriangleB;

  double s1line1Length;
  double s1line2Length;
  double s1line3Length;
  double s1line4Length;
  double s1line1Line3Angle;
  double s1line1ToOriginLength;
  
  //Soporte sensor 2
  double s2angleAlphaTriangleA;
  double s2angleBetaTriangleA;
  double s2hipotenusaTriangleA;
  double s2opuestoTriangleA;
  double s2adyacenteTriangleA;

  double s2angleAlphaTriangleB;
  double s2angleBetaTriangleB;
  double s2hipotenusaTriangleB;
  double s2opuestoTriangleB;
  double s2adyacenteTriangleB;

  double s2line1Length;
  double s2line2Length;
  double s2line3Length;
  double s2line4Length;
  double s2line1Line3Angle;
  double s2line1ToOriginLength;
  


  //Soporte sensor 1  
  s1angleAlphaTriangleA = (*n1 * *angulo + *x);
  s1angleBetaTriangleA = getTriangleMissingAngle(s1angleAlphaTriangleA);
  s1hipotenusaTriangleA = *radioInternoDisco + *altoempaqueSensor;
  s1opuestoTriangleA = opuestoFromHipotenusa(s1angleAlphaTriangleA,
					   s1hipotenusaTriangleA);
  s1adyacenteTriangleA = adyacenteFromHipotenusa(s1angleAlphaTriangleA,
					       s1hipotenusaTriangleA);

  s1angleAlphaTriangleB = 90 - (90 - s1angleBetaTriangleA);
  s1angleBetaTriangleB = getTriangleMissingAngle(s1angleAlphaTriangleB);
  s1hipotenusaTriangleB = *anchoSensor/2 ;
  s1opuestoTriangleB = opuestoFromHipotenusa(s1angleAlphaTriangleB,
					     s1hipotenusaTriangleB);
  s1adyacenteTriangleB = adyacenteFromHipotenusa(s1angleAlphaTriangleB
					     ,s1hipotenusaTriangleB);
  
  s1line1Length = *radioInternoDisco + *altoempaqueSensor - s1adyacenteTriangleA -s1adyacenteTriangleB;
  s1line2Length = s1line1Length + (s1adyacenteTriangleB * 2);
  s1line3Length = *anchoSensor;
  s1line4Length = s1opuestoTriangleB * 2;
  s1line1Line3Angle = s1angleBetaTriangleB + 90;
  s1line1ToOriginLength = s1opuestoTriangleA - s1opuestoTriangleB;
 

  //Soporte sensor 2
  s2angleAlphaTriangleA = (*n2 * *angulo - (.5 * *angulo) + *x);
  s2angleBetaTriangleA = getTriangleMissingAngle(s2angleAlphaTriangleA);
  s2hipotenusaTriangleA = *radioInternoDisco + *altoempaqueSensor;
  s2opuestoTriangleA = opuestoFromHipotenusa(s2angleAlphaTriangleA,
					   s2hipotenusaTriangleA);
  s2adyacenteTriangleA = adyacenteFromHipotenusa(s2angleAlphaTriangleA,
					       s2hipotenusaTriangleA);

  s2angleAlphaTriangleB = 90 - (90 - s2angleBetaTriangleA);
  s2angleBetaTriangleB = getTriangleMissingAngle(s2angleAlphaTriangleB);
  s2hipotenusaTriangleB = *anchoSensor/2 ;
  s2opuestoTriangleB = opuestoFromHipotenusa(s2angleAlphaTriangleB,
					     s2hipotenusaTriangleB);
  s2adyacenteTriangleB = adyacenteFromHipotenusa(s2angleAlphaTriangleB
					     ,s2hipotenusaTriangleB);
  
  s2line1Length = *radioInternoDisco + *altoempaqueSensor - s2adyacenteTriangleA -s2adyacenteTriangleB;
  s2line2Length = s2line1Length + (s2adyacenteTriangleB * 2);
  s2line3Length = *anchoSensor;
  s2line4Length = s2opuestoTriangleB * 2;
  s2line1Line3Angle = s2angleBetaTriangleB + 90;
  s2line1ToOriginLength = s2opuestoTriangleA - s2opuestoTriangleB;
  
  if(print == 1){
    
    printf(
     ".\n"  
     ". .      .       Li  .\n"
     ".  .    ..       ne ..\n"       
     ". A .  .B.       3 . .\n"
     "..........        .  .\n"
     "     ..  .       .   .\n"
     "    .B.  .      .    .\n"
     "   ....  .   Li.     .Li\n"
     "   .     .   ne.     .ne\n"
     "   .......   1 .......2\n"
     "                Line4\n\n"
);
    
    printf("\nSENSOR UNO\n");
    printf("PositionAngle %.4fmm\n"
	   "Hipotenusa Triangle A %.4fmm\n",
	   s1angleAlphaTriangleA,
	   s1hipotenusaTriangleA);
    printf("Line1 lenght %.4fmm\n"
           "Line2 lenght %.4fmm\n"
           "Line3 lenght %.4fmm\n"
	   "Line4 lenght %.4fmm\n",
	   s1line1Length,
	   s1line2Length,
	   s1line3Length,
	   s1line4Length);
    printf("Line1-line3 angle %.4fmm\n"
	   "Line1 to origin length %.4fmm\n",
	   s1line1Line3Angle,
	   s1line1ToOriginLength);
    
    printf("\nSENSOR DOS\n");
    printf("PositionAngle %.4fmm\n"
	   "Hipotenusa Triangle A %.4fmm\n",
	   s2angleAlphaTriangleA,
	   s2hipotenusaTriangleA);
    printf("Line1 lenght %.4fmm\n"
           "Line2 lenght %.4fmm\n"
           "Line3 lenght %.4fmm\n"
	   "Line4 lenght %.4fmm\n",
	   s2line1Length,
	   s2line2Length,
	   s2line3Length,
	   s2line4Length);
    printf("Line1-line3 angle %.4fmm\n"
	   "Line1 to origin length %.4fmm\n",
	   s2line1Line3Angle,
	   s2line1ToOriginLength);
    printf("\nGROSOR SOPORTES\n");
    printf("Grosor soportes %.4fmm\n", *grosorSoportes);

    //MANEJO DE ERRORES    
    printf("\n--CHECKING FOR POSSIBLE ERRORS--\n");
    
    printf("\nPOSICION SENSORES\n");
    if((s2angleAlphaTriangleA) <= -0
       || (s1angleAlphaTriangleA) >=90){
      printf("Angulo no posisionable\n"); 
    }

    printf("\nCOLISIÓN CON SOPORTE BALEROS\n");
    if(s2line1ToOriginLength < (*anchoSopBaleros / 2.0)){
      printf("Ángulo muy corto\n");
    }
    
  }
  printf("\n");
  
}

void Flecha(int print){
    //Dependencias
    double *rinterno = &baleroGlobal.radiointerno;

    //Variables para uso propio

    if(print == 1){

        printf("\nMEDIDAS FLECHA\n");
        printf("Rardio arcos: %.4fmm\n", *rinterno);
	printf("Ángulo arcos: 45grados\n");

        printf("\n--CHECKING FOR POSSIBLE ERRORS--\n");

        printf("\n");
    }
}

void CajaResorte(int print)
{
    //Dependencias
    double *grosTapaExt = &cajaResorteGlobal.grosorTapas;
    double *extranchoFleje = &cajaResorteGlobal.extraGrosorTambor;
    double *radioInterno = &cajaResorteGlobal.radioCilInterno;
    double *extraRadioInt = &cajaResorteGlobal.extradioexterno;
    double *grosFleje = &flejeGlobal.grueso_fleje;

    //variables de uso propio
    double GrosorCuerpo;
    double GrosorSoporte;
    double GrosorTapaExterna;
    double grosorTotal;
    double radioexterno;

    double radioCirculoFleje;
    double radioAlma;

    //GROSORES
    GrosorCuerpo = *grosFleje + *extranchoFleje;
    GrosorSoporte = GrosorCuerpo;
    GrosorTapaExterna = *grosTapaExt;
    grosorTotal = GrosorTapaExterna *2  + GrosorCuerpo;
    
    radioexterno = *radioInterno + *extraRadioInt;

    radioCirculoFleje = (*grosFleje/2)/1.85;
    radioAlma = *radioInterno/2;

    if(print == 1){
        printf("\nMEDIDAS GROSOR CAJA RESORTE\n");
        printf("Grosor cuerpo %.4fmm\n", GrosorCuerpo);
        printf("Grosor soporte: %.4fmm\n", GrosorSoporte);
        printf("Grosor TapaExterna: %.4fmm\n",
               GrosorTapaExterna);
        printf("Grosor Total: %.4fmm\n", grosorTotal);

	printf("\nMEDIDAS RADIO CAJA RESORTE\n");
        printf("Radio interno: %.4fmm\n", *radioInterno);
        printf("Radio externo: %.4fmm\n", radioexterno);

	printf("\n MEDIDAS ABERTURAS CAJA RESORTE\n");
        printf("Radio para anclar fleje horizontalmente %.4fmm\n", radioCirculoFleje);
        printf("Radio circulo fleche  %.4fmm\n", radioAlma);
        printf("\n");

        //MANEJO DE ERRORES
        printf("\n--CHECKING FOR POSSIBLE ERRORS--\n");
        printf("\nRADIO EXTERNO CAJA RESORTE\n");

        printf("\n");
    }
}
