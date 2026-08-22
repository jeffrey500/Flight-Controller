#ifndef PID_H
#define PID_H

typedef struct{
    // Change in time per execution
    float DT;

    // Tuning Constants
    float Kk;
    float Ki;
    float Kd;

    // Previous state memories
    float integral;
    float prev_actual;
    float prev_filtered_d;

    // LFP filter values
    float lpf_alpha;

    // Max values
    float max_integral;
    float max_output;

} PID_Controller;

void PID_Init(PID_Controller *pid, float dt, float alpha, float k, float i, float d, float m_integral, float m_output);
float PID_Generate(PID_Controller *pid, float setpoint, float acutal);

#endif