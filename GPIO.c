/** ****************************************************************************
 *   FRDM-KL46Z - Peripherals
 *    Module: GPIO
 *    Author: Jan Szemiet
 *    Version: 0.1
 *  ****************************************************************************
 */

#include "MKL46Z4.h"

#include "GPIO.h"


void GPIO_Config(void){
  PORTC->PCR[3]  = PORT_PCR_MUX(1)          // Alternative function: 1 (GPIO)
                 | PORT_PCR_PE_MASK         // Enable pullup/pulldown resistor
                 | PORT_PCR_PS_MASK;        // Select pullup resistor
  PORTC->PCR[12] = PORT_PCR_MUX(1)          // Alternative function: 1 (GPIO)
                 | PORT_PCR_PE_MASK         // Enable pullup/pulldown resistor
                 | PORT_PCR_PS_MASK;        // Select pullup resistor
  PORTD->PCR[5]  = PORT_PCR_MUX(1);         // Alternative function: 1 (GPIO)
  PORTE->PCR[29] = PORT_PCR_MUX(1);         // Alternative function: 1 (GPIO)
  
  PORTE->PCR[0]  = PORT_PCR_MUX(6);         // Alternative function: 6 (I2C1_SDA)
  PORTE->PCR[1]  = PORT_PCR_MUX(6);         // Alternative function: 6 (I2C1_SCL)
  
  GPIO_ChangePortBitDirection(LED_GREEN, OUTPUT);
  GPIO_ChangePortBitDirection(LED_RED, OUTPUT);
  GPIO_ChangePortBitDirection(SW_1, INPUT);
  GPIO_ChangePortBitDirection(SW_2, INPUT);
  
  GPIO_SetPortBit(LED_GREEN);               // Turn off green LED
  GPIO_SetPortBit(LED_RED);                 // Turn off red LED
}


void GPIO_ChangePortBitDirection(GPIO_Type* port, uint8_t bit, GPIO_IO_Dir dir){
  if(dir == INPUT){
    port->PDDR &= ~((uint32_t)1<<bit);
  }
  else if(dir == OUTPUT){
    port->PDDR |= ((uint32_t)1<<bit);
  }
}

void GPIO_ClearPortBit(GPIO_Type* port, uint8_t bit){
  port->PCOR = (uint32_t)1<<bit;
}

uint8_t GPIO_ReadPortBit(GPIO_Type* port, uint8_t bit){
  return (uint8_t)(((port->PDIR)>>bit)&0x1);
}

void GPIO_SetPortBit(GPIO_Type* port, uint8_t bit){
  port->PSOR = (uint32_t)1<<bit;
}

void GPIO_TogglePortBit(GPIO_Type* port, uint8_t bit){
  port->PTOR = (uint32_t)1<<bit;
}

void GPIO_WritePortBit(GPIO_Type* port, uint8_t bit, GPIO_OUT_State state){
  port->PDOR = (port->PDOR & ~((uint32_t)1<<bit)) | ((uint32_t)state<<bit);
}
