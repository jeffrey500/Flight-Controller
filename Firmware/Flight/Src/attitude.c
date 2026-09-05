#include "attitude.h"
#include "imu.h"
#include <math.h>

#define RAD_TO_DEG 57.29577951f

// Create empty Attitude_data struct (all 0)
void Attitude_Init(Attitude_data *attitude){
    attitude->roll = 0;
    attitude->pitch = 0;
}

// Update Attitude_data struct with imu data and complementary filter
void Update_Attitude(float alpha, float DT, IMU_data *imu, Attitude_data *attitude){
    float roll_accel =  atan2f(-imu->accel_y, -imu->accel_z) * RAD_TO_DEG;
    // Pitch requires the hypotenuse of Y and Z to decouple the axes
    float accel_pitch = atan2f(-imu->accel_x, sqrtf((imu->accel_y * imu->accel_y) + (imu->accel_z * imu->accel_z))) * RAD_TO_DEG;

    // Complementary filter
    attitude->roll = alpha * (attitude->roll + imu->gyro_x * DT) + (1 - alpha) * roll_accel;
    attitude->pitch = alpha * (attitude->pitch + imu->gyro_y * DT) + (1 - alpha) * accel_pitch;
}