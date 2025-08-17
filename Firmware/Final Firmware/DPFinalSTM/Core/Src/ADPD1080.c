/*
 * ADPD1080.c
 *
 *  Created on: Apr 12, 2025
 *      Author: 36dhe
 */



#include "ADPD1080.h"
#include "stm32wbxx_hal.h"
#include <stdbool.h>
#include <stdint.h>


// ========== Register Addresses ==========
// These registers should match the ones defined in the header file
uint8_t adpd1080_reg_addresses[] = {
    REG_SLOT_A_TIMING,
    REG_SLOT_B_TIMING,
    REG_LED_A_OFFSET_WIDTH,
    REG_LED_B_OFFSET_WIDTH,
    REG_LED1_FINE,
    REG_LED2_FINE,
    REG_LED3_FINE,
    REG_MODE,
    REG_LED_CONTROL,
    REG_CLK_CONTROL,
    REG_FIFO_CFG,
    REG_NUM_AVG,
    REG_INTEG_WIDTH_A,
    REG_INTEG_WIDTH_B,
    REG_TIA_CFG,
    REG_AFE_CFG_SLOT_A,
    REG_SAMPLE_CLK,
    REG_CLK_DIV,
    REG_CLK_CALIB,
	ADPD1080_REG_SAMPLE_FREQ
};

uint16_t adpd1080_reg_values_write[]={
    ADPD1080_REG_SLOT_A_TIMING,
    ADPD1080_REG_SLOT_B_TIMING,
    ADPD1080_REG_LED_A_OFFSET_WIDTH,
    ADPD1080_REG_LED_B_OFFSET_WIDTH,
    ADPD1080_REG_LED1_FINE,
    ADPD1080_REG_LED2_FINE,
    ADPD1080_REG_LED3_FINE,
    ADPD1080_REG_MODE,
    ADPD1080_REG_LED_CONTROL,
    ADPD1080_REG_CLK_CONTROL,
    ADPD1080_REG_FIFO_CFG,
    ADPD1080_REG_NUM_AVG,
    ADPD1080_REG_INTEG_WIDTH_A,
    ADPD1080_REG_INTEG_WIDTH_B,
    ADPD1080_REG_TIA_CFG,
    ADPD1080_REG_AFE_CFG_SLOT_A,
    ADPD1080_REG_SAMPLE_CLK,
    ADPD1080_REG_CLK_DIV,
    ADPD1080_REG_CLK_CALIB,
	REG_SAMPLE_FREQ,
} ;



// Array to store the values of the registers
uint16_t adpd1080_reg_values[ADPD1080_TOTAL_REGS] = {0};
uint16_t adpd1080_recieved_array[ADPD1080_BUFFER/2] = {0}; // Array to store the received values from the FIFO
uint8_t fifo_byte_count = 0; // Variable to store the number of bytes in the FIFO
uint32_t adpd1080_actual_reading_row_1[ADPD1080_BUFFER/16] = {0}; // Array to store the actual readings from the FIFO
uint32_t adpd1080_actual_reading_row_2[ADPD1080_BUFFER/16] = {0};
uint32_t adpd1080_actual_reading_row_3[ADPD1080_BUFFER/16] = {0};
uint32_t adpd1080_actual_reading_row_4[ADPD1080_BUFFER/16] = {0};
uint32_t row_index=0;
uint8_t NewReading_Check=0;


// ========== Function Implementations ==========

/**
 * @brief  Read all registers of the ADPD1080 and store the values in the global array.
 * @param  hi2c: Pointer to the I2C handle structure.
 * @retval None
 */
HAL_StatusTypeDef ADPD1080_ReadAllRegisters(I2C_HandleTypeDef *hi2c)
{
    uint8_t reg_data[2]; // 2 bytes of data for 16-bit register value
    HAL_StatusTypeDef status;

    // Loop through all registers and read their values
    for (uint8_t i = 0; i < ADPD1080_TOTAL_REGS; i++) {
        // Read the register value (2 bytes) from ADPD1080
        status = HAL_I2C_Mem_Read(hi2c, ADPD1080_ADDR, adpd1080_reg_addresses[i], I2C_MEMADD_SIZE_8BIT, reg_data, 2, HAL_MAX_DELAY);

        if (status == HAL_OK) {
            // Combine the two bytes into a 16-bit value
            adpd1080_reg_values[i] = (reg_data[0] << 8) | reg_data[1];
        } else {
            // Handle error in reading register
            adpd1080_reg_values[i] = 0xFFFF; // Error code
        }
    }
    return status; // Return the last status
}

