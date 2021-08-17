#include "at4nb.h"
#include "usart.h"
#include "lcd.h"
#include "string.h"

extern Uart2DmaRcvData uart2DmaRcvData;
AT4NB_MSG msgs[7] = {
		{"AT+NRB\r\n","OK",5},
	{"AT+CSQ\r\n","OK",15},
	{"AT+CGPADDR\r\n","OK",20},
	{"AT+NSOCR=STREAM,6,10000\r\n","OK",5},
	{"AT+NSOCO=1,64.69.43.237,10244\r\n","OK",5},
	{"AT+NNMI=1\r\n","OK",5},
	{"AT+NSORF=1,5\r\n","1,64.69.43.237,10244,2,BB",5},
};
MsgIndex currentIndex = REBOOT;

uint8_t retryTimesBack[7] = {5, 15, 30, 5, 5, 5, 5};






