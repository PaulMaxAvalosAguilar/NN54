#include "i2c.h"
#include "FreeRTOS.h"
#include "task.h"

static inline TickType_t diff_ticks(TickType_t early,TickType_t later){

	if ( later >= early )
		return later - early;
	return ~(TickType_t)0 - early + 1 + later;
}

void i2c_configure(uint32_t i2cDevice){
  i2c_peripheral_disable(i2cDevice);
  i2c_reset(i2cDevice);
  I2C_CR1(i2cDevice) &= ~I2C_CR1_STOP;	// Clear stop
  i2c_set_standard_mode(i2cDevice);	// 100 kHz mode
  i2c_set_clock_frequency(i2cDevice,I2C_CR2_FREQ_36MHZ); // APB Freq
  i2c_set_trise(i2cDevice,36);		// 1000 ns
  i2c_set_dutycycle(i2cDevice,I2C_CCR_DUTY_DIV2);
  i2c_set_ccr(i2cDevice,180);		// 100 kHz <= 180 * 1 /36M
  i2c_peripheral_enable(i2cDevice);
}

void i2c_start_addr(uint32_t i2cDevice, uint8_t i2c_addr, enum I2C_RW rw){
  TickType_t t0 = xTaskGetTickCount();

  while ( I2C_SR2(i2cDevice) & I2C_SR2_BUSY );	// Block until not busy
  I2C_SR1(i2cDevice) &= ~I2C_SR1_AF;	// Clear Acknowledge failure
  i2c_clear_stop(i2cDevice);		// Do not generate a Stop
  if ( rw == Read )
    i2c_enable_ack(i2cDevice);
  i2c_send_start(i2cDevice);		// Generate a Start/Restart

  // Loop until ready or timeout
  while ( !((I2C_SR1(i2cDevice) & I2C_SR1_SB) 
	    && (I2C_SR2(i2cDevice) & (I2C_SR2_MSL|I2C_SR2_BUSY))) ) {
    if ( diff_ticks(t0,xTaskGetTickCount()) > timeoutInTicks)
      taskYIELD();
  }

  // Send Address & R/W flag:
  i2c_send_7bit_address(i2cDevice,i2c_addr,rw == Read ? I2C_READ : I2C_WRITE);

  // Wait until completion, NAK or timeout
  t0 = xTaskGetTickCount();
  while ( !(I2C_SR1(i2cDevice) & I2C_SR1_ADDR) ) {
    if ( I2C_SR1(i2cDevice) & I2C_SR1_AF ) {
      i2c_send_stop(i2cDevice);
      (void)I2C_SR1(i2cDevice);
      (void)I2C_SR2(i2cDevice); 	// Clear flags
      // NAK Received (no ADDR flag will be set here)
    }
    if ( diff_ticks(t0,xTaskGetTickCount()) > timeoutInTicks)
      taskYIELD();
  }

  (void)I2C_SR2(i2cDevice);		// Clear flags
}

void i2c_write(uint32_t i2cDevice,uint8_t byte){
  TickType_t t0 = xTaskGetTickCount();

  i2c_send_data(i2cDevice,byte);
  while ( !(I2C_SR1(i2cDevice) & (I2C_SR1_BTF)) ) {
    if ( diff_ticks(t0,xTaskGetTickCount()) > timeoutInTicks )
      taskYIELD();
  }
}

void i2c_stopI2C(uint32_t i2cDevice){
  i2c_send_stop(i2cDevice);
}


