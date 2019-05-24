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
    radiocilindrosujecion = radio_interno/2;
    gruesocilindrosujecion = 4* *gruesoDisco;


    if(print == 1){

        printf("\nMEDIDAS GENERALES DISCO DENTADO\n");
        //impresion de medidas basicas
        printf("El ancho de los desfases es de %.4fmm\n", ancho_desfases);
        printf("El ancho de los agujeros es de %.4fmm\n", ancho_agujeros);
        printf("El espacio entre agujeros es de %.4fmm\n", espacio_agujeros);
        printf("El angulo entre desfases es de %.4f grados\n", angulo_desfases);

        printf("\nMEDIDAS INTERNAS\n");
        //impresion de medidas internas
        printf("El arco interno de los agujeros es de %.4fmm\n", arco_interno);
        printf("El perimetro interno de los agujeros es de %.6fmm\n",
               perim_interno);
        printf("El radio interno del circulo es de %.4fmm\n", radio_interno);

        printf("\nMEDIDAS EXTERNAS\n");
        //impresion de medidas externas
        printf("El arco externo de los agujeros es de %.4fmm\n", arco_externo);
        printf("El perimetro externo de los agujeros es de %.4fmm\n",perim_externo);
        printf("El radio externo del circulo es de %.4fmm\n", radio_externo);

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
                   "entre diodo y transistor de  %.4fmm\n", *gruesoDisco, *diotrans);
        }else{
            printf("El grueso del disco de %.4fmm cabe dentro del espacio"
                   " entre\n el diodo y transistor de  %.4fmm\n",
                   *gruesoDisco, *diotrans);
        }

        printf("\nDIENTES DEL DISCO\n");
        if((radio_externo - radio_interno) > *altoMaximoDisco){
            printf("ERROR: La medida de alto de los dientes de %.4f del disco es mayor que el espacio\n"
                   "disponible en el sensor de %.4f\n", radio_externo-radio_interno,
                   sensorGlobal.EspacioAlturaparaDisco);
        }else{
            printf("La medida de alto de los dientes del disco de %.4f es adecuada dado\n"
                   "el espacio disponible en el sensor de %.4f\n", radio_externo-radio_interno,
                   sensorGlobal.EspacioAlturaparaDisco);
        }

        printf("\nTAMAÑO DEL DISCO\n");
        if(*radioBalero > radio_interno){
            printf("ERROR: El radio del disco interno de %.4f es demasiado pequeño para el balero de %.4f\n"
                   "Aumenta la medida de seguridad del archivo disco o bien\n"
                   "Aumenta el numero de dientes deseados o\n"
                   "Consigue un balero más pequeño\n", radio_interno, *radioBalero);
        }else{
            printf("El radio del disco de %.4f es adecuado para el tamaño de balero usado\n"
                   "de %.4f\n", radio_interno, *radioBalero);
        }

        printf("\nGRUESO CILINDRO PARA SUJECION\n");
        if(gruesocilindrosujecion >= (*gruesoEmpaque/2)){
            printf("ERROR: El grueso del medio cilíndro de 180 grados de %.4f es mayor o igual\n"
                   "que la mitad del grueso del empaque de %.4f\n"
                   "diminuye el tamaño", gruesocilindrosujecion, (*gruesoEmpaque/2));
        }else{
            printf("El grueso del medio cliíndro de 180 grados de %.4f es menor que\n"
                   "la mitad del grueso del empaque de %.4f\n",
                   gruesocilindrosujecion,(*gruesoEmpaque/2));
        }

        printf("\n");
    }
    //comunicación de cálculos
    discoGlobal.radiointerno = radio_interno;
    discoGlobal.angulo_desfases = angulo_desfases;
}

