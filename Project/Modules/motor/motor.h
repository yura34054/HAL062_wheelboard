#ifndef MODULES_MOTORS_MOTOR_H
#define MODULES_MOTORS_MOTOR_H

#include "stm32f4xx_hal.h"

#define PWM_CHANNEL TIM_CHANNEL_1
#define ENC_MAX_PULSE_VALUE 65535
#define ENC_PULSE_PER_ROTATION 1024
#define VELOCITY_CLOCK_TIME 100 //in ms

extern int32_t target_speed; 
extern float current_speed;

// @brief High-level function that should be placed in a continuous loop to control the speed of motors
// @param params array of singleMotorParam structures for 3 motors from one side of the rover
// @param array_length length of params array (is anticipated to be always 3 - 3 motors connected to one board)
// @returns true if iteration succeeded, false otherwise
//
// TODO update desctiption
void updatePID(int32_t encoder_ticks);

// @brief required motor calibration before first usage
void motor_calibration(void);

#endif /* MODULES_MOTORS_MOTOR_H */
