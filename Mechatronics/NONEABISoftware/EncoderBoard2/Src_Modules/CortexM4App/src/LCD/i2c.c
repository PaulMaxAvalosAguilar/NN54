#include "stm32g431xx.h"
#include "i2c.h"

void i2c_start(uint8_t i2c_addr, uint8_t nbytes){
  while(I2C2->ISR & I2C_ISR_BUSY); //Wait till bus is free
  
  I2C2->CR2 = (I2C2->CR2 & (~I2C_CR2_SADD)) | (i2c_addr << I2C_CR2_SADD_Pos);//Slave address
  I2C2->CR2 &= ~I2C_CR2_RD_WRN; //Master requests a write transfer
  I2C2->CR2 = (I2C2->CR2 & (~I2C_CR2_NBYTES)) | (nbytes << I2C_CR2_NBYTES_Pos);//Nuber of bytes to be transfered
  I2C2->CR2 |= I2C_CR2_AUTOEND; //Automatic end mode
  I2C2->CR2 |= I2C_CR2_START; //Start generation

  //Clear no acknowledge flag
  //Clear stop flag

  while((I2C2->CR2 & I2C_CR2_START));//Wait till I2C is cleared
  
}

void i2c_write(uint8_t byte){
  while(!(I2C2->ISR & I2C_ISR_TXIS));
  I2C2->TXDR = byte; //Send TXIS
}


