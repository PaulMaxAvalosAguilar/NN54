#ifndef i2c_h
#define i2c_h

#include <stdint.h>

#define timeoutInTicks 1000

void i2c_start(uint8_t i2c_addr, uint8_t nbytes);
void i2c_write(uint8_t byte);

#endif /* i2c_h */
