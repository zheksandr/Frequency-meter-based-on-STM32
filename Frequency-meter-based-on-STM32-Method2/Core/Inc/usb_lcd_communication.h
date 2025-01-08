#ifndef COMMUNICATION
#define COMMUNICATION

#include "main.h"
#include "liquidcrystal_i2c.h"

#define ERR_MESSAGE_LENGTH   9
#define RAW_LENGTH           16

extern UART_HandleTypeDef huart4;
void SendDataLCDUSB(uint32_t frequency){
	  uint32_t temp = frequency;
	  uint16_t  f_lenght    = 0;
	  char send_buff[RAW_LENGTH]  ={'0'};
	  if(frequency){
		  sprintf(send_buff, "%d",(uint32_t)frequency);
		  while (temp){
			  temp /= 10;
			  f_lenght++;
		  }
	  }else{
		  sprintf(send_buff, "%s","NOT_READY");
		  f_lenght = ERR_MESSAGE_LENGTH;
	  }
	  HD44780_Clear();
	  HD44780_SetCursor(0,0);
	  for (int i = 0; i < f_lenght; i++){
		  HD44780_PrintSpecialChar(send_buff[i]);
	  }
	  if(frequency){
		  HD44780_PrintStr("HZ");
	  	  memcpy(send_buff+f_lenght,"HZ\r\n",5);
	  	  f_lenght+=5;
	  }else{
		  memcpy(send_buff+f_lenght,"\r\n",3);
		  f_lenght+=3;
	  }
	  HAL_UART_Transmit(&huart4, send_buff,f_lenght, HAL_MAX_DELAY);

}

#endif
