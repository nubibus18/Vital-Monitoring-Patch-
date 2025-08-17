/*
 * LSM6DSL.h
 *
 *  Created on: May 4, 2025
 *      Author: 36dhe
 */

#ifndef INC_LSM6DSL_H_
#define INC_LSM6DSL_H_

#include "stm32wbxx_hal.h"

// I2C address (adjust if SDO/SA0 pin is high)
#define LSM6DSL_ADDR         (0x6A << 1)  // or (0x6B << 1)

// ========== Register Map ==========
#define REG_FUNC_CFG_ACCESS        0x01
#define REG_SENSOR_SYNC_TIME_FRAME 0x04
#define REG_SENSOR_SYNC_RES_RATIO  0x05
#define REG_FIFO_CTRL1             0x06
#define REG_FIFO_CTRL2             0x07
#define REG_FIFO_CTRL3             0x08
#define REG_FIFO_CTRL4             0x09
#define REG_FIFO_CTRL5             0x0A
#define REG_DRDY_PULSE_CFG_G       0x0B
#define REG_INT1_CTRL              0x0D
#define REG_INT2_CTRL              0x0E
#define REG_WHO_AM_I               0x0F  // Should return 0x6A

#define REG_CTRL1_XL               0x10
#define REG_CTRL2_G                0x11
#define REG_CTRL3_C                0x12
#define REG_CTRL4_C                0x13
#define REG_CTRL5_C                0x14
#define REG_CTRL6_C                0x15
#define REG_CTRL7_G                0x16
#define REG_CTRL8_XL               0x17
#define REG_CTRL9_XL               0x18
#define REG_CTRL10_C               0x19
#define REG_MASTER_CONFIG          0x1A

#define REG_WAKE_UP_SRC            0x1B
#define REG_TAP_SRC                0x1C
#define REG_D6D_SRC                0x1D
#define REG_STATUS_REG             0x1E

#define REG_OUT_TEMP_L             0x20
#define REG_OUT_TEMP_H             0x21
#define REG_OUTX_L_G               0x22
#define REG_OUTX_H_G               0x23
#define REG_OUTY_L_G               0x24
#define REG_OUTY_H_G               0x25
#define REG_OUTZ_L_G               0x26
#define REG_OUTZ_H_G               0x27
#define REG_OUTX_L_XL              0x28
#define REG_OUTX_H_XL              0x29
#define REG_OUTY_L_XL              0x2A
#define REG_OUTY_H_XL              0x2B
#define REG_OUTZ_L_XL              0x2C
#define REG_OUTZ_H_XL              0x2D

#define REG_SENSORHUB1             0x2E
#define REG_SENSORHUB12            0x39
#define REG_FIFO_STATUS1           0x3A
#define REG_FIFO_STATUS2           0x3B
#define REG_FIFO_STATUS3           0x3C
#define REG_FIFO_STATUS4           0x3D
#define REG_FIFO_DATA_OUT_L        0x3E
#define REG_FIFO_DATA_OUT_H        0x3F
#define REG_TIMESTAMP0             0x40
#define REG_TIMESTAMP1             0x41
#define REG_TIMESTAMP2             0x42

#define REG_STEP_TIMESTAMP_L       0x49
#define REG_STEP_TIMESTAMP_H       0x4A
#define REG_STEP_COUNTER_L         0x4B
#define REG_STEP_COUNTER_H         0x4C

#define REG_FUNC_SRC1              0x53
#define REG_FUNC_SRC2              0x54
#define REG_WRIST_TILT_IA          0x55

#define REG_TAP_CFG                0x58
#define REG_TAP_THS_6D             0x59
#define REG_INT_DUR2               0x5A
#define REG_WAKE_UP_THS            0x5B
#define REG_WAKE_UP_DUR            0x5C
#define REG_FREE_FALL              0x5D
#define REG_MD1_CFG                0x5E
#define REG_MD2_CFG                0x5F
#define REG_MASTER_CMD_CODE        0x60
#define REG_SENS_SYNC_SPI_ERROR    0x61

#define REG_OUT_MAG_RAW_X_L        0x66
#define REG_OUT_MAG_RAW_X_H        0x67
#define REG_OUT_MAG_RAW_Y_L        0x68
#define REG_OUT_MAG_RAW_Y_H        0x69
#define REG_OUT_MAG_RAW_Z_L        0x6A
#define REG_OUT_MAG_RAW_Z_H        0x6B

#define REG_X_OFS_USR              0x73
#define REG_Y_OFS_USR              0x74
#define REG_Z_OFS_USR              0x75

// ========== Global Buffers ==========
#define LSM6DSL_MAX_REGS           0x7F  // Highest addressable register

extern uint8_t lsm6dsl_registers[LSM6DSL_MAX_REGS + 1];  // Holds register snapshots

// ========== Data Buffers ==========

#define LSM6DSL_BUFFER_SIZE         256  // Adjust: FIFO buffer size in bytes
#define LSM6DSL_DATA_ROWS           4096 // Maximum stored rows

extern uint8_t lsm6dsl_registers[LSM6DSL_MAX_REGS + 1];        // Snapshot of all registers

extern uint16_t lsm6dsl_received_fifo[LSM6DSL_BUFFER_SIZE / 2]; // Raw FIFO data (assuming 16-bit words)
extern uint8_t fifo_byte_count;                                 // Number of bytes currently in FIFO

// Processed sensor readings (per FIFO burst)
extern int16_t lsm6dsl_accel_x[LSM6DSL_BUFFER_SIZE / 12];
extern int16_t lsm6dsl_accel_y[LSM6DSL_BUFFER_SIZE / 12];
extern int16_t lsm6dsl_accel_z[LSM6DSL_BUFFER_SIZE / 12];
extern int16_t lsm6dsl_gyro_x[LSM6DSL_BUFFER_SIZE / 12];
extern int16_t lsm6dsl_gyro_y[LSM6DSL_BUFFER_SIZE / 12];
extern int16_t lsm6dsl_gyro_z[LSM6DSL_BUFFER_SIZE / 12];

extern uint32_t row_index;
extern uint8_t NewReading_Check;



// ========== Function Prototypes ==========
HAL_StatusTypeDef LSM6DSL_ReadRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *value);
HAL_StatusTypeDef LSM6DSL_ReadAllRegisters(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef LSM6DSL_WriteRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t value);
void LSM6DSL_ProcessData(void);

#endif /* INC_LSM6DSL_H_ */
