#ifndef LSM6DSL_I2C_H
#define LSM6DSL_I2C_H

#include "stm32wbxx_hal.h"

// I2C address
#define LSM6DSL_I2C_ADDR         0x6A // 7-bit address (usually 0x6A or 0x6B)

// Register addresses
#define LSM6DSL_REG_CTRL1_XL     0x10
#define LSM6DSL_REG_CTRL2_G      0x11
#define LSM6DSL_REG_CTRL3_C      0x12
#define LSM6DSL_REG_CTRL4_C      0x13
#define LSM6DSL_REG_CTRL5_C      0x14
#define LSM6DSL_REG_CTRL6_C      0x15
#define LSM6DSL_REG_CTRL7_G      0x16
#define LSM6DSL_REG_CTRL8_XL     0x17
#define LSM6DSL_REG_CTRL9_XL     0x18
#define LSM6DSL_REG_CTRL10_C     0x19
#define LSM6DSL_REG_FIFO_CTRL1   0x07
#define LSM6DSL_REG_FIFO_CTRL2   0x08
#define LSM6DSL_REG_FIFO_CTRL3   0x09
#define LSM6DSL_REG_FIFO_CTRL4   0x0A
#define LSM6DSL_REG_FIFO_CTRL5   0x0B
#define LSM6DSL_REG_DRDY_PULSE_CFG 0x0C
#define LSM6DSL_REG_INT1_CTRL    0x0D
#define LSM6DSL_REG_INT2_CTRL    0x0E

// Data output registers
#define LSM6DSL_REG_OUTX_L_XL    0x28
#define LSM6DSL_REG_OUTX_H_XL    0x29
#define LSM6DSL_REG_OUTY_L_XL    0x2A
#define LSM6DSL_REG_OUTY_H_XL    0x2B
#define LSM6DSL_REG_OUTZ_L_XL    0x2C
#define LSM6DSL_REG_OUTZ_H_XL    0x2D
#define LSM6DSL_REG_OUTX_L_G     0x22
#define LSM6DSL_REG_OUTX_H_G     0x23
#define LSM6DSL_REG_OUTY_L_G     0x24
#define LSM6DSL_REG_OUTY_H_G     0x25
#define LSM6DSL_REG_OUTZ_L_G     0x26
#define LSM6DSL_REG_OUTZ_H_G     0x27

// FIFO registers
#define LSM6DSL_REG_FIFO_STATUS1 0x1A
#define LSM6DSL_REG_FIFO_STATUS2 0x1B
#define LSM6DSL_REG_FIFO_DATA_OUT_L 0x3E

// Default configuration values
#define LSM6DSL_CTRL1_XL_CONFIG  0x60 // 416 Hz, ±8g
#define LSM6DSL_CTRL2_G_CONFIG   0x60 // 416 Hz, 2000 dps
#define LSM6DSL_CTRL3_C_CONFIG   0x44 // BDU enabled, auto-increment
#define LSM6DSL_CTRL4_C_CONFIG   0x00 // Default
#define LSM6DSL_CTRL5_C_CONFIG   0x00 // Default
#define LSM6DSL_CTRL6_C_CONFIG   0x00 // Default
#define LSM6DSL_CTRL7_G_CONFIG   0x00 // Default
#define LSM6DSL_CTRL8_XL_CONFIG  0x00 // Default
#define LSM6DSL_CTRL9_XL_CONFIG  0x00 // Default
#define LSM6DSL_CTRL10_C_CONFIG  0x00 // Default
#define LSM6DSL_FIFO_CTRL1_CONFIG 0x00 // FIFO disabled by default
#define LSM6DSL_FIFO_CTRL2_CONFIG 0x00
#define LSM6DSL_FIFO_CTRL3_CONFIG 0x00
#define LSM6DSL_FIFO_CTRL4_CONFIG 0x00
#define LSM6DSL_FIFO_CTRL5_CONFIG 0x00
#define LSM6DSL_DRDY_PULSE_CONFIG 0x00
#define LSM6DSL_INT1_CTRL_CONFIG 0x00
#define LSM6DSL_INT2_CTRL_CONFIG 0x00

// Constants
#define LSM6DSL_NUM_CONFIG_REGISTERS 18
#define LSM6DSL_FIFO_BUFFER_SIZE 512 // Adjust based on your needs

// Function prototypes
void LSM6DSL_I2C_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef LSM6DSL_Initialize(void);
HAL_StatusTypeDef LSM6DSL_ReadSensorData(void);
HAL_StatusTypeDef LSM6DSL_ReadFIFO(void);
void LSM6DSL_ProcessFIFOData(void);
HAL_StatusTypeDef LSM6DSL_ConfigureDevice(void);
HAL_StatusTypeDef LSM6DSL_VerifyConfiguration(void);

#endif // LSM6DSL_I2C_H
