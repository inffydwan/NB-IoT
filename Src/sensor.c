#include "sensor.h"
#include "at4nb.h"
#include "usart.h"
#include "string.h"
#include "lcd.h"

uint8_t  ulfraredTriggered = 0, ulfraedUntriggered = 0;
extern MsgIndex currentIndex;
extern AT4NB_MSG msgs[7];
extern Uart2DmaRcvData uart2DmaRcvData;

void sensoring()
{
	while (1)
	{
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
		else if (uart2DmaRcvData.rcvFlag)  //���ܵ�����˵�ָ��
		{
			uint8_t *p;
				
				p = strstr(uart2DmaRcvData.rcvBuff, "+NSONMI:1,2");  //�ҵ��׵�ַ
				if (p)  //0���ж���˵���ѻ�ȡ��ַ
				{
					Lcd_Clear(WHITE);
					currentIndex++;
					return;
				}
			uart2DmaRcvData.rcvFlag = 0;
		}
		
	}
	
	
}

