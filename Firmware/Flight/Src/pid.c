#include "pid.h"
#include "stm32h743xx.h"

// Initalize PID struct
void PID_Init(PID_Controller *pid, float dt, float alpha, float k, float i, float d, float m_integral, float m_output){
    pid->DT = dt;

    pid->Kk = k;
    pid->Ki = i;
    pid->Kd = d;

    pid->integral = 0;
    pid->prev_actual = 0;
    pid->prev_filtered_d = 0;

    pid->lpf_alpha = alpha;

    pid->max_integral = m_integral;
    pid->max_output = m_output;
}

// Update PID values as well as return output
float PID_Generate(PID_Controller *pid, float setpoint, float acutal){
    float current_error = setpoint - acutal;
    
    // Porportional
    float P = pid->Kk * current_error;

    // Integral
    float I = pid->integral + pid->DT * current_error * pid->Ki * pid->Kk;

    // Clamp integral
    if (I > pid->max_integral){
        I = pid->max_integral;
    } else if (I < -pid->max_integral) {
        I = -pid->max_integral;
    }

    // Raw Derivative
    float raw_d = (acutal - pid->prev_actual)/pid->DT;

    // Apply LPF
    float filtered_d = pid->lpf_alpha * raw_d + (1.0f - pid->lpf_alpha) * pid->prev_filtered_d;

    // Derivative
    float D = filtered_d* pid->Kd * pid->Kk;

    // Calculate and clamp output
    float output = P + I - D;

    if (output > pid->max_output){
        output = pid->max_output;
    } else if (output < -pid->max_output){
        output = -pid->max_output;
    }

    // Update previous state variables
    pid->integral = I;
    pid->prev_actual = acutal;
    pid->prev_filtered_d = filtered_d;

    return output;
}