/*
 ******************************************************************************
 * @file           : timers.h
 * @author         : D. Mucha
 * @brief          : Timers configuration
 ******************************************************************************
 */

#ifndef MOTORS_TIMERS_H_
#define MOTORS_TIMERS_H_

#include <stm32f4xx_hal.h>

// TODO change once the new board is finished
// [Timer - Encoder - Pin] mapping for our custom motor board
// TIM1_CH1 - ENK1_A - PA8
// TIM1_CH2 - ENK1_B - PA9

// [Timer - PWM - Pin] PWM generation for our custom motor board
// TIM5_CH1 - PWM1 - PA0

extern uint8_t message_recived;
extern TIM_HandleTypeDef *tim_encoder; //encoder - TIM1
extern TIM_HandleTypeDef *tim_pwm; // PWM - TIM5
extern TIM_HandleTypeDef *tim_speed; // measuring speed - TIM7

void TIM_init(void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* MOTORS_TIMERS_H_ */