/**
 * @brief  Write all registers of the ADPD1080 with the values from the global array.
 * @param  hi2c: Pointer to the I2C handle structure.
 * @retval None
 */
HAL_StatusTypeDef ADPD1080_WriteAllRegisters(I2C_HandleTypeDef *hi2c)
{
    uint8_t reg_data[2]; // 2 bytes of data for 16-bit register value
    HAL_StatusTypeDef status;

    // Write Startup Configs
    // Write 0x2680 to register 0x4B
    reg_data[0] = (0x2680 >> 8) & 0xFF; // High byte
    reg_data[1] = 0x2680 & 0xFF;        // Low byte
    status = HAL_I2C_Mem_Write(hi2c, ADPD1080_ADDR, 0x4B, I2C_MEMADD_SIZE_8BIT, reg_data, 2, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return status; // Return the last status on failure
    }

    // Write 0x0001 to register 0x10
    reg_data[0] = (0x0001 >> 8) & 0xFF; // High byte
    reg_data[1] = 0x0001 & 0xFF;        // Low byte
    status = HAL_I2C_Mem_Write(hi2c, ADPD1080_ADDR, 0x10, I2C_MEMADD_SIZE_8BIT, reg_data, 2, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return status; // Return the last status on failure
    }



    for (uint8_t i = 0; i < ADPD1080_TOTAL_REGS; i++) {
        // Split the 16-bit value into two bytes
        reg_data[0] = (adpd1080_reg_values_write[i] >> 8) & 0xFF; // High byte
        reg_data[1] = adpd1080_reg_values_write[i] & 0xFF;        // Low byte

        // Write the register value (2 bytes) to ADPD1080
        status = HAL_I2C_Mem_Write(hi2c, ADPD1080_ADDR, adpd1080_reg_addresses[i], I2C_MEMADD_SIZE_8BIT, reg_data, 2, HAL_MAX_DELAY);

        if (status != HAL_OK) {
            return status; // Return the last status on failure
        }
    }

    reg_data[0] = (0x0002 >> 8) & 0xFF; // High byte
    reg_data[1] = 0x0002 & 0xFF;        // Low byte
    status = HAL_I2C_Mem_Write(hi2c, ADPD1080_ADDR, 0x10, I2C_MEMADD_SIZE_8BIT, reg_data, 2, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return status; // Return the last status on failure
    }

    return HAL_OK; // Return success if all registers are written successfully
}

/**
 * @brief  Check if the read registers match the expected write values.
 * @retval uint8_t: Returns 1 if all registers match, 0 otherwise.
 */
HAL_StatusTypeDef ADPD1080_CheckRegisters(I2C_HandleTypeDef *hi2c)
{
    HAL_StatusTypeDef status;

    for (uint8_t i = 0; i < ADPD1080_TOTAL_REGS; i++) {
        if (adpd1080_reg_values[i] != adpd1080_reg_values_write[i]) {
            // Mismatch found, write all registers and return the status
            status = ADPD1080_WriteAllRegisters(hi2c);
            return status;
        }
    }
    return HAL_OK; // All registers match
}

/**
 * @brief  Take Sensor Readings.
 * @param  hi2c: Pointer to the I2C handle structure.
 * @param  Buffer: Number of Bytes that can be stored before Interrupt.
 * @retval uint16_t: return a array of the recieved readings
 */

 HAL_StatusTypeDef ADPD1080_Read_Buffer(I2C_HandleTypeDef *hi2c)
 {
    uint8_t reg_data[112]; // Buffer to store 112 bytes of data
    HAL_StatusTypeDef status;

    // Read 112 bytes from the REG_FIFO_VALUES register
    status = HAL_I2C_Mem_Read(hi2c, ADPD1080_ADDR, REG_FIFO_VALUES, I2C_MEMADD_SIZE_8BIT, reg_data, 112, HAL_MAX_DELAY);
    // Send the entire reg_data array as a single USB packet
    

    if (status == HAL_OK) {
        // Store the received data into the 16-bit array
        for (uint8_t i = 0; i < 56; i++) {
            adpd1080_recieved_array[i] = (reg_data[2 * i] << 8) | reg_data[2 * i + 1];
            //sendDataOverUSB(reg_data[2*i], reg_data[2 * i + 1], adpd1080_recieved_array[i] , 0);
            //HAL_Delay(1);

        }
    } else {
        // Handle error in reading register
        for (uint8_t i = 0; i < 56; i++) {
            adpd1080_recieved_array[i] = 0xFFFF; // Error code
        }
    }

    return status; // Return the status
 }



