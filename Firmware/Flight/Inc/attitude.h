#ifndef ATTITUDE_H
#define ATTITUDE_H

#include "imu.h"

typedef struct{
    float roll;
    float pitch;
} Attitude_data;

void Attitude_Init(Attitude_data *attitude);
void Update_Attitude(float alpha, float DT, IMU_data *imu, Attitude_data *attitude);

#endif