#ifndef PID_H
#define PID_H

typedef struct{
    // Change in time per execution
    float DT;

    // Tuning Constants
    float Kp;
    float Ki;
    float Kd;

    // Previous state memories
    float integral;
    float prev_error;

    // Max values
    float max_integral;
    float max_output;

} PID_Controller;

void PID_Init(PID_Controller *pid, float dt, float p, float i, float d, float m_integral, float m_output);
float PID_Generate(PID_Controller *pid, float setpoint, float acutal);

#endif