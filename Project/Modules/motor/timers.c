/*
 ******************************************************************************
 * @file           : timers.c
 * @author         : D. Mucha
 * @brief          : Timers configuration
 ******************************************************************************
 */

#include <stdint.h>
#include "timers.h"
#include "motor.h"
#include "can/can.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim7;

TIM_HandleTypeDef *tim_encoder = &htim1; //encoder - TIM1
TIM_HandleTypeDef *tim_pwm = &htim5; // PWM - TIM5
TIM_HandleTypeDef *tim_speed = &htim7; // measuring speed - TIM7

uint8_t message_recived = 0;
static uint8_t timer_counter = 0;
static int32_t g_encoderTick = 0;

static void TIM_speedPeriodElapsedCallback(TIM_HandleTypeDef *htim);

void TIM_init(void) {
	HAL_TIM_RegisterCallback(tim_speed, HAL_TIM_PERIOD_ELAPSED_CB_ID, TIM_speedPeriodElapsedCallback);
}

void TIM_speedPeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	(void) htim;

	g_encoderTick = (int32_t) htim1.Instance->CNT;

	// resetting counter for next interrupt
	__HAL_TIM_SET_COUNTER(&htim1, 0);

	updatePID(g_encoderTick);

	// every 10 callbacks
	if ((timer_counter % 10) == 0) {
		uint32_t speed = *(uint32_t *) &current_speed;
		Command command = {0};

		command.ID = NORMAL_ID;
		command.payload[0] = (uint8_t) (speed & 0xFF000000) >> 24;
		command.payload[1] = (uint8_t) (speed & 0x00FF0000) >> 16;
		command.payload[2] = (uint8_t) (speed & 0x0000FF00) >> 8;
		command.payload[3] = (uint8_t) (speed & 0x000000FF);

		CAN_transmit(&command);
	}

	// every 30 callbacks
	if ((timer_counter % 30) == 0) {
		if (!message_recived) {
			target_speed = 0;
			// coms loss
			// panic!!!
		}
		message_recived = 1;
	}

	timer_counter++;

}

// TODO add error callback registration 
void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim) {
	UNUSED(htim);
}

