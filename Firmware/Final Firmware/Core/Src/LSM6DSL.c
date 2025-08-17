/*
 * LSM6DSL.c
 *
 *  Created on: May 10, 2025
 *      Author: 36dhe
 */

#include "LSM6DSL.h"
#include "stm32wbxx_hal.h"
#include <stdbool.h>
#include <stdint.h>

// I2C handle
I2C_HandleTypeDef *hi2c_lsm6dsl;

// ========== Register Addresses ==========
const uint8_t LSM6DSL_REGISTER_ADDRESSES[] = {
    LSM6DSL_REG_CTRL1_XL,
    LSM6DSL_REG_CTRL2_G,
    LSM6DSL_REG_CTRL3_C,
    LSM6DSL_REG_CTRL4_C,
    LSM6DSL_REG_CTRL5_C,
    LSM6DSL_REG_CTRL6_C,
    LSM6DSL_REG_CTRL7_G,
    LSM6DSL_REG_CTRL8_XL,
    LSM6DSL_REG_CTRL9_XL,
    LSM6DSL_REG_CTRL10_C,
    LSM6DSL_REG_FIFO_CTRL1,
    LSM6DSL_REG_FIFO_CTRL2,
    LSM6DSL_REG_FIFO_CTRL3,
    LSM6DSL_REG_FIFO_CTRL4,
    LSM6DSL_REG_FIFO_CTRL5,
    LSM6DSL_REG_DRDY_PULSE_CFG,
    LSM6DSL_REG_INT1_CTRL,
    LSM6DSL_REG_INT2_CTRL
};

const uint8_t LSM6DSL_DEFAULT_CONFIG[] = {
    LSM6DSL_CTRL1_XL_CONFIG,
    LSM6DSL_CTRL2_G_CONFIG,
    LSM6DSL_CTRL3_C_CONFIG,
    LSM6DSL_CTRL4_C_CONFIG,
    LSM6DSL_CTRL5_C_CONFIG,
    LSM6DSL_CTRL6_C_CONFIG,
    LSM6DSL_CTRL7_G_CONFIG,
    LSM6DSL_CTRL8_XL_CONFIG,
    LSM6DSL_CTRL9_XL_CONFIG,
    LSM6DSL_CTRL10_C_CONFIG,
    LSM6DSL_FIFO_CTRL1_CONFIG,
    LSM6DSL_FIFO_CTRL2_CONFIG,
    LSM6DSL_FIFO_CTRL3_CONFIG,
    LSM6DSL_FIFO_CTRL4_CONFIG,
    LSM6DSL_FIFO_CTRL5_CONFIG,
    LSM6DSL_DRDY_PULSE_CONFIG,
    LSM6DSL_INT1_CTRL_CONFIG,
    LSM6DSL_INT2_CTRL_CONFIG
};

// Sensor data buffers
uint8_t lsm6dsl_registers[LSM6DSL_NUM_CONFIG_REGISTERS];
int16_t acceleration[3];  // X, Y, Z
int16_t angular_rate[3];  // X, Y, Z
uint8_t fifo_buffer[LSM6DSL_FIFO_BUFFER_SIZE];
uint16_t fifo_samples_available = 0;

// ========== Private Function Prototypes ==========
static HAL_StatusTypeDef LSM6DSL_ReadRegister(uint8_t reg, uint8_t *data);
static HAL_StatusTypeDef LSM6DSL_WriteRegister(uint8_t reg, uint8_t value);
static HAL_StatusTypeDef LSM6DSL_ReadMultipleRegisters(uint8_t reg, uint8_t *data, uint16_t length);

// ========== I2C Initialization ==========
void LSM6DSL_I2C_Init(I2C_HandleTypeDef *hi2c)
{
    hi2c_lsm6dsl = hi2c;
}

