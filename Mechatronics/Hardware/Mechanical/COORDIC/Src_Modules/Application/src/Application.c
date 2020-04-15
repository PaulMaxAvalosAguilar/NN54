#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fixmath.h"

int32_t catan2(int32_t xvalue, int32_t yvalue);

int main(int argc, char *argv[]){

  int32_t y = 10;
  int32_t x = 42;
  int32_t angle = catan2(y,x);

  fix16_t yFix = fix16_from_int(y);
  fix16_t xFix = fix16_from_int(x);
  fix16_t angleFix = 0;

  angleFix = fix16_atan2(y,x);

  /*
  printf("%.3f %.3f\n", fix16_to_float(angleFix), atan2((double)y, (double)x) );
  printf("%.3f %d %.3f\n", fix16_to_float(angleFix) * 180/PI, catan2(y,x),
	 atan2((double)y, (double)x) * 180/PI );
  */

  double angleA = 45;
  double angleB = 45;
  
  double phiAngle = acos(1/sqrt(tan(angleA * PI/180) *tan(angleA * PI/180) + tan(angleB * PI/180) * tan(angleB * PI/180) +1)) * 180/PI;
  double myPhiAngle = acos( ((angleB * (1/90)) * cos(angleA) )/1 ) * 180/PI ;

  printf("%.3f %.3f", phiAngle, myPhiAngle);
  
  return 0;
}

int32_t catan2(int32_t xvalue, int32_t yvalue){

  int32_t y = xvalue * 1000;
  int32_t x = yvalue * 1000;

  int32_t iterations = 0;

  int32_t xRotationResult = x;
  int32_t yRotationResult = y;
    
  int32_t sum = 0;

  int32_t angles[16] = {
			 45000, 26565, 14036, 7125, 3576, 1790, 895, 448, 224, 112, 56, 28, 14, 7, 3
  };


  while( iterations < 16){

    if(yRotationResult > 0){
      
      xRotationResult = x + (y >>  iterations);
      yRotationResult = y - (x >>  iterations);

      x = xRotationResult;
      y = yRotationResult;


      sum += angles[iterations]; 

    }else{

      xRotationResult = x - (y >> iterations);
      yRotationResult = y + (x >> iterations);
      x = xRotationResult;
      y = yRotationResult;
      
      sum -= angles[iterations]; 
    }

    iterations++;
  }

  return sum;
}


