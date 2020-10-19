#include "i2c.h"
#include <libopencm3/stm32/i2c.h>

/*
void i2c_start_addr(uint32_t i2cDevice, uint8_t i2c_addr, enum I2C_RW rw){

  while ( I2C_SR2(i2cDevice) & I2C_SR2_BUSY );	// Block until not busy
  I2C_SR1(i2cDevice) &= ~I2C_SR1_AF;	// Clear Acknowledge failure
  i2c_clear_stop(i2cDevice);		// Do not generate a Stop
  if ( rw == Read )
    i2c_enable_ack(i2cDevice);
  i2c_send_start(i2cDevice);		// Generate a Start/Restart

  // Loop until ready or timeout
  while ( !((I2C_SR1(i2cDevice) & I2C_SR1_SB) 
	    && (I2C_SR2(i2cDevice) & (I2C_SR2_MSL|I2C_SR2_BUSY))) ) {
  }

  // Send Address & R/W flag:
  i2c_send_7bit_address(i2cDevice,i2c_addr,rw == Read ? I2C_READ : I2C_WRITE);

  // Wait until completion, NAK or timeout
  while ( !(I2C_SR1(i2cDevice) & I2C_SR1_ADDR) ) {
    if ( I2C_SR1(i2cDevice) & I2C_SR1_AF ) {
      i2c_send_stop(i2cDevice);
      (void)I2C_SR1(i2cDevice);
      (void)I2C_SR2(i2cDevice); 	// Clear flags
      // NAK Received (no ADDR flag will be set here)
    }
  }

  (void)I2C_SR2(i2cDevice);		// Clear flags
}

void i2c_write(uint32_t i2cDevice,uint8_t byte){
  i2c_send_data(i2cDevice,byte);
  while ( !(I2C_SR1(i2cDevice) & (I2C_SR1_BTF)) ) {
  }
}

void i2c_stopI2C(uint32_t i2cDevice){
  i2c_send_stop(i2cDevice);
}
*/

void i2c_transmit_polling(uint8_t i2c_addr, uint8_t *buffer, uint8_t nbytes){
  
  while ( I2C_SR2(I2C1) & I2C_SR2_BUSY );	// Block until not busy
  I2C_SR1(I2C1) &= ~I2C_SR1_AF;	// Clear Acknowledge failure
  i2c_clear_stop(I2C1);		// Do not generate a Stop
  i2c_send_start(I2C1);		// Generate a Start/Restart
  // Loop until ready or timeout
  while ( !((I2C_SR1(I2C1) & I2C_SR1_SB) 
	    && (I2C_SR2(I2C1) & (I2C_SR2_MSL|I2C_SR2_BUSY))) ) {
  }
  // Send Address & R/W flag:
  i2c_send_7bit_address(I2C1,i2c_addr,I2C_WRITE);
  // Wait until completion, NAK or timeout
  while ( !(I2C_SR1(I2C1) & I2C_SR1_ADDR) ) {
    if ( I2C_SR1(I2C1) & I2C_SR1_AF ) {
    }
  }
  (void)I2C_SR2(I2C1);		// Clear flags
  
  for(uint8_t i = 0; i < nbytes; i++){
    i2c_send_data(I2C1,*buffer++);
    while ( !(I2C_SR1(I2C1) & (I2C_SR1_BTF)) ) {
    }
  }

  i2c_send_stop(I2C1);
}