void Sensores(int print){
  //Dependencias
  int *n1 = &posSensoresGlobal.n1;
  int *n2 = &posSensoresGlobal.n2;
  double *x = &posSensoresGlobal.x;
  double *radioInternoDisco = &discoGlobal.radiointerno;
  double *angulo = &discoGlobal.angulo_desfases;
  double *anchoSensor = &sensorGlobal.AnchoempaqueSensor;
  double *altoempaqueSensor = &sensorGlobal.AltoempaqueSensor;

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
  //Sensor Soporte 1
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
  
  //Sensor Soporte 2
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
  


  //Sensor Soporte 1  
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
 

  //Sensor Soporte 2
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
    printf("PositionAngle %.4f\n"
	   "Hipotenusa Triangle A %.4f\n",
	   s1angleAlphaTriangleA,
	   s1hipotenusaTriangleA);
    printf("Line1 lenght %.4f\n"
           "Line2 lenght %.4f\n"
           "Line3 lenght %.4f\n"
	   "Line4 lenght %.4f\n",
	   s1line1Length,
	   s1line2Length,
	   s1line3Length,
	   s1line4Length);
    printf("Line1-line3 angle %.4f\n"
	   "Line1 to origin length %.4f\n",
	   s1line1Line3Angle,
	   s1line1ToOriginLength);
    
    printf("\nSENSOR DOS\n");
    printf("PositionAngle %.4f\n"
	   "Hipotenusa Triangle A %.4f\n",
	   s2angleAlphaTriangleA,
	   s2hipotenusaTriangleA);
    printf("Line1 lenght %.4f\n"
           "Line2 lenght %.4f\n"
           "Line3 lenght %.4f\n"
	   "Line4 lenght %.4f\n",
	   s2line1Length,
	   s2line2Length,
	   s2line3Length,
	   s2line4Length);
    printf("Line1-line3 angle %.4f\n"
	   "Line1 to origin length %.4f\n",
	   s2line1Line3Angle,
	   s2line1ToOriginLength);
    

    //MANEJO DE ERRORES
    /*
    printf("\n--CHECKING FOR POSSIBLE ERRORS--\n");
    printf("\nPOSICION SENSORES\n");
    if((anglePosition2negative) <= -0
       || (anglePosition2plus) >=90){
      printf("Angulo no posisionable\n"); 
    }
    */
  }
  printf("\n");
  
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

