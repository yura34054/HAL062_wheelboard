
#ifndef PROJECT_MODULES_CAN_DATA_H
#define PROJECT_MODULES_CAN_DATA_H

#include "stm32f4xx_hal.h"

typedef struct Command
{
	uint8_t ID;
	uint8_t payload[8];
} Command;

#endif // PROJECT_MODULES_CAN_DATA_H
