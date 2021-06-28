#include "sensor.h"
#include "at4nb.h"
#include "usart.h"
#include "string.h"
#include "lcd.h"
#include "adc.h"

uint8_t  ulfraredTriggered = 0, ulfraedUntriggered = 0;
extern MsgIndex currentIndex;
extern AT4NB_MSG msgs[];
extern Uart2DmaRcvData uart2DmaRcvData;
uint32_t AD_Time = 0;
uint16_t debug1;


void sensoring()
{
	while (1)
	{
		#if  ULFRARED
		if (ulfraredTriggered)
		{
			ulfraredTriggered = 0;
			msgs[currentIndex].cmd = "AT+NSOSD=1,3,AA0101\r\n";
			return;
		}
		else if (ulfraedUntriggered)
		{
			ulfraedUntriggered = 0;
			msgs[currentIndex].cmd = "AT+NSOSD=1,3,AA0100\r\n";
			return;
			
		}
		#elif  GAS
		if (HAL_GetTick() - AD_Time > ADC_INTEVAL)
		{
			HAL_ADC_Start(&hadc);
			HAL_ADC_PollForConversion(&hadc, 100);
			if (HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc), HAL_ADC_STATE_REG_EOC))
			{
				uint16_t adcValue;
				adcValue = HAL_ADC_GetValue(&hadc);
				AD_Time = HAL_GetTick();

				debug1 = adcValue;
				
				char str[80];
				sprintf(str, "AT+NSOSD=1,4,AA03%02X%02X\r\n", (uint8_t)(adcValue >> 8), (uint8_t)adcValue);
				memcpy(msgs[currentIndex].cmd, str, sizeof str);
				printf(msgs[currentIndex].cmd);
				
				return;
			}
		}
		#elif  FIRE
				if (ulfraredTriggered)
		{
			ulfraredTriggered = 0;
			msgs[currentIndex].cmd = "AT+NSOSD=1,3,AA0101\r\n";
			return;
		}
		else if (ulfraedUntriggered)
		{
			ulfraedUntriggered = 0;
			msgs[currentIndex].cmd = "AT+NSOSD=1,3,AA0100\r\n";
			return;
			
		}
		#elif  RELAY
		 if (uart2DmaRcvData.rcvFlag)  //接受到服务端的指令
		{
			uint8_t *p;
				
				p = strstr(uart2DmaRcvData.rcvBuff, "+NSONMI:1,2");  //找到首地址
				if (p)  //0后有逗号说明已获取地址
				{
					Lcd_Clear(WHITE);
					currentIndex++;
					return;
				}
			uart2DmaRcvData.rcvFlag = 0;
		}
		#endif
	}
	
	
}

