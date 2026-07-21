#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

void motor_init(void);

/* Rear Motor */
void rear_forward(uint8_t speed);
void rear_backward(uint8_t speed);
void rear_stop(void);

/* Steering Motor */
void steer_left(void);
void steer_right(void);
void steer_stop(void);

#endif