#ifndef IMU_H
#define IMU_H

#include <stdbool.h>

typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
} IMU_data;

bool IMU_Init(void);
void IMU_Read(IMU_data *data);

#endif