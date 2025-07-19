/*
 ******************************************************************************
 * @file           : control_consts.h
 * @author         : D. Mucha
 * @brief          : Provides constants to general control (regulators, filters)
 ******************************************************************************
 */

#ifndef MODULES_MOTORS_PID_CONSTS_H
#define MODULES_MOTORS_PID_CONSTS_H

// alpha parameter of LPF filter (alpha = dt / (RC + dt))
// const float LPF_ALPHA = 1;

#define TP (float) 0.1 // 100 ms = 8 Mhz clock / (80 kHz tim7 prescaler * 10 tim7 period) -> encoders measure

#define PID_K (float) 3.5
#define PID_TD (float) 0.0
#define PID_TI (float) 1.0

// TODO find the formula for this and paste it in without all this macro math
#define R0 (PID_K * (1 + TP / (2 * PID_TI) + PID_TD / TP))
#define R1 (PID_K * (TP / (2 * PID_TI) - 2 * PID_TD / TP - 1))
#define R2 (PID_K * PID_TD / TP)

#endif /* MODULES_MOTORS_PID_CONSTS_H */