void CajaResorte(int print)
{
    //Dependencias
    double *grosTapaExt = &cajaResorteGlobal.grosorTapas;
    double *extranchoFleje = &cajaResorteGlobal.extraGrosorTambor;
    double *radioInterno = &cajaResorteGlobal.radioCilInterno;
    double *extraRadioInt = &cajaResorteGlobal.extradioexterno;
    double *anguloTotal = &cajaResorteGlobal.angulo;
    double *altoExtra = &cajaResorteGlobal.altoExtadeBase;
    double *extraRAlm = &cajaResorteGlobal.extradioAlma;

    double *grosFleje = &flejeGlobal.grueso_fleje;

    double *radioInternoDisco = &discoGlobal.radiointerno;
    double *altoSensor = &sensorGlobal.AltoempaqueSensor;

    double *radioBalero = &baleroGlobal.radiointerno;

    //variables de uso propio
    double angulo;
    double anguloRadianes;
    double catetoOpuesto;
    double catetoAdyacente;
    double GrosorCuerpo;
    double GrosorSoporte;
    double GrosorTapaExterna;
    double grosorTotal;
    double radioexterno;
    double anchoSoporte;
    double altoSoporte;
    double altoTotalSoporte;
    double circuloFleje;
    double radioAlma;

    angulo = *anguloTotal/2.0;
    anguloRadianes = (angulo * PI)/180;

    //GROSORES
    GrosorCuerpo = *grosFleje + *extranchoFleje;
    GrosorSoporte = GrosorCuerpo;
    GrosorTapaExterna = *grosTapaExt;
    grosorTotal = GrosorTapaExterna *2  + GrosorCuerpo;
    //Anchosoporte
    radioexterno = *radioInterno + *extraRadioInt;
    catetoOpuesto = radioexterno * sin(anguloRadianes);
    catetoAdyacente = radioexterno * cos(anguloRadianes);
    anchoSoporte = catetoOpuesto *2;
    altoSoporte = *altoSensor + *radioInternoDisco +
            *altoExtra - radioexterno;
    altoTotalSoporte = altoSoporte +
            (radioexterno - catetoAdyacente);

    circuloFleje = .25 * *grosFleje;
    radioAlma = *radioBalero + *extraRAlm;

    if(print == 1){
        printf("\nMEDIDAS Caja Resorte\n");
        printf("El grosor del cuerpo es de: %.4fmm\n", GrosorCuerpo);
        printf("El grosor del soporte es de: %.4fmm\n", GrosorSoporte);
        printf("El grosor de la TapaExterna es de: %.4fmm\n",
               GrosorTapaExterna);
        printf("El grosor Total es de: %.4fmm\n", grosorTotal);
        printf("El radio interno es de: %.4fmm\n", *radioInterno);
        printf("El radio externo es de: %.4fmm\n", radioexterno);
        printf("El ancho del Soporte por el ángulo es de: %.4fmm\n",
               anchoSoporte);
        printf("El alto del soporte es de %.4fmm\n", altoSoporte);
        printf("El alto total del soporte para resta de figuras debe de ser"
               "de %.4fmm\n", altoTotalSoporte);
        printf("El radio para anclar el fleje a 180 grados es de %.4f\n", circuloFleje);
        printf("El radio del circulo del alma es de %.4f\n", radioAlma);
        printf("\n");

        //MANEJO DE ERRORES
        printf("\n--CHECKING FOR POSSIBLE ERRORS--\n");
        printf("\nALTURA DEL SOPORTE\n");
        if((altoSoporte <= 1)){
            printf("ERROR: EL contenedor del resorte no puede tener un soporte de %.4f\n"
                   "el contenedor es demasiado grande\n",altoSoporte);
        }else{
            printf("El contenedor del resorte tiene un soporte permitido de %.4f\n",
                   altoSoporte);
        }
        printf("\n");
    }

    //comunicación de calculos
    cajaResorteGlobal.grosorTamborcTapas = grosorTotal;
    cajaResorteGlobal.grosorTambor = GrosorCuerpo;
    cajaResorteGlobal.anchoSoporte = anchoSoporte;
}


