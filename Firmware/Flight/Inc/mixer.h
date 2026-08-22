// We want to send D-Shot

#ifndef MIXER_H
#define MIXER_H

#include <stdint.h>

extern uint16_t motor_outputs[4];

float channel_to_angle(uint16_t throttle, float max_angle);
float channel_to_angular_rate(uint16_t throttle, float max_rate);
void Mixer(uint16_t throttle, float roll_pid, float pitch_pid, float yaw_pid, uint8_t percent_power);

#endif