/**
 * @brief  Check the FIFO status and read the buffer if necessary.
 * @param  hi2c: Pointer to the I2C handle structure.
 * @retval HAL_StatusTypeDef: Status of the operation.
 */

HAL_StatusTypeDef ADPD1080_Check_FIFO(I2C_HandleTypeDef *hi2c)
{
    uint8_t reg_data[2]; // 2 bytes of data for 16-bit register value
    HAL_StatusTypeDef status;

    // Read the FIFO status register (0x00)
    status = HAL_I2C_Mem_Read(hi2c, ADPD1080_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, reg_data, 2, HAL_MAX_DELAY);
    //sendDataOverUSB(reg_data[0], 0, 0, 0);
    //status = HAL_I2C_Mem_Read(hi2c, ADPD1080_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, reg_data, 2, HAL_MAX_DELAY);
    //sendDataOverUSB(reg_data[0], 0, 0, 0);


    if (status == HAL_OK) {
        // Extract the number of bytes currently stored in the FIFO (Bits[15:8])
        fifo_byte_count = reg_data[0]; // High byte contains the FIFO byte count
        //sendDataOverUSB(fifo_byte_count, 0, 0, 0);

        // Compare fifo_byte_count to ADPD_BUFFER
        if (fifo_byte_count >= ADPD1080_BUFFER) {
            // Call the function to read the FIFO
            status = ADPD1080_Read_Buffer(hi2c);
            if (status == HAL_OK) {
                ADPD1080_ProcessFIFO();
                NewReading_Check=1;
            }

        }
        else{
            //sendDataOverUSB(21,21, 21, 21); // Send the FIFO byte count over USB
        }

        return status;
    } else {
        fifo_byte_count = 0; // Set to 0 in case of error
        return status; // Return the error status
    }
}

/**
 * @brief  Process the FIFO data and store it in the actual readings array.
 * @retval None
 */

void ADPD1080_ProcessFIFO(void)
{
    // Process the FIFO data stored in adpd1080_recieved_array
    for (uint8_t i = 0; i < 112 / 16; i++) {
        // Row 1
        adpd1080_actual_reading_row_1[i] = ((uint32_t)adpd1080_recieved_array[8 * i + 1] << 16) | adpd1080_recieved_array[8 * i];
        // Row 2
        adpd1080_actual_reading_row_2[i] = ((uint32_t)adpd1080_recieved_array[8 * i + 3] << 16) | adpd1080_recieved_array[8 * i + 2];
        // Row 3
        adpd1080_actual_reading_row_3[i] = ((uint32_t)adpd1080_recieved_array[8 * i + 5] << 16) | adpd1080_recieved_array[8 * i + 4];
        //sendDataOverUSB(adpd1080_actual_reading_row_3[i], 0, 0, 0);

        // Row 4
        adpd1080_actual_reading_row_4[i] = ((uint32_t)adpd1080_recieved_array[8 * i + 7] << 16) | adpd1080_recieved_array[8 * i + 6];
    }
}

void ADPD1080_LEDoff(&I2C_HandleTypeDef *hi2c)
{
    uint8_t reg_data[2]; // 2 bytes of data for 16-bit register value
    HAL_StatusTypeDef status;

    // Write 0x0000 to register 0x4B
    reg_data[0] = (0x0001 >> 8) & 0xFF; // High byte
    reg_data[1] = 0x0001 & 0xFF;        // Low byte
    status = HAL_I2C_Mem_Write(hi2c, ADPD1080_ADDR, 0x10, I2C_MEMADD_SIZE_8BIT, reg_data, 2, HAL_MAX_DELAY);

    // Write 0x0000 to register 0x4B
    reg_data[0] = (0x80FF >> 8) & 0xFF; // High byte
    reg_data[1] = 0x80FF & 0xFF;        // Low byte
    status = HAL_I2C_Mem_Write(hi2c, ADPD1080_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, reg_data, 2, HAL_MAX_DELAY);
    

    if (status != HAL_OK) {
        return status; // Return the last status on failure
    }
}
 

void ADPD1080_LEDon(&I2C_HandleTypeDef *hi2c)
{
    uint8_t reg_data[2]; // 2 bytes of data for 16-bit register value
    HAL_StatusTypeDef status;

    // Write 0x0001 to register 0x4B
    reg_data[0] = (0x0002 >> 8) & 0xFF; // High byte
    reg_data[1] = 0x0002 & 0xFF;        // Low byte
    status = HAL_I2C_Mem_Write(hi2c, ADPD1080_ADDR, 0x10, I2C_MEMADD_SIZE_8BIT, reg_data, 2, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return status; // Return the last status on failure
    }
}

