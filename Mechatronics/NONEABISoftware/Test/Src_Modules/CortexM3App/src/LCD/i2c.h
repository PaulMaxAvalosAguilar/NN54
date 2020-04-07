#ifndef i2c_h
#define i2c_h

#include <stdint.h>
#include <libopencm3/stm32/i2c.h>

enum I2C_RW {
	Read = 1,
	Write = 0
};

#define timeoutInTicks 1000

#define i2c_init() i2c_configure(I2C1)
#define i2c_start(addr,rw) i2c_start_addr(I2C1,addr,rw)
#define i2c_byte(byte) i2c_write(I2C1, byte)
#define i2c_stop() i2c_stopI2C(I2C1)

void i2c_configure(uint32_t i2cDevice);
void i2c_start_addr(uint32_t i2cDevice, uint8_t i2c_addr, enum I2C_RW rw);
void i2c_write(uint32_t i2cDevice,uint8_t byte);
void i2c_stopI2C(uint32_t i2cDevice);

#endif /* i2c_h */
