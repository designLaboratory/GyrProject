#include "i2c.h"
#include "GPIO.h"
#include "clock.h"


int main(void)
	{
		uint8_t a=0x0E;
		uint8_t b=0x07;
		volatile uint8_t buf=0;
	Clock_Config();
  GPIO_Config();
  I2C0_Init();
	
	for(;;)
	{
		buf= read_register(a,b);
	}
}

