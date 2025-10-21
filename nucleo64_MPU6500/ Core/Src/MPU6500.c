#include <MPU6500.h>

// Default I2C address for MPU6500
#define MPU6500_I2C_ADDR 0x68

// MPU6500 Registers
#define MPU6500_REG_PWR_MGMT_1    0x6B
#define MPU6500_REG_WHO_AM_I      0x75
#define MPU6500_REG_ACCEL_XOUT_H  0x3B
#define MPU6500_REG_GYRO_XOUT_H   0x43
#define MPU6500_REG_TEMP_OUT_H    0x41
#define MPU6500_REG_SMPLRT_DIV    0x19
#define MPU6500_REG_CONFIG        0x1A
#define MPU6500_REG_GYRO_CONFIG   0x1B
#define MPU6500_REG_ACCEL_CONFIG  0x1C
#define MPU6500_REG_ACCEL_CONFIG2 0x1D

// Sensitivity Scale Factors
#define ACCEL_FS_2G  16384.0
#define GYRO_FS_250DPS 131.0


/**
 * @brief  Initializes the MPU6500 sensor.
 * @param  hi2c: Pointer to an I2C_HandleTypeDef structure that contains the configuration
 * information for the specified I2C.
 * @retval MPU6500 status (MPU6500_OK or MPU6500_ERROR).
 */
MPU6500_Status MPU6500_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t check, data;

    // Check device ID
    HAL_I2C_Mem_Read(hi2c, (MPU6500_I2C_ADDR << 1), MPU6500_REG_WHO_AM_I, 1, &check, 1, 1000);
    if (check != 0x70) { // MPU-6500 WHO_AM_I should be 0x70
        return MPU6500_ERROR;
    }

    // Wake up sensor
    data = 0x00;
    HAL_I2C_Mem_Write(hi2c, (MPU6500_I2C_ADDR << 1), MPU6500_REG_PWR_MGMT_1, 1, &data, 1, 1000);

    // Set sample rate to 1KHz
    data = 0x07;
    HAL_I2C_Mem_Write(hi2c, (MPU6500_I2C_ADDR << 1), MPU6500_REG_SMPLRT_DIV, 1, &data, 1, 1000);

    // Set accelerometer configuration to +/- 2g
    data = 0x00;
    HAL_I2C_Mem_Write(hi2c, (MPU6500_I2C_ADDR << 1), MPU6500_REG_ACCEL_CONFIG, 1, &data, 1, 1000);

    // Set gyroscope configuration to 250 dps
    data = 0x00;
    HAL_I2C_Mem_Write(hi2c, (MPU6500_I2C_ADDR << 1), MPU6500_REG_GYRO_CONFIG, 1, &data, 1, 1000);

    return MPU6500_OK;
}

/**
 * @brief  Reads all sensor data from the MPU6500.
 * @param  hi2c: Pointer to an I2C_HandleTypeDef structure.
 * @param  data_struct: Pointer to a MPU6500_Data structure to store the data.
 * @retval MPU6500 status (MPU6500_OK or MPU6500_ERROR).
 */
MPU6500_Status MPU6500_Read_All(I2C_HandleTypeDef *hi2c, MPU6500_Data *data_struct) {
    uint8_t rec_data[14];
    int16_t temp_raw;

    // Read 14 bytes of data starting from ACCEL_XOUT_H register
    if (HAL_I2C_Mem_Read(hi2c, (MPU6500_I2C_ADDR << 1), MPU6500_REG_ACCEL_XOUT_H, 1, rec_data, 14, 1000) != HAL_OK) {
        return MPU6500_ERROR;
    }

    // Process Accelerometer data
    data_struct->Accel_X_raw = (int16_t)(rec_data[0] << 8 | rec_data[1]);
    data_struct->Accel_Y_raw = (int16_t)(rec_data[2] << 8 | rec_data[3]);
    data_struct->Accel_Z_raw = (int16_t)(rec_data[4] << 8 | rec_data[5]);

    // Process Temperature data
    temp_raw = (int16_t)(rec_data[6] << 8 | rec_data[7]);

    // Process Gyroscope data
    data_struct->Gyro_X_raw = (int16_t)(rec_data[8] << 8 | rec_data[9]);
    data_struct->Gyro_Y_raw = (int16_t)(rec_data[10] << 8 | rec_data[11]);
    data_struct->Gyro_Z_raw = (int16_t)(rec_data[12] << 8 | rec_data[13]);

    // Convert raw values to meaningful data
    data_struct->Accel_X_g = data_struct->Accel_X_raw / ACCEL_FS_2G;
    data_struct->Accel_Y_g = data_struct->Accel_Y_raw / ACCEL_FS_2G;
    data_struct->Accel_Z_g = data_struct->Accel_Z_raw / ACCEL_FS_2G;

    data_struct->Gyro_X_dps = data_struct->Gyro_X_raw / GYRO_FS_250DPS;
    data_struct->Gyro_Y_dps = data_struct->Gyro_Y_raw / GYRO_FS_250DPS;
    data_struct->Gyro_Z_dps = data_struct->Gyro_Z_raw / GYRO_FS_250DPS;

    data_struct->Temp_C = (temp_raw / 340.0) + 36.53;

    return MPU6500_OK;
}
