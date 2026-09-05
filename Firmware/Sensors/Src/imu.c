#include "main.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_def.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"
#include <stdint.h>
#include "imu.h"
#include "bmi270_firmware.h"

// SPI1 handle
extern SPI_HandleTypeDef hspi1;
#define IMU_SPI &hspi1
#define CS_PORT SPI1_CS_GPIO_Port
#define CS_PIN  SPI1_CS_Pin

// Define BMI270 Registers https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmi270-ds000.pdf
#define REG_CHIP_ID       0x00
#define REG_DATA_8        0x0C // Start of Accel/Gyro data
#define REG_PWR_CONF      0x7C
#define REG_PWR_CTRL      0x7D
#define REG_ACC_CONF      0x40
#define REG_GYR_CONF      0x42
#define REG_INIT_CTRL     0x59
#define REG_INIT_DATA     0x5E
#define REG_INTERNAL_STAT 0x21
#define REG_RESET 0x7E

// Scaling (For ±2000 dps and ±8g)
#define ACCEL_SCALE (1.0f / 4096.0f)
#define GYRO_SCALE  (1.0f / 16.4f)  

// Read registers
static uint8_t IMU_ReadReg(uint8_t reg) {
    // reg | 0x80 forces the first bit to be 1 as 0x80 = 10000000
    // txData[1] is the delay 8 pulses txData[2] is the return 8 pulses
    uint8_t txData[3] = {reg | 0x80, 0x00, 0x00}; 
    uint8_t rxData[3] = {0};

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(IMU_SPI, txData, rxData, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);

    return rxData[2];
}

// Write to registers
static void IMU_WriteReg(uint8_t reg, uint8_t value) {
    // reg & 0x7F forces the first bit to be 0 as 0x7F = 11111111
    uint8_t txData[2] = {reg & 0x7F, value};

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(IMU_SPI, txData, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

static bool IMU_WriteFirmware(void){

    // Prepare BMI270 for firmware
    IMU_WriteReg(REG_INIT_CTRL, 0x00);
    HAL_Delay(1);

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    
    // Send bmi270 firmware
    HAL_SPI_Transmit(IMU_SPI, bmi270_config_file, sizeof(bmi270_config_file)/sizeof(uint8_t), HAL_MAX_DELAY);

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);

    // MBI270 firmware complete
    IMU_WriteReg(REG_INIT_CTRL, 0x01);
    HAL_Delay(50);

    // Verify firmware
    uint8_t status = IMU_ReadReg(REG_INTERNAL_STAT);
    if (status != 0x01) {
        return false; // If status is not 1, the firmware failed to load!
    }
    return true;
}

// Switch from I2C to SPI
void IMU_Switch_SPI(void){
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
}

// Initalize IMU
bool IMU_Init(void){
    // Delay initalization for 100ms such that IMU has time to boot up HAL_Delay ok becuase ouside of main loop
    HAL_Delay(100);

    IMU_Switch_SPI();

    // Reset Chip
    IMU_WriteReg(REG_RESET, 0xB6);
    HAL_Delay(100);

    IMU_Switch_SPI();

    // Check Chip ID 0x24 from datasheet
    uint8_t chipID = IMU_ReadReg(REG_CHIP_ID);
    if (chipID != 0x24) return false;

    // Disable Power Save
    IMU_WriteReg(REG_PWR_CONF, 0x00);
    HAL_Delay(20);

    if (IMU_WriteFirmware() == false) return false;

    // Enable Accel and Gyro
    IMU_WriteReg(REG_PWR_CTRL, 0x0E); 
    
    // Set ranges (±8g, ±2000 dps)
    IMU_WriteReg(REG_ACC_CONF, 0xA8); 
    IMU_WriteReg(REG_GYR_CONF, 0xA9); 

    return true;
}

// Read data from IMU
void IMU_Read(IMU_data *data){
    // Transmit format: add, dummy, 12 bytes of clock
    uint8_t txData[14] = {0};
    uint8_t rxData[14] = {0};

    txData[0] = REG_DATA_8 | 0x80;

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(IMU_SPI, txData, rxData, 14, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);

    // Bit shift such that you can make 16 bit raw values with 2 bytes
    int16_t ax_raw = (rxData[3] << 8) | rxData[2];
    int16_t ay_raw = (rxData[5] << 8) | rxData[4];
    int16_t az_raw = (rxData[7] << 8) | rxData[6];

    int16_t gx_raw = (rxData[9] << 8) | rxData[8];
    int16_t gy_raw = (rxData[11] << 8) | rxData[10];
    int16_t gz_raw = (rxData[13] << 8) | rxData[12];

    // Assign values in the struct to raw data
    data->accel_x = ax_raw*ACCEL_SCALE;
    data->accel_y = ay_raw*ACCEL_SCALE;
    data->accel_z = az_raw*ACCEL_SCALE;

    data->gyro_x = gx_raw*GYRO_SCALE;
    data->gyro_y = -gy_raw*GYRO_SCALE;
    data->gyro_z = gz_raw*GYRO_SCALE;
}