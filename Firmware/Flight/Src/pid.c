#include "pid.h"

// Initalize PID struct
void PID_Init(PID_Controller *pid, float dt, float p, float i, float d, float m_integral, float m_output){
    pid->DT = dt;

    pid->Kp = p;
    pid->Ki = i;
    pid->Kd = d;

    pid->integral = 0;
    pid->prev_error = 0;

    pid->max_integral = m_integral;
    pid->max_output = m_output;
}

// Update PID values as well as return output
float PID_Generate(PID_Controller *pid, float setpoint, float acutal) {
    float current_error = setpoint - acutal;
    
    // Porportional
    float P = pid->Kp * current_error;

    // Integral
    float I = pid->integral + pid->DT * current_error * pid->Ki;

    // Clamp integral
    if (I > pid->max_integral){
        I = pid->max_integral;
    } else if (I < -pid->max_integral) {
        I = -pid->max_integral;
    }
    pid->integral = I;

    // Derivative
    float D = (current_error - pid->prev_error)/pid->DT * pid->Kd;

    // Calculate and clamp output
    float output = P + I + D;

    if (output > pid->max_output){
        output = pid->max_output;
    } else if (output < -pid->max_output){
        output = -pid->max_output;
    }

    pid->prev_error = current_error;

    return output;
}