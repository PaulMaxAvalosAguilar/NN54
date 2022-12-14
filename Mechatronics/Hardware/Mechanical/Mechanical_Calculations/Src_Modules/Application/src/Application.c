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

void (*functions[5])(int) = {Disk,SoportesBaleros, SoportesSensores,SistemaTraccion};

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

  //Soporte
  soporteGlobal.anguloParaAncho= 60.0;
  soporteGlobal.altoExtradeBase= 3.0;
  soporteGlobal.extradioExterno = 3.0;

  //PosSensores
  posSensoresGlobal.n1 = 3;
  posSensoresGlobal.n2 = 3;
  posSensoresGlobal.x = 0;

  //CajaResorte
  cajaResorteGlobal.extraGrosorTambor = 1.5;
  cajaResorteGlobal.radioCilInterno = 22;
  cajaResorteGlobal.extradioexterno = 4;

  //Flecha
  flechaGlobal.angulo = 90;
  flechaGlobal.offset = .1;

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
	 "1 :calculos del disco\n"
	 "2 :calculos de los soportes baleros\n"
	 "3 :calculos de los soportes sensores\n"
	 "4 :calculos sistema tracción\n"
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
    if(election >=1 || election <=5){
      for(int i = 0; i < election;i++){
	(*functions[i])(0);
      }
      (*functions[election-1])(1);
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