void Base(int print){

    //Dependencias
    double *profundidadextra = &baseGlobal.extraBase;
    double *anchoExtra = &baseGlobal.extraAncho;
    double *espesens = &baseGlobal.espacioentresensores;
    double *esperessop = &baseGlobal.espacioentreResorteySoporte;
    double *esporillas = &baseGlobal.espacioenorillas;

    double *grosSopor = &soporteGlobal.grosorSoporte;
    double *anchSopor = &soporteGlobal.anchoSoporte;
    double *altoxtraSopor = &soporteGlobal.altoExtradeBase;

    double *grosCajcTap = &cajaResorteGlobal.grosorTamborcTapas;
    double *grosCaj = &cajaResorteGlobal.grosorTambor;
    double *grosTap = &cajaResorteGlobal.grosorTapas;
    double *anchCajaRes = &cajaResorteGlobal.anchoSoporte;
    double *altoxtraCajaRes = &cajaResorteGlobal.altoExtadeBase;

    double *grosSens = &sensorGlobal.GrosorempaqueSensor;
    double *anchSens = &sensorGlobal.AnchoempaqueSensor;
    double *orillaSensorAHueco = &sensorGlobal.GrosorEspacioExtremoATorre;

    //Variables de uso propio
    double soporteMasAncho;
    double soporteextraMasAlto;
    double anchoTotal;
    double altoTotal;
    double grosorTotal;
    double posicionSoporteone;
    double posicionsensorone;
    double posicionsensortwo;
    double posicionSoportetwo;
    double posicionCajRes;
    double distanciaprimerhueco;
    double distanciasegundohueco;

    double mitadAnchoTotal;
    double centroSoportes;
    double centroSensores;
    double centroCajRes;


    soporteMasAncho = *anchCajaRes;
    if(*anchSens > soporteMasAncho){
        soporteMasAncho = *anchSens;
    }
    if(*anchSopor > soporteMasAncho){
        soporteMasAncho = *anchSopor;
    }
    anchoTotal = soporteMasAncho + *anchoExtra;


    soporteextraMasAlto = *altoxtraCajaRes;
    if(*altoxtraSopor > soporteextraMasAlto){
        soporteextraMasAlto = *altoxtraSopor;
    }
    altoTotal = soporteextraMasAlto + *profundidadextra;



    grosorTotal = *esporillas + *grosSopor + *espesens + *grosSens + *grosSens
            + *espesens + *grosSopor + *esperessop
            +*grosCajcTap + *esporillas;

    posicionSoporteone = *esporillas;
    posicionsensorone = posicionSoporteone + *grosSopor + *espesens;
    posicionsensortwo = posicionsensorone +*grosSens;
    posicionSoportetwo = posicionsensortwo +*grosSens +*espesens;
    posicionCajRes = posicionSoportetwo + *grosSopor + *esperessop + *grosTap;
    distanciaprimerhueco = posicionsensorone + *orillaSensorAHueco;
    distanciasegundohueco = posicionsensortwo + *orillaSensorAHueco;

    mitadAnchoTotal = anchoTotal/2;
    centroSoportes = mitadAnchoTotal - (*anchSopor/2);
    centroSensores = mitadAnchoTotal - (*anchSens/2);
    centroCajRes = mitadAnchoTotal - (*anchCajaRes/2);

    if(print == 1){

        printf("\nMEDIDAS BASE\n");
        printf("El dispositivo mas ancho es de %.2f\n", soporteMasAncho);
        printf("El ancho Total de la base es de %.2fmm\n", anchoTotal);
        printf("El dispositivo con más alto extra de base es de %.2f\n",
               soporteextraMasAlto);
        printf("El alto total de la base es de %.4fmm\n", altoTotal);
        printf("El grosor Total es de %.4fmm\n\n", grosorTotal);
        printf("La distancia del primer soporte a la orilla es de: %.4f\n"
               "La distancia del primer sensor a la orilla es de: %.4f\n"
               "La distancia del segundo sensor a la orilla es de: %.4f\n"
               "La distancia del segundo soporte a la orilla es de: %.4f\n"
               "La distancia de la cajaResorte a la orilla es de: %.4f\n",
               posicionSoporteone, posicionsensorone, posicionsensortwo,
               posicionSoportetwo, posicionCajRes);

        printf("\nANCHOS\n");
        printf("El ancho de los soportes de los baleros es de %.4f\n", *anchSopor);
        printf("El ancho de los sensores es de %.4f\n", *anchSens);
        printf("El ancho de la cajaResorte es de %.4f\n", *anchCajaRes);

        printf("\nGROSORES\n");
        printf("El grosor de los soportes de los baleros es de %.4f\n", *grosSopor);
        printf("El grosor del sensor es de %.4f\n", *grosSens);
        printf("El grosor de la cajaResorte sin tapas es de es de %.4f\n", *grosCaj);

        printf("\nCENTROS\n");
        printf("Soporte baleros a %.4f de la orilla de ancho\n", centroSoportes);
        printf("Sensores a %.4f de la orilla de ancho\n", centroSensores);
        printf("CajaResorte a %.4f de la orilla de ancho\n", centroCajRes);

        printf("\nPROFUNDIDADES\n");
        printf("La profundidad (alto) de los agujeros para los soportes\n"
               "de los baleros es de %.4f\n", soporteextraMasAlto);

        printf("\nAGUJEROS SENSORES\n");
        printf("La distancia del agujero que atraviesa la base para el sensor\n"
               "es de %.4f respecto a la orilla\n", distanciaprimerhueco);
        printf("La distancia del agujero que atraviesa la base para el sensor 2\n"
               "es de %.4f respecto a la orilla\n", distanciasegundohueco);
        printf("\n");
    }

    baseGlobal.distanciaSoporte_Extremo = *grosSopor + *espesens + *grosSens + *grosSens
            + *espesens + *grosSopor + *esperessop + *grosCajcTap+ *esporillas;
}

