#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Archivos.h"
#include "Calculations.h"
#include "mathHelp.h"

void cleanScreen(void);
void thirdMenuInstructions(void);//Funcion for displaying calculations instructions
int checkyorn(char);//function to check for y or no values
void calculations(void);//Function for initiate calculations and display them
void typeagain(int *election);


int main(int argc, char *argv[]){

  //medidas en milímetros

  //Sensores
  sensorGlobal.AnchodelTransistor = 1.0;
  sensorGlobal.AltodelTransistor = 4.0;
  sensorGlobal.GrosorempaqueSensor = 24.4;
  sensorGlobal.AnchoempaqueSensor = 6;
  sensorGlobal.AltoempaqueSensor = 10.4;
  sensorGlobal.GrosorEspaciodiodoTransistor = 3.1;
  sensorGlobal.GrosorEspacioExtremoATorre = 6.25;
  sensorGlobal.EspacioAlturaparaDisco = 7.4;

  //Baleros
  baleroGlobal.radiointerno = 4.5;
  baleroGlobal.radioexterno = 12.0;
  baleroGlobal.grosorBalero = 7.0;

  //Fleje
  flejeGlobal.grueso_fleje = 13;

  //Disco
  discoGlobal.AnchoSeguridad= 2.0;
  discoGlobal.AltoSeguridad = 1.0;
  discoGlobal.agujerosDeseados = 10.0;
  discoGlobal.gruesoDisco = 1.5;

  //PosSensores
  posSensoresGlobal.n1 = 2;
  posSensoresGlobal.n2 = 2;
  posSensoresGlobal.x = 0;

  //Soporte
  soporteGlobal.anguloParaAncho= 60.0;
  soporteGlobal.altoExtradeBase= 3.0;
  soporteGlobal.extradioExterno = 3.0;

  //CajaResorte
  cajaResorteGlobal.extraGrosorTambor = 4;
  cajaResorteGlobal.grosorTapas = 2;
  cajaResorteGlobal.radioCilInterno = 23;
  cajaResorteGlobal.extradioexterno = 3;
  cajaResorteGlobal.angulo = 90;
  cajaResorteGlobal.altoExtadeBase = 3;
  cajaResorteGlobal.extradioAlma = 2;

  //Base
  baseGlobal.extraBase = 3.0;
  baseGlobal.extraAncho = 10.0;
  baseGlobal.espacioentresensores = 3.0;
  baseGlobal.espacioentreResorteySoporte = 2.0;
  baseGlobal.espacioenorillas = 2.0;

  //Flecha
  flechaGlobal.extraGruesoparaCuerda = 50;

  cleanScreen();
  calculations();

  return 0;
}

void cleanScreen(){
  for(int i=0; i<=160;i++)
    printf("\n");
}



void thirdMenuInstructions(void){
  printf("---Menu de cálculos---\n\n"
	 "Cálculos Mecánicos:\n"
	 "Oprime 1 para ver los calculos del disco\n"
	 "Oprime 2 para ver los calculos de la posicion del Soporte\n"
	 "Oprime 3 para ver los calculos de los soportes\n"
	 "Oprime 4 para ver los calculos de la Caja del Resorte\n"
	 "Oprime 5 para ver los calculos de la Base\n"
	 "Oprime 6 para ver los calculos de la Flecha\n"
	 "\nCálculos Electrónicos:\n"
	 "Oprime -1 para salir");
  printf("\n");

}


int checkyorn(char repeatanswer){
  if(repeatanswer == 'y'){
    return 0;
  }else if(repeatanswer == 'n'){
    return 0;
  }else{
    return 1;
  }
}

void calculations(void){

  int election;
  thirdMenuInstructions();

  while(scanf("%d", &election)!=1){
    scanf("%*s");
    cleanScreen();
    thirdMenuInstructions();
  }

  cleanScreen();

  while(election != -1){
    if(election ==1){

      Disk(1);
      typeagain(&election);
    }else if(election ==2){
      Disk(0);
      Sensores(1);
      typeagain(&election);
    }else if(election ==3){
      Disk(0);
      Soportes(1);
      typeagain(&election);
    }else if(election ==4){
      Disk(0);
      Soportes(0);
      CajaResorte(1);
      typeagain(&election);
    }else if(election ==5){
      Disk(0);
      Soportes(0);
      CajaResorte(0);
      Base(1);
      typeagain(&election);
    }else if(election ==6){
      Disk(0);
      Soportes(0);
      CajaResorte(0);
      Base(0);
      Flecha(1);
      typeagain(&election);
    }else{
      printf("No option with that number\n");
      typeagain(&election);
    }
    cleanScreen();
  }
  cleanScreen();
}

void typeagain(int *election){
  char repeatanswer;

  //Give option to repeat
  printf("Do you want to make calculations again?[y/n]:");
  scanf(" %c", &repeatanswer);

  while(checkyorn(repeatanswer)){
    printf("Type y or n\n");
    printf("Do you want to make calculations again?[y/n]:");
    scanf(" %c", &repeatanswer);
  }

  if(repeatanswer == 'n'){
    *election = -1;
  }else{
    cleanScreen();
    thirdMenuInstructions();
    while(scanf("%d", election)!=1){
      scanf("%*s");
      cleanScreen();
      thirdMenuInstructions();
    }
  }
}

