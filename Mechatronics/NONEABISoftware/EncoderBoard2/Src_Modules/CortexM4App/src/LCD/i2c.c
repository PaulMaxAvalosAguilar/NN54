#include "stm32g431xx.h"
#include "i2c.h"

void i2c_transmit_polling(uint8_t i2c_addr, uint8_t *buffer, uint8_t nbytes){
  while(I2C2->ISR & I2C_ISR_BUSY); //Wait till bus is free
  
  I2C2->CR2 = (I2C2->CR2 & (~I2C_CR2_SADD)) | (i2c_addr << I2C_CR2_SADD_Pos);//Slave address
  I2C2->CR2 &= ~I2C_CR2_RD_WRN; //Master requests a write transfer
  I2C2->CR2 = (I2C2->CR2 & (~I2C_CR2_NBYTES)) | (nbytes << I2C_CR2_NBYTES_Pos);//Nuber of bytes to be transfered
  I2C2->CR2 |= I2C_CR2_AUTOEND; //Automatic end mode
  I2C2->CR2 |= I2C_CR2_START; //Start generation

  while((I2C2->CR2 & I2C_CR2_START));//Wait till I2C is cleared

  for(uint8_t i = 0; i < nbytes; i++){
    while(!(I2C2->ISR & I2C_ISR_TXIS));
    I2C2->TXDR = *buffer++; //Send TXIS
  }  
}

void i2c_transmit_dma(uint8_t i2c_addr, uint8_t *buffer, uint8_t nbytes){
  while(I2C2->ISR & I2C_ISR_BUSY); //Wait till bus is free
  for (int i = 0; i < 10000; i++);
  
  I2C2->CR2 = (I2C2->CR2 & (~I2C_CR2_SADD)) | (i2c_addr << I2C_CR2_SADD_Pos);//Slave address
  I2C2->CR2 &= ~I2C_CR2_RD_WRN; //Master requests a write transfer
  I2C2->CR2 = (I2C2->CR2 & (~I2C_CR2_NBYTES)) | (nbytes << I2C_CR2_NBYTES_Pos);//Nuber of bytes to be transfered
  I2C2->CR2 |= I2C_CR2_AUTOEND; //Automatic end mode

  I2C2->TXDR = *buffer++;
  
  DMA1_Channel3->CCR &= ~DMA_CCR_EN;//Channel disable
  DMA1_Channel3->CMAR = (uint32_t)buffer;//DMA source address
  DMA1_Channel3->CNDTR = nbytes-1;//DMA length
  DMA1_Channel3->CCR |= DMA_CCR_EN;//Channel enable
  
  I2C2->CR2 |= I2C_CR2_START; //Start generation

  while((I2C2->CR2 & I2C_CR2_START));//Wait till I2C is cleared
  while(!(DMA1->ISR & DMA_ISR_TCIF3));//Wait till transfere complete
  DMA1->IFCR |= DMA_IFCR_CTCIF3;//Clear transfere complete
 }


