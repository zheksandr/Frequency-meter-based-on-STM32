#ifndef COMMUNICATION
#define COMMUNICATION

#include "main.h"
#include "liquidcrystal_i2c.h"

uint8_t  f_lenght = 0;
uint32_t temp = 0;
extern UART_HandleTypeDef huart4;
void SendDataLCDUSB(uint32_t frequency){
	  temp = frequency;
	  f_lenght = 0;
	  HD44780_Clear();
	  HD44780_SetCursor(0,0);
	  while (temp){
		  temp /= 10;
		  f_lenght++;
	  }
	  if (f_lenght){
	  uint8_t Frequency_Buff[f_lenght];
	  unsigned char send_Buff_UART[f_lenght+5];
	  for (int i = 0; i < f_lenght; i++){
		  Frequency_Buff[i] = frequency % 10 + 48;
		  frequency /= 10;
	  }
	  uint8_t start = 0;
	  uint8_t end = f_lenght - 1;
	  uint8_t temp;
	  while (start < end) {
	      temp = Frequency_Buff[start];
	      Frequency_Buff[start] = Frequency_Buff[end];
	      Frequency_Buff[end] = temp;
          start++;
          end--;
      }
	  memcpy(send_Buff_UART,Frequency_Buff,f_lenght);
      memcpy(send_Buff_UART+f_lenght,"HZ\r\n",5);
	  for (int i = 0; i < f_lenght; i++){
		  HD44780_PrintSpecialChar(Frequency_Buff[i]);
	  }
	  HD44780_PrintStr("HZ");
	  HAL_UART_Transmit(&huart4, send_Buff_UART,(uint16_t) (f_lenght+5), HAL_MAX_DELAY);
	  }
}

#endif
