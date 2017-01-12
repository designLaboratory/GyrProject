#include "i2c.h"


void I2C0_Init(void){
  // Set baudrate to 400 kb/s
  I2C0->F   = I2C_F_MULT(2)
            | I2C_F_ICR(0x5);
}



 void I2C0_Disable(void){
  // Wylaczenie modulu I2C, bit IICEN = 0
  I2C0->C1 &= ~I2C_C1_IICEN_MASK;
}
 
void I2C0_Enable(void){
  // Wlaczenie modulu I2C, bit IICEN = 1
  I2C0->C1 |= I2C_C1_IICEN_MASK;
}
 
uint8_t I2C0_ReadByte( uint8_t ack){
  // Wybór trybu odbioru, bit TX = 0
  I2C0->C1 &= ~I2C_C1_TX_MASK;
 
  // Zapis wartosci bitu ACK/NACK przed odczytem bajta jezeli bit FACK == 0
  if((I2C0->SMB & I2C_SMB_FACK_MASK) == 0)
    I2C0->C1 = (ack == I2C_NACK) ? I2C0->C1 | I2C_C1_TXAK_MASK : I2C0->C1 & ~I2C_C1_TXAK_MASK;
 
  // Wyzerowanie flagi IICIF
  I2C0->S |= I2C_S_IICIF_MASK;
 
  // Wymuszenie odbioru jednego bajta danych i odrzucenie starej wartosci z rejestru D
  (void)I2C0->D;
  // Oczekiwanie na zakonczenie transmisji danych oraz bitu ACK/NACK (dla FACK == 0)
  while((I2C0->S & I2C_S_IICIF_MASK) == 0);
 
  // Wyslanie bitu ACK/NACK jezeli bit FACK == 1
  if((I2C0->SMB & I2C_SMB_FACK_MASK) != 0)
    I2C0->C1 = (ack == I2C_NACK) ? I2C0->C1 | I2C_C1_TXAK_MASK : I2C0->C1 & ~I2C_C1_TXAK_MASK;
 
  // Wybór trybu nadawania w celu odczytania odebranego bajta i pominiecia
  //  kolejnego transferu danych, bit TX = 1
  I2C0->C1 |= I2C_C1_TX_MASK;
 
  // Zwrócenie odebranej wartosci 8-bitowej
  return I2C0->D;
}
 
void I2C0_Restart(void){
  // Wygenerowanie bitu RESTART, RSTA = 1
  I2C0->C1 |= I2C_C1_RSTA_MASK;
}
 
void I2C0_Start(void){
  // Wybór trybu Master i jednoczesne wygenerowanie bitu START, MST = 0 -> 1
  I2C0->C1 |= I2C_C1_MST_MASK;
}
 
void I2C0_Stop(void){
  // Wyzerowanie flagi STOPF
  I2C0->FLT |= I2C_FLT_STOPF_MASK;
 
  // Wybór trybu Slave i jednoczesne wygenerowanie bitu STOP, MST = 1 -> 0
  I2C0->C1 &= ~I2C_C1_MST_MASK;
  // Oczekiwanie na wyslanie bitu STOP
  while((I2C0->FLT & I2C_FLT_STOPF_MASK) == 0){
    I2C0->C1 &= ~I2C_C1_MST_MASK;
  }
}
 
uint8_t I2C0_WriteByte(uint8_t data){
  // Wybór trybu nadawania, bit TX = 1
  I2C0->C1 |= I2C_C1_TX_MASK;
 
  // Wyzerowanie flagi IICIF
  I2C0->S |= I2C_S_IICIF_MASK;
 
  // Wymuszenie wyslania jednego bajta danych
  I2C0->D = data;
  // Oczekiwanie na zakonczenie transmisji danych i odbiór bitu ACK/NACK
  while((I2C0->S & I2C_S_IICIF_MASK) == 0);
 
  // Zwrócenie wartosci odebranego bitu ACK/NACK
  return ((I2C0->S & I2C_S_RXAK_MASK) == I2C_S_RXAK_MASK ? I2C_NACK : I2C_ACK);
}

uint8_t read_register(uint8_t adress, uint8_t device_register)
{
	  uint8_t temperature;
 
  I2C0_Enable();                           // Wlaczenie modulu I2C1
 
  I2C0_Start();                            // Wyslanie bitu Start
  I2C0_WriteByte(I2C_MASTER_WRITE(adress));     // Wyslanie adresu TC74 z bitem W
  I2C0_Restart();                          // Wyslanie bitu Restart
  I2C0_WriteByte(I2C_MASTER_READ(adress));      // Wyslanie adresu TC74 z bitem R
  temperature = I2C0_ReadByte(I2C_NACK); // Odbiór wartosci temperatury
  I2C0_Stop();                             // Wyslanie bitu Stop
 
  I2C0_Disable();                          // Wylaczenie modulu I2C1
                                              //  i zresetowanie flag
 
  // Konwersja ‘temperature’ na wartosc ze znakiem i zwrócenie jej funkcji wywolujacej
  return (int8_t) temperature;
	
	
}