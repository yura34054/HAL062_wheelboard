#include "motor.h"
#include "pid_consts.h"
#include "timers.h"

extern TIM_HandleTypeDef *tim_pwm;

// variable stores output of PID controller for motor1
static float PIDOut = 0.0f;
static float error[3] = {0.0f};

float current_speed = 0.0f;
int32_t target_speed = 0;

// min PWM - 100k*5% = 5_000
// mid PWM - 100k*7.5% = 7_500
// max PWM - 100k*10% = 10_000
static void PWM_SetDutyCycle(uint16_t duty) {
	__HAL_TIM_SET_COMPARE(tim_pwm, PWM_CHANNEL, duty);
}

void updatePID(int32_t encoder_ticks) {
	if (encoder_ticks > ENC_MAX_PULSE_VALUE / 2.0)
		encoder_ticks = encoder_ticks - ENC_MAX_PULSE_VALUE;

	current_speed = (float)encoder_ticks * 1000.0f/((float)VELOCITY_CLOCK_TIME * (float)ENC_PULSE_PER_ROTATION);
	current_speed *= 300; // 100 is 1 rotate per sec

	error[2] = error[1];
	error[1] = error[0];
	error[0] = (float) target_speed - current_speed;

	PIDOut = R2*error[2] + R1*error[1] + R0*error[0] + PIDOut;

	if (target_speed == 0) {PIDOut = 0;}
	if (PIDOut > 2500) {PIDOut = 2500;}
	if (PIDOut < -2500) {PIDOut = -2500;}

	PWM_SetDutyCycle((uint16_t) (7500 + PIDOut));
}

// unused?
void motor_calibration(void) {
	PWM_SetDutyCycle(10000);
	HAL_Delay(5000);
	PWM_SetDutyCycle(5000);
	HAL_Delay(5000);
	PWM_SetDutyCycle(7500);
	HAL_Delay(5000);
}
