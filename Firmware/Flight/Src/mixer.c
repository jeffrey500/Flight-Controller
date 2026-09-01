// We want to send D-Shot
// CRSF Channel values range from 172 to 1811
// D-shot has range 48 to 2047
// Motor positions A: front right B: back right C: back left D: front left https://ardupilot.org/copter/docs/connect-escs-and-motors.html

#include "mixer.h"
#include <stdint.h>

// 0:A 1:B 2:C 3:D
uint16_t motor_outputs[4] = {0};

static uint16_t verify_dshot(float value){
    if (value > 2047) return 2047;
    if (value < 48) return 48;
    return (uint16_t)value;
}

float channel_to_angle(uint16_t throttle, float max_angle){
    return (float)(throttle - 992.0f) * (max_angle / (820.0f));
}

float channel_to_angular_rate(uint16_t throttle, float max_rate){
    return (float)(throttle - 992.0f) * (max_rate / (820.0f));
}

void Mixer(uint16_t throttle, float roll_pid, float pitch_pid, float yaw_pid, uint8_t percent_power){
    // Turn off motor if throttle close to zero
    if (throttle < 250){
        for (int i = 0; i < 4; i++){
            motor_outputs[i] = 0;
        }
        return;
    }

    // Map throttle (channel data) to dshot
    float dshot_throttle = (float)(throttle - 172.0f) * 1.22f * (float)percent_power * 0.01f + 48.0f;
    
    // Mix the output to each motor
    float mA = dshot_throttle - roll_pid - pitch_pid + yaw_pid;

    float mB = dshot_throttle - roll_pid + pitch_pid - yaw_pid;

    float mC = dshot_throttle + roll_pid + pitch_pid + yaw_pid;

    float mD = dshot_throttle + roll_pid - pitch_pid - yaw_pid;

    // Verify and output
    motor_outputs[0] = verify_dshot(mA);
    motor_outputs[1] = verify_dshot(mB);
    motor_outputs[2] = verify_dshot(mC);
    motor_outputs[3] = verify_dshot(mD);
}