// ========== Core I2C Functions ==========
HAL_StatusTypeDef LSM6DSL_ReadRegister(uint8_t reg, uint8_t *data)
{
    return HAL_I2C_Mem_Read(hi2c_lsm6dsl, LSM6DSL_I2C_ADDR, reg,
                          I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef LSM6DSL_WriteRegister(uint8_t reg, uint8_t value)
{
    return HAL_I2C_Mem_Write(hi2c_lsm6dsl, LSM6DSL_I2C_ADDR, reg,
                           I2C_MEMADD_SIZE_8BIT, &value, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef LSM6DSL_ReadMultipleRegisters(uint8_t reg, uint8_t *data, uint16_t length)
{
    return HAL_I2C_Mem_Read(hi2c_lsm6dsl, LSM6DSL_I2C_ADDR, reg,
                          I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
}

// ========== Configuration Functions ==========
HAL_StatusTypeDef LSM6DSL_ConfigureDevice(void)
{
    HAL_StatusTypeDef status;

    // Software reset the device
    status = LSM6DSL_WriteRegister(LSM6DSL_REG_CTRL3_C, 0x01);
    if (status != HAL_OK) return status;
    HAL_Delay(10); // Wait for reset

    // Write all configuration registers
    for (uint8_t i = 0; i < LSM6DSL_NUM_CONFIG_REGISTERS; i++) {
        status = LSM6DSL_WriteRegister(LSM6DSL_REGISTER_ADDRESSES[i],
                                      LSM6DSL_DEFAULT_CONFIG[i]);
        if (status != HAL_OK) return status;
    }

    return HAL_OK;
}

HAL_StatusTypeDef LSM6DSL_VerifyConfiguration(void)
{
    HAL_StatusTypeDef status;

    // Read all configuration registers
    for (uint8_t i = 0; i < LSM6DSL_NUM_CONFIG_REGISTERS; i++) {
        status = LSM6DSL_ReadRegister(LSM6DSL_REGISTER_ADDRESSES[i],
                                     &lsm6dsl_registers[i]);
        if (status != HAL_OK) return status;

        // Compare with expected values
        if (lsm6dsl_registers[i] != LSM6DSL_DEFAULT_CONFIG[i]) {
            // Rewrite incorrect register
            status = LSM6DSL_WriteRegister(LSM6DSL_REGISTER_ADDRESSES[i],
                                         LSM6DSL_DEFAULT_CONFIG[i]);
            if (status != HAL_OK) return status;
        }
    }

    return HAL_OK;
}

// ========== Data Acquisition Functions ==========
HAL_StatusTypeDef LSM6DSL_ReadAcceleration(void)
{
    uint8_t data[6];
    HAL_StatusTypeDef status;

    status = LSM6DSL_ReadMultipleRegisters(LSM6DSL_REG_OUTX_L_XL, data, 6);
    if (status != HAL_OK) return status;

    // Combine bytes into 16-bit values
    acceleration[0] = (int16_t)((data[1] << 8) | data[0]); // X
    acceleration[1] = (int16_t)((data[3] << 8) | data[2]); // Y
    acceleration[2] = (int16_t)((data[5] << 8) | data[4]); // Z

    return HAL_OK;
}

HAL_StatusTypeDef LSM6DSL_ReadAngularRate(void)
{
    uint8_t data[6];
    HAL_StatusTypeDef status;

    status = LSM6DSL_ReadMultipleRegisters(LSM6DSL_REG_OUTX_L_G, data, 6);
    if (status != HAL_OK) return status;

    // Combine bytes into 16-bit values
    angular_rate[0] = (int16_t)((data[1] << 8) | data[0]); // X
    angular_rate[1] = (int16_t)((data[3] << 8) | data[2]); // Y
    angular_rate[2] = (int16_t)((data[5] << 8) | data[4]); // Z

    return HAL_OK;
}

// ========== FIFO Functions ==========
HAL_StatusTypeDef LSM6DSL_CheckFIFOStatus(void)
{
    uint8_t status_reg[2];
    HAL_StatusTypeDef status;

    // Read FIFO status registers
    status = LSM6DSL_ReadMultipleRegisters(LSM6DSL_REG_FIFO_STATUS1, status_reg, 2);
    if (status != HAL_OK) return status;

    // Extract number of samples (14-bit value)
    fifo_samples_available = ((status_reg[1] & 0x3F) << 8) | status_reg[0];

    return HAL_OK;
}

HAL_StatusTypeDef LSM6DSL_ReadFIFO(void)
{
    HAL_StatusTypeDef status;

    // Check how many samples are available
    status = LSM6DSL_CheckFIFOStatus();
    if (status != HAL_OK) return status;

    if (fifo_samples_available == 0) return HAL_OK;

    // Calculate bytes to read (each sample is 6 bytes)
    uint16_t bytes_to_read = fifo_samples_available * 6;
    if (bytes_to_read > LSM6DSL_FIFO_BUFFER_SIZE) {
        bytes_to_read = LSM6DSL_FIFO_BUFFER_SIZE;
    }

    // Read FIFO data
    status = LSM6DSL_ReadMultipleRegisters(LSM6DSL_REG_FIFO_DATA_OUT_L,
                                         fifo_buffer, bytes_to_read);

    return status;
}

void LSM6DSL_ProcessFIFOData(void)
{
    for (uint16_t i = 0; i < fifo_samples_available; i++) {
        uint16_t offset = i * 6;

        // Process accelerometer data (first 3 axes)
        int16_t acc_x = (int16_t)((fifo_buffer[offset+1] << 8) | (fifo_buffer[offset]));
        int16_t acc_y = (int16_t)((fifo_buffer[offset+3] << 8) | (fifo_buffer[offset+2]));
        int16_t acc_z = (int16_t)((fifo_buffer[offset+5] << 8) | (fifo_buffer[offset+4]));

        // Process gyroscope data (next 3 axes if available in your FIFO configuration)
        // ...
    }
}

// ========== Combined Functions ==========
HAL_StatusTypeDef LSM6DSL_ReadSensorData(void)
{
    HAL_StatusTypeDef status;

    // Read acceleration
    status = LSM6DSL_ReadAcceleration();
    if (status != HAL_OK) return status;

    // Read angular rate
    status = LSM6DSL_ReadAngularRate();

    return status;
}

HAL_StatusTypeDef LSM6DSL_Initialize(void)
{
    HAL_StatusTypeDef status;

    // Configure device
    status = LSM6DSL_ConfigureDevice();
    if (status != HAL_OK) return status;

    // Verify configuration
    status = LSM6DSL_VerifyConfiguration();

    return status;
}
