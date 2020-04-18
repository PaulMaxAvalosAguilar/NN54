#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fixmath.h"

int32_t catan2(int32_t xvalue, int32_t yvalue);
int32_t cCos(int32_t desiredAngle);
double arccos_cordic ( double t, int n );

int main(int argc, char *argv[]){
  /*
  int32_t y = 10;
  int32_t x = 42;
  int32_t angle = catan2(y,x);

  fix16_t yFix = fix16_from_int(y);
  fix16_t xFix = fix16_from_int(x);
  fix16_t angleFix = 0;

  angleFix = fix16_atan2(y,x);
  */

  /*
  printf("%.3f %.3f\n", fix16_to_float(angleFix), atan2((double)y, (double)x) );
  printf("%.3f %d %.3f\n", fix16_to_float(angleFix) * 180/PI, catan2(y,x),
	 atan2((double)y, (double)x) * 180/PI );
  */

  double angleA = 45.10;
  double angleB = 44.66;
  
  double phiAngle = acos(1/sqrt(tan(angleA * PI/180) *tan(angleA * PI/180) + tan(angleB * PI/180) * tan(angleB * PI/180) +1)) * 180/PI;
  //  double myPhiAngle = atan(sqrt(tan(angleA * PI/180) *tan(angleA * PI/180) + tan(angleB * PI/180) * tan(angleB * PI/180) )) * 180/PI ;
  double myPhiAngle = acos( cos(angleA * PI/180) * cos(angleB * PI/180))  * 180/PI;

  int angle = 800000;

  printf("%.6f %.6f", cCos(angle)/1000000.0, cos((angle/ 1000000.0) * PI/180));
  
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

int32_t cCos(int32_t desiredAngle){

  int32_t currentAngle = 0;

  int32_t y = 0;
  int32_t x = 607252;

  int32_t iterations = 0;

  int32_t xRotationResult = x;
  int32_t yRotationResult = y;
    
  int32_t angles[16] = {
			 45000000, 26565000, 14036000, 7125000, 3576000, 1790000, 895000, 448000, 224000, 112000, 56000, 28000, 14000, 7000, 3000
  };

  while( iterations < 16){

    if( currentAngle > desiredAngle){
      
      xRotationResult = x + (y >>  iterations);
      yRotationResult = y - (x >>  iterations);

      x = xRotationResult;
      y = yRotationResult;

      currentAngle = currentAngle - angles[iterations];

    }else{

      xRotationResult = x - (y >> iterations);
      yRotationResult = y + (x >> iterations);
      x = xRotationResult;
      y = yRotationResult;

      currentAngle = currentAngle + angles[iterations];
    }

    iterations++;
  }

  return xRotationResult;
}


