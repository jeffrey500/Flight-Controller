#ifndef IMU_H
#define IMU_H

#include 

typedef struct{
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
} IMU_data;

bool IMU_init(void);
void IMU_read(IMU_data *data);

#endif