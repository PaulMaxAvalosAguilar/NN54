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
  double *altoempaqueSensor = &sensorGlobal.AltoempaqueSensor;
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

  double eje;
      
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
  //EJE
  eje = radio_interno + *altoempaqueSensor;

  if(print == 1){

    printf("\nMEDIDAS EJE\n");
    printf("Eje pieza %.4fmm\n", eje);
      
    printf("\nMEDIDAS GENERALES DISCO DENTADO\n");
    //impresion de medidas basicas
    printf("Ancho desfases %.4fmm\n", ancho_desfases);
    printf("Ancho agujeros %.4fmm\n", ancho_agujeros);
    printf("Espacio entre agujeros %.4fmm\n", espacio_agujeros);
    printf("Angulo entre desfases %.4f grados\n", angulo_desfases);

    printf("\nMEDIDAS INTERNAS\n");
    //impresion de medidas internas
    printf("Arco interno %.4fmm\n", arco_interno);
    printf("Perimetro agujeros %.6fmm\n",
	   perim_interno);
    printf("Radio interno circulo %.4fmm\n", radio_interno);

    printf("\nMEDIDAS EXTERNAS\n");
    //impresion de medidas externas
    printf("Arco externo agujeros %.4fmm\n", arco_externo);
    printf("Perimetro externo agujeros %.4fmm\n",perim_externo);
    printf("Radio externo circulo %.4fmm\n", radio_externo);

    printf("\nGROSOR DISCO\n");
    //impresion de medidas externas
    printf("Grosor disco %.4fmm\n", *gruesoDisco);

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

    printf("\n");
  }
  //comunicación de cálculos
  discoGlobal.radiointerno = radio_interno;
  discoGlobal.angulo_desfases = angulo_desfases;
}

void SoportesBaleros(int print){
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
    printf("Internal cilinder perimeter-origin legth %.4fmm\n", altosoporte);
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

void SoportesSensores(int print){
  //Dependencias
  int *n1 = &posSensoresGlobal.n1;
  int *n2 = &posSensoresGlobal.n2;
  double *x = &posSensoresGlobal.x;
  double *radioInternoDisco = &discoGlobal.radiointerno;
  double ang = 15;
  //  double *angulo = &discoGlobal.angulo_desfases;
  double *angulo = &ang;
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

void SistemaTraccion(int print)
{
  //Dependencias
  double *extranchoFleje = &cajaResorteGlobal.extraGrosorTambor;
  double *radioInterno = &cajaResorteGlobal.radioCilInterno;
  double *extraRadioInt = &cajaResorteGlobal.extradioexterno;
  double *grosFleje = &flejeGlobal.grueso_fleje;

  double *rinterno = &baleroGlobal.radiointerno;

  double *altoempaqueSensor = &sensorGlobal.AltoempaqueSensor;
  double *radioInternoDisco = &discoGlobal.radiointerno;

  double *anguloFlecha = &flechaGlobal.angulo;
  double *offset = &flechaGlobal.offset;

  //variables de uso propio
  double GrosorCuerpo;
  double radioexterno;

  double anchoFlecha;
  double radioFlechaChica;
  double anguloFlechaChica;
  double radioFlechaDiscoGrande;
  double anguloFlechaDiscoGrande;
  
  double eje;

  //CAJA RESORTE
  GrosorCuerpo = *grosFleje + *extranchoFleje;
  radioexterno = *radioInterno + *extraRadioInt;

  //Flecha
  anchoFlecha = opuestoFromHipotenusa(*anguloFlecha/2,*rinterno) * 2;
  radioFlechaChica = *rinterno - (*offset/2);
  anguloFlechaChica = asinDegree(((anchoFlecha/2)-(*offset/2))/
				 radioFlechaChica)*2;
  radioFlechaDiscoGrande = *rinterno + (*offset/2);
  anguloFlechaDiscoGrande = asinDegree(((anchoFlecha/2)+(*offset/2))/
				 radioFlechaDiscoGrande)*2;;

  eje = *radioInternoDisco + *altoempaqueSensor;
  
  if(print == 1){
    printf("\nMEDIDAS EJE\n");
    printf("Eje pieza %.4fmm\n", eje);
    
    printf("\nMEDIDAS GROSOR CAJA RESORTE\n");
    printf("Grosor cuerpo %.4fmm\n", GrosorCuerpo);

    printf("\nMEDIDAS RADIO CAJA RESORTE\n");
    printf("Radio interno: %.4fmm\n", *radioInterno);
    printf("Radio externo: %.4fmm\n", radioexterno);

    printf("\nMEDIDAS FLECHA\n");
    printf("Rardio arcos: %.4fmm\n", *rinterno);
    printf("Ángulo arcos:%.4fmm\n", *anguloFlecha);
    printf("Ancho flecha: %.4fmm\n", anchoFlecha);

    printf("\nMEDIDAS EXPANSION FLECHA\n");
    printf ("Radio Flecha Chica: %.4fmm\n", radioFlechaChica);
    printf ("Angulo Flecha Chica: %.4fmm\n", anguloFlechaChica);
    printf ("Radio Flecha Disco Grande: %.4fmm\n",
	    radioFlechaDiscoGrande);
    printf ("Angulo Flecha Disco Grande: %.4fmm\n",
	    anguloFlechaDiscoGrande);
    
	
    //MANEJO DE ERRORES
    printf("\n--CHECKING FOR POSSIBLE ERRORS--\n");
    printf("\n");
  }
}
