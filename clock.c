#include "clock.h"

void Clock_Config(void){
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);
  
  SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;        // Enable clock for I2C1
  SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK        // Enable clock for Port A
              | SIM_SCGC5_PORTB_MASK        // Enable clock for Port B
              | SIM_SCGC5_PORTC_MASK        // Enable clock for Port C
              | SIM_SCGC5_PORTD_MASK        // Enable clock for Port D
              | SIM_SCGC5_PORTE_MASK;       // Enable clock for Port E
}
