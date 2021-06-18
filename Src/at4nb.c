#include "at4nb.h"
#include "usart.h"
#include "lcd.h"
#include "string.h"

extern Uart2DmaRcvData uart2DmaRcvData;
extern AT4NB_MSG msgs[7];
MsgIndex currentIndex;

void msgInit(AT4NB_MSG *msgs)
{
	msgs[0].cmd = "AT+NRB\r\n";
	msgs[0].feedBack = "OK";
	msgs[0].retryTimes = 5;
	
	msgs[1].cmd = "AT+CSQ\r\n";
	msgs[1].feedBack = "OK";
	msgs[1].retryTimes = 5;
	
	msgs[2].cmd = "AT+CGPADDR\r\n";
	msgs[2].feedBack = "OK";
	msgs[2].retryTimes = 5;
	
	msgs[3].cmd = "AT+NSOCR=STREAM,6,10000\r\n";
	msgs[3].feedBack = "OK";
	msgs[3].retryTimes = 5;
	
	msgs[4].cmd = "AT+NSOCO=1,64.69.43.237,10244\r\n";
	msgs[4].feedBack = "OK";
	msgs[4].retryTimes = 5;
	
	msgs[5].cmd = "AT+NSOSD=1,3,AABBCC\r\n";
	msgs[5].feedBack = "OK";
	msgs[5].retryTimes = 5;
	
	msgs[6].cmd = "AT+NSOCL=1\r\n";
	msgs[6].feedBack = "OK";
	msgs[6].retryTimes = 5;
	
	currentIndex = REBOOT;
}



void sendMsg(MsgIndex index)
{
	memset(uart2DmaRcvData.rcvBuff, 0, uart2RcvBuffSize);
	HAL_UART_Transmit(&huart2, msgs[index].cmd, strlen(msgs[index].cmd), 100);
	HAL_UART_Transmit(&huart1, msgs[index].cmd, strlen(msgs[index].cmd), 100);
	currentIndex = index;
	
}


void rcvMsg()
{
	if (uart2DmaRcvData.rcvFlag)
	{
		if (strstr(uart2DmaRcvData.rcvBuff, msgs[currentIndex].feedBack) != NULL)
		{
			
			
			
		}
		else
		{
			
			
		}
		
	}
	
	uart2DmaRcvData.rcvFlag = 0;
	
}


char nbConnect(AT4NB_MSG *msgs)
{
	//上电NB模块启动
	while (uart2DmaRcvData.rcvFlag == 0)
	{
		Gui_DrawFont_GBK16(10, 10, BLUE, WHITE, "booting...");
		
	}
	printf("received message: %s from NB module", uart2DmaRcvData.rcvBuff);
	 Lcd_Clear(WHITE);
	
	
}