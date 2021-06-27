#ifndef __AT4NB_H
#define __AT4NB_H


#include "stm32f0xx_hal.h"

typedef struct {
	uint8_t* cmd;
	uint8_t retryTimes;
	uint8_t* feedBack;
} AT4NB_MSG;


typedef enum {
	REBOOT,
	GET_SIGNAL_STRENGTH,
	GET_IP_ADDR,
	CREATE_TCP_SOCKET,
	CONNECT_SOCKET,
	SEND_MSG,
	GET_MSG,
} MsgIndex;


void msgInit(AT4NB_MSG *msgs);
char nbConnect(AT4NB_MSG *msgs);

#endif