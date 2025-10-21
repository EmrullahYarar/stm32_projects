#ifndef MPU6500_H
#define MPU6500_H

#include "stm32c0xx_hal.h"

// MPU6500 status enumeration
typedef enum {
    MPU6500_OK = 0,
    MPU6500_ERROR
} MPU6500_Status;

// Data structure for sensor readings
typedef struct {
    int16_t Accel_X_raw;
    int16_t Accel_Y_raw;
    int16_t Accel_Z_raw;
    float Accel_X_g;
    float Accel_Y_g;
    float Accel_Z_g;

    int16_t Gyro_X_raw;
    int16_t Gyro_Y_raw;
    int16_t Gyro_Z_raw;
    float Gyro_X_dps;
    float Gyro_Y_dps;
    float Gyro_Z_dps;

    float Temp_C;
} MPU6500_Data;


// Function prototypes
MPU6500_Status MPU6500_Init(I2C_HandleTypeDef *hi2c);
MPU6500_Status MPU6500_Read_All(I2C_HandleTypeDef *hi2c, MPU6500_Data *data_struct);

#endif // MPU6500_H
