#include "sensor.h"
#include "at4nb.h"

uint8_t  ulfraredTriggered = 0, ulfraedUntriggered = 0;
extern MsgIndex currentIndex;
extern AT4NB_MSG msgs[7];

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
		
	}
	
	
}