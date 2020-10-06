#ifndef i2c_h
#define i2c_h

#include <stdint.h>

#define timeoutInTicks 1000

void i2c_transmit_polling(uint8_t i2c_addr, uint8_t *buffer, uint8_t nbytes);
void i2c_transmit_dma(uint8_t i2c_addr, uint8_t *buffer, uint8_t nbytes);

#endif /* i2c_h */
