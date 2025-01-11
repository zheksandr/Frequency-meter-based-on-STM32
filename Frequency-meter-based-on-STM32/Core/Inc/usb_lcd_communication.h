#ifndef COMMUNICATION
#define COMMUNICATION

#include "main.h"
#include "liquidcrystal_i2c.h"

#define ERR_MESSAGE_LENGTH   9
#define RAW_LENGTH           16
#define FREQ_NO_DIGIT        10000
#define FREQ_ONE_DIGIT       1000
#define FREQ_TWO_DIGIT       100
#define FREQ_THREE_DIGIT     10

extern UART_HandleTypeDef huart4;
void SendDataLCDUSB(uint16_t *Buffer_CCR, uint16_t *Buffer_RCR, uint8_t Flag){
	uint16_t  f_lenght    = 0;
	double frequency       = 0;
	char send_buff[RAW_LENGTH]  ={'0'};
	if(Flag){
		double sum = 0;
		for (int i = 0; i < BUFFER_SIZE-1; i++){
			if ((Buffer_RCR[i]-Buffer_RCR[i+1])<0){
				sum += (Buffer_RCR[i]+REPETITION_PERIOD-Buffer_RCR[i+1])*PERIOD+Buffer_CCR[i]-Buffer_CCR[i+1];
			}else {
				sum += (Buffer_RCR[i]-Buffer_RCR[i+1])*PERIOD+Buffer_CCR[i]-Buffer_CCR[i+1];
			}
		}
		frequency = (180e6*8)/(PRESCALER*sum);
		if((uint32_t)frequency/FREQ_NO_DIGIT){
			uint32_t   temp = 0;
			f_lenght = 0;
			temp = frequency;
			while (temp){
				temp /= 10;
				f_lenght++;
		    }
			sprintf(send_buff, "%d",(uint32_t)frequency);
		}else if ((uint32_t)frequency/FREQ_ONE_DIGIT){
			sprintf(send_buff, "%.1f",frequency);
			f_lenght = 6;
		}else if((uint32_t)frequency/FREQ_TWO_DIGIT){
			sprintf(send_buff, "%.2f",frequency);
			f_lenght = 6;
		}else if((uint32_t)frequency/FREQ_THREE_DIGIT){
			sprintf(send_buff, "%.3f",frequency);
			f_lenght = 6;
		}else{
			sprintf(send_buff, "%.4f",frequency);
			f_lenght = 6;
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
	  if(Flag){
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
