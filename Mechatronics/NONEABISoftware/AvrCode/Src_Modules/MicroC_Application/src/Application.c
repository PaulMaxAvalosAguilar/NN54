#include "header.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>
#include "communication.h"
#include "BatteryManagement.h"
#include "Encoder.h"

int main(void) {

  initEncoder();//Should go first
  initBatteryManagement();
  initCommunication();//enables interruptions automatically
  OLED_puts("Encoder On",2);
  
  while(1){
    
    readAllSerial();

    //After CMD is read and handles found
    if(rn4020.allUUIDHandlesMatchesFound){
      uint16_t detectedPosition = 0;
      int recolectedEncoderValues = 0;
      int16_t valuesBuff[10];
      
      uint16_t voltageResult = 0;
      int8_t batteryPercentage = 0;
      char percentBuff[3];
      
      
      OLED_puts("Transmiting",2);

      //initiate Loop
      while(1){


	readAllSerial();


	
	//Writting a biger than characteristic size
	//value will make rn4020 urresponsive so take care
	//with disconnecting encoder related wires which make
	//microcontroller read big Values

	if(getPositionValues(&detectedPosition) != -1){
	  valuesBuff[recolectedEncoderValues] = detectedPosition;
	  recolectedEncoderValues++;	  
	  if(recolectedEncoderValues == 10){
	    writeTenTwoBytesCharacteristic(ENCODER_VALUE,
					   valuesBuff[0],
					   valuesBuff[1],
					   valuesBuff[2],
					   valuesBuff[3],
					   valuesBuff[4],
					   valuesBuff[5],
					   valuesBuff[6],
					   valuesBuff[7],
					   valuesBuff[8],
					   valuesBuff[9]);
	    recolectedEncoderValues = 0;
	  }
	  	  
	}
	

	//TEST FOR CHECKING ENCODER TIMER INTERRUPT EXECUTION
	/*
	while(getPositionValues(&detectedPosition) != -1){
	  printf("SHW,001B,00%04X\n",detectedPosition);
	}
	*/	

	/*
	detectedPosition = 100;
	writeTwoByteCharacteristic(ENCODER_VALUE, &detectedPosition);

	detectedPosition = -100;
	writeTwoByteCharacteristic(BATTERYCHARGING_VALUE, &detectedPosition);

	detectedPosition = -70;
	writeTwoByteCharacteristic(BATTERYLEVEL_VALUE, &detectedPosition);
	
	_delay_ms(500);
	*/

	if(analogBufferAvailable()){
	  voltageResult = getAnalogVoltage();
	  batteryPercentage = getBatteryPercentage(voltageResult);
	  writeOneOneByteCharacteristic(BATTERYLEVEL_VALUE, batteryPercentage);
	  
	  sprintf(percentBuff,"Charge: %d%%",batteryPercentage);
	  OLED_puts(percentBuff,6);
      
	  if(batteryIsCharging()){
	    OLED_puts("     --CHARGING--",7);
	  }else{
	    if(batteryPercentage == 0){
	      OLED_puts("      CHARGE NOW!",7);
	    }else{
	      OLED_puts("",7);
	    }
	  }
	  	  
	}      
      }
    }
  } 

    return 0;
}










