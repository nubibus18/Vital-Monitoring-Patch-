	/*
 * ADPD1080.h
 *
 *  Created on: Apr 12, 2025
 *      Author: 36dhe
 */

#ifndef INC_ADPD1080_H_
#define INC_ADPD1080_H_
#include "stm32wbxx_hal.h"

#define ADPD1080_ADDR     (0x64 << 1)

#define REG_FIFO_NUMBER			0x00 //Reg to read the number of Bytes stored in Buffer
#define REG_FIFO_VALUES			0x60 //Reg to Read the FIFO

#define REG_SLOT_A_TIMING       0x31
#define REG_SLOT_B_TIMING       0x36
#define REG_LED_A_OFFSET_WIDTH  0x30
#define REG_LED_B_OFFSET_WIDTH  0x35
#define REG_LED1_FINE           0x22
#define REG_LED2_FINE           0x23
#define REG_LED3_FINE           0x24
#define REG_MODE                0x10
#define REG_LED_CONTROL         0x14
#define REG_CLK_CONTROL         0x4B
#define REG_FIFO_CFG            0x11
#define REG_NUM_AVG             0x15
#define REG_INTEG_WIDTH_A       0x39
#define REG_INTEG_WIDTH_B       0x3B
#define REG_TIA_CFG             0x55
#define REG_AFE_CFG_SLOT_A      0x42
#define REG_SAMPLE_CLK          0x06
#define REG_CLK_DIV             0x01
#define REG_CLK_CALIB           0x02

// Adding non-existent registers
#define REG_OPERATING          0x11
#define REG_SAMPLE_FREQ        0x12
#define REG_INTERRUPT_CONFIG   0x01
#define REG_GPIO_CONFIG        0x02
#define REG_ILED_FINE          0x25


#define ADPD1080_REG_SLOT_A_TIMING      0x3214
#define ADPD1080_REG_SLOT_B_TIMING      0x3214
#define ADPD1080_REG_LED_A_OFFSET_WIDTH 0x0519
#define ADPD1080_REG_LED_B_OFFSET_WIDTH 0x0519
#define ADPD1080_REG_LED1_FINE          0x3005
#define ADPD1080_REG_LED2_FINE          0x3005
#define ADPD1080_REG_LED3_FINE          0x3005
#define ADPD1080_REG_MODE               0x0001
#define ADPD1080_REG_LED_CONTROL        0x055F
#define ADPD1080_REG_CLK_CONTROL        0x2680
#define ADPD1080_REG_FIFO_CFG           0x31B8
#define ADPD1080_REG_NUM_AVG            0x0000
#define ADPD1080_REG_INTEG_WIDTH_A      0x31E0
#define ADPD1080_REG_INTEG_WIDTH_B      0x31E0
#define ADPD1080_REG_TIA_CFG            0x0000
#define ADPD1080_REG_AFE_CFG_SLOT_A     0x1C06
#define ADPD1080_REG_SAMPLE_CLK         0x3700
#define ADPD1080_REG_CLK_DIV            0x009F
#define ADPD1080_REG_CLK_CALIB          0x0307
#define ADPD1080_REG_SAMPLE_FREQ        0x0032




// ========== Data Structures ==========

#define ADPD1080_TOTAL_REGS         20  // Adjust to match the size of your register list
#define ADPD1080_BUFFER				112  //ADPD1080 buffer size in Bytes
#define ADPD1080_ROWS 4096



extern uint8_t adpd1080_reg_addresses[ADPD1080_TOTAL_REGS];
extern uint16_t adpd1080_reg_values[ADPD1080_TOTAL_REGS];
extern uint16_t adpd1080_recieved_array[ADPD1080_BUFFER/2]; // Array to store the received values from the FIFO
extern uint8_t fifo_byte_count; // Variable to store the number of bytes in the FIFO
extern uint32_t adpd1080_actual_reading_row_1[ADPD1080_BUFFER/16]; // Array to store the actual readings from the FIFO
extern uint32_t adpd1080_actual_reading_row_2[ADPD1080_BUFFER/16]; // Array to store the actual readings from the FIFO
extern uint32_t adpd1080_actual_reading_row_3[ADPD1080_BUFFER/16]; // Array to store the actual readings from the FIFO
extern uint32_t adpd1080_actual_reading_row_4[ADPD1080_BUFFER/16]; // Array to store the actual readings from the FIFO
extern uint32_t row_index;
extern uint8_t NewReading_Check;
// ========== Function Prototypes ==========


HAL_StatusTypeDef ADPD1080_ReadAllRegisters(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef ADPD1080_WriteAllRegisters(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef ADPD1080_CheckRegisters(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef ADPD1080_Read_Buffer(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef ADPD1080_Check_FIFO(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef ADPD1080_LEDoff(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef ADPD1080_LEDon(I2C_HandleTypeDef *hi2c);
void ADPD1080_ProcessFIFO(void);


#endif /* INC_ADPD1080_H_ */
