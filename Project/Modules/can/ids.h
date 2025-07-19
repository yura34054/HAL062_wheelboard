#ifndef PROJECT_MODULES_CAN_IDS_H
#define PROJECT_MODULES_CAN_IDS_H

#define WHEEL_ID 0 // internal use only, reffer to COMS.md

#define PANIC_ID	WHEEL_ID + 1
#define PARENT_ID	WHEEL_ID + 128
#define NORMAL_ID	PANIC_ID + 133

#if WHEEL_ID == 0 || WHEEL_ID == 2 || WHEEL_ID == 4 
#define SIDE_MULT -1 // left side has to be in other direction
#else
#define SIDE_MULT 1
#endif

#endif // PROJECT_MODULES_CAN_IDS_H
