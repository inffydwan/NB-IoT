#ifndef __SENSOR_H
#define __SENSOR_H



#include "stm32f0xx_hal.h"

#define  GAS  1
#define   ULFRARED  0
#define FIRE  0
#define RELAY 0

#define ADC_INTEVAL  5*1000


void sensoring();


#endif