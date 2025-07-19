#ifndef PROJECT_MODULES_CAN_CAN_H
#define PROJECT_MODULES_CAN_CAN_H

#include "data.h"
#include "ids.h"

void CAN_init(void);
void CAN_transmit(Command *command);

#endif // PROJECT_MODULES_CAN_CAN_H
