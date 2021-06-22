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




