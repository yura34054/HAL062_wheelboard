#ifndef PROJECT_MODULES_LEDS_LEDS_H
#define PROJECT_MODULES_LEDS_LEDS_H

#include <stm32f4xx_hal.h>

#define LED_1		GPIO_PIN_7
#define LED_2		GPIO_PIN_8
#define LED_3       GPIO_PIN_9
#define LED_ALL		LED_1 | LED_2 | LED_3 
#define LED_PORT 	GPIOB

#define LED_TURN_ON(led)	HAL_GPIO_WritePin(LED_PORT, led, 0x01u) 
#define LED_TURN_OFF(led)	HAL_GPIO_WritePin(LED_PORT, led, 0x00u)
#define LED_TOGGLE(led)		HAL_GPIO_TogglePin(LED_PORT, led) 

#endif // PROJECT_MODULES_LEDS_LEDS_H
