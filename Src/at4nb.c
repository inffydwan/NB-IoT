#include "at4nb.h"
#include "usart.h"
#include "lcd.h"
#include "string.h"

extern Uart2DmaRcvData uart2DmaRcvData;
extern AT4NB_MSG msgs[7];
MsgIndex currentIndex;

uint8_t retryTimesBack[7];

void msgInit(AT4NB_MSG *msgs)
{
	msgs[0].cmd = "AT+NRB\r\n";
	msgs[0].feedBack = "OK";
	msgs[0].retryTimes = 5;
	retryTimesBack[0] = 5;
	
	msgs[1].cmd = "AT+CSQ\r\n";
	msgs[1].feedBack = "OK";
	msgs[1].retryTimes = 7;
	retryTimesBack[1] = 7;
	
	msgs[2].cmd = "AT+CGPADDR\r\n";
	msgs[2].feedBack = "OK";
	msgs[2].retryTimes = 10;
	retryTimesBack[2] = 10;
	
	msgs[3].cmd = "AT+NSOCR=STREAM,6,10000\r\n";
	msgs[3].feedBack = "OK";
	msgs[3].retryTimes = 5;
	retryTimesBack[3] = 5;
	
	msgs[4].cmd = "AT+NSOCO=1,64.69.43.237,10244\r\n";
	msgs[4].feedBack = "OK";
	msgs[4].retryTimes = 5;
	retryTimesBack[4] = 5;
	
	msgs[5].cmd = "AT+NSOSD=1,3,AABBCC\r\n";
	msgs[5].feedBack = "OK";
	msgs[5].retryTimes = 5;
	retryTimesBack[5] = 5;
	
	msgs[6].cmd = "AT+NSOCL=1\r\n";
	msgs[6].feedBack = "OK";
	msgs[6].retryTimes = 5;
	retryTimesBack[6] = 5;
	
	currentIndex = REBOOT;
}






void nbDataProcess()
{
		if (strstr(uart2DmaRcvData.rcvBuff, msgs[currentIndex].feedBack) != NULL)  //����У���ȡ��������
		{
			if (currentIndex == GET_SIGNAL_STRENGTH)  //��Ҫ��һ��У���ź�����
			{
				uint16_t rssi;
				uint8_t *p;
				p = strstr(uart2DmaRcvData.rcvBuff, "+CSQ:");
				if (p)
				{
					sscanf(p, "+CSQ:%d", &rssi);
					
					if (rssi > 0 && rssi < 99)  //У��ɹ�
					{
						char buf[5];
						sprintf(buf, "%d", rssi);
						currentIndex++;
						Lcd_Clear(WHITE);
						Gui_DrawFont_GBK16(0, 10, BLUE, WHITE, "signal strength:");
						Gui_DrawFont_GBK16(50, 26, GREEN, WHITE, buf);
						HAL_Delay(1000);
						Lcd_Clear(WHITE);
						HAL_UART_Transmit(&huart2, msgs[currentIndex].cmd, strlen(msgs[currentIndex].cmd), 100);  
						printf("sending %s\r\n", msgs[currentIndex].cmd);
						
					}
					else goto label;

				}
			}
			else if (currentIndex == GET_IP_ADDR)  //��һ��У���ַ�Ƿ���Ч
			{
				
				uint8_t *p;
				
				p = strstr(uart2DmaRcvData.rcvBuff, "+CGPADDR:0,");  //�ҵ��׵�ַ
				if (p)  //0���ж���˵���ѻ�ȡ��ַ
				{
					uint8_t a1, a2, a3, a4;
					char buf[15];
					
					sscanf(p, "+CGPADDR:0,%c.%c.%c.%c", &a1, &a2, &a3, &a4);
					
						sprintf(buf, "%d.%d.%d.%d", a1, a2, a3, a4);
						
						Lcd_Clear(WHITE);
						Gui_DrawFont_GBK16(0, 10, BLUE, WHITE, "IP address:");
						Gui_DrawFont_GBK16(0, 26, GREEN, WHITE, buf);
						HAL_Delay(1000);
						Lcd_Clear(WHITE);
					currentIndex++;
					HAL_UART_Transmit(&huart2, msgs[currentIndex].cmd, strlen(msgs[currentIndex].cmd), 100);   //������һ��ָ��
				}
				else  goto label;
			}
			else  //У��ɹ�
			{
				msgs[currentIndex].retryTimes = retryTimesBack[currentIndex];  //��ԭ��ǰ��Ϣ���Դ���
				currentIndex++;
				HAL_UART_Transmit(&huart2, msgs[currentIndex].cmd, strlen(msgs[currentIndex].cmd), 100);  
				uart2DmaRcvData.rcvFlag = 0;
				return;
			}
		}
		else  //����У������ʧ��
		{
			label:
			if (msgs[currentIndex].retryTimes > 0)  
			{
				msgs[currentIndex].retryTimes--;
				if (currentIndex != REBOOT)  
				{
					char buf[3];
					sprintf(buf, "%d", msgs[currentIndex].retryTimes);
					Gui_DrawFont_GBK16(40, 40, BLUE, WHITE, buf);
					HAL_Delay(1000);
					HAL_UART_Transmit(&huart2, msgs[currentIndex].cmd, strlen(msgs[currentIndex].cmd), 100);
					uart2DmaRcvData.rcvFlag = 0;
					return;
				}
			}
			else   //������Դ�����������
			{
				msgs[currentIndex].retryTimes = retryTimesBack[currentIndex];
				currentIndex = REBOOT;
			}
			
		}
		uart2DmaRcvData.rcvFlag = 0;
	
	
	
	
}