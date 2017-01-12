#ifndef i2c_h
#define i2c_h

#include "MKL46Z4.h"


#define I2C_MASTER_WRITE_BIT 0x00 /* Master write */    
#define I2C_MASTER_READ_BIT  0x01 /* Master read */
#define I2C_MASTER_WRITE(x) (((x)<<1)|0x00)/* Master write */    
#define I2C_MASTER_READ(x)  (((x)<<1)|0x01)/* Master read */	

typedef enum{
  I2C_ACK = 0,
  I2C_NACK = 1,
} I2C_Acknowledge_Bit;
uint8_t read_register(uint8_t adress, uint8_t device_register);
void I2C0_Init(void);
#endif