void Flecha(int print){
    //Dependencias
    double *rinterno = &baleroGlobal.radiointerno;
    double *distanciaEntreSopaExt = &baseGlobal.distanciaSoporte_Extremo;
    double *esporillas = &baseGlobal.espacioenorillas;
    double *tamanioCajaResorte = &cajaResorteGlobal.grosorTamborcTapas;
    double *gruesCuer = &flechaGlobal.extraGruesoparaCuerda;

    //Variables para uso propio
    double radioCirculo;
    double angulo;
    double anguloRadianes;
    double diagonalCuadrado;
    double catetopuesto;
    double lados;
    double grosor;
    double anchoLineaResorte;
    double posicionLineaResorte;
    double gruesoLinea;

    //cilindro
    double anchocilindrocable;
    double radiocilindrocable;
    double posicioncilcable;


    //Calculos
    radioCirculo = *rinterno;
    angulo = 45;
    anguloRadianes = (angulo * PI)/180;
    diagonalCuadrado = radioCirculo * 2;
    catetopuesto = diagonalCuadrado * sin(anguloRadianes);
    lados = catetopuesto;

    grosor = *distanciaEntreSopaExt+*gruesCuer;
    anchoLineaResorte = (1/5.0)*catetopuesto;
    posicionLineaResorte = *distanciaEntreSopaExt
            + *gruesCuer - *tamanioCajaResorte - *esporillas;
    gruesoLinea = *tamanioCajaResorte +*esporillas;

    //cilindro
    anchocilindrocable = lados;
    radiocilindrocable = lados/4;
    posicioncilcable = *gruesCuer/2;

    if(print == 1){

        printf("\nMEDIDAS FLECHA\n");
        printf("El radio del Círculo es de: %.4fmm\n", radioCirculo);
        printf("Los lados del cuadrado deben de ser de: %.4fmm\n", lados);
        printf("El grosor de la flecha debe de ser de: %.4f\n", grosor);
        printf("El ancho de la linea del resorte debe de ser de: %.4f\n", anchoLineaResorte);
        printf("El  grosor de la linea debe de ser de:%.4f y debe estar a una distancia\n"
               "de %.4f a  partir de un extremo\n", gruesoLinea, posicionLineaResorte);
         printf("El ancho del cilindro para el cable es de %.4f\n", anchocilindrocable);
        printf("El radio del cilindro para el cable es de %.4f\n", radiocilindrocable);
        printf("La posición del cilindro debe de ser de %.4f\n", posicioncilcable);
        printf("\n");

        printf("\n--CHECKING FOR POSSIBLE ERRORS--\n");
        printf("\nANCHO LINEA RESORTE\n");
        if(anchoLineaResorte <=1){
            printf("ERROR: EL ancho de la línea de %.4f del resorte es igual o menor"
                   " que 1mm", anchoLineaResorte);
        }else{
            printf("El ancho de la línea de %.4f del resorte es mayor que 1mm",
                   anchoLineaResorte);
        }

        printf("\nRADIO DEL CILÍNDRO\n");
        if(radiocilindrocable <=1){
            printf("ERROR: El radio del cilíndro de %.4f del resorte es igual o menor"
                   " que 1mm", radiocilindrocable);
        }else{
            printf("El radio del cilíndro de %.4f del resorte es mayor que 1mm",
                   radiocilindrocable);
        }
        printf("\n");
    }
}


