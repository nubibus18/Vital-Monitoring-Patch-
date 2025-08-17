	/*
 * ADPD1080.h
 *
 *  Created on: Apr 14, 2025
 *      Author: 36dhe
 */


#ifndef ADP1080_REG_GEN_H
#define ADP1080_REG_GEN_H

#include "ADPD1080.h"
#include "stm32wbxx_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>



// Configuration structure
typedef struct {
    uint16_t address;
    uint16_t value;
} ADPD1080_Register;

// Default configuration based on Python script
static const ADPD1080_Register adpd1080_default_config[] = {
    // Slot timing registers
    {REG_SLOT_A_TIMING, 0x0046},     // Slot A: 70 pulses, 20µs period (0x46 = 70)
    {REG_SLOT_B_TIMING, 0x0046},     // Slot B: 70 pulses, 20µs period
    
    // LED width and offset registers
    {REG_LED_A_OFFSET_WIDTH, 0x0519}, // Slot A: 5µs width, 25µs offset
    {REG_LED_B_OFFSET_WIDTH, 0x0519}, // Slot B: 5µs width, 25µs offset
    
    // LED current registers
    {REG_LED3_FINE, 0xA00F},         // LED3: scale=1, slew=0, coarse=15
    {REG_LED1_FINE, 0xA00F},         // LED1: scale=1, slew=0, coarse=15
    {REG_LED2_FINE, 0xA00F},         // LED2: scale=1, slew=0, coarse=15
    {REG_ILED_FINE, 0xF83F},         // Fine currents: LED1=31, LED2=31, LED3=31
    
    // Sampling frequency
    {REG_SAMPLE_FREQ, 0x0032},       // 50Hz sampling rate
    
    // LED control
    {REG_LED_CONTROL, 0x0551},       // Slot A LED3, Slot B LED1, PD fixed
    
    // Clock control
    {REG_CLK_CONTROL, 0x2700},       // clk32k_byp=0, clk32k_en=1, clk32k_adjust=0
    
    // Mode register
    {REG_MODE, 0x0001},              // Program mode
    
    // Number of averages
    {REG_NUM_AVG, 0x0000},           // Slot A: 0 (1 sample), Slot B: 0 (1 sample)
    
    // AFE integration settings
    {REG_INTEG_WIDTH_A, 0x30C0},     // Slot A: width=6µs, offset=15000ns (0x1E0=480 steps)
    {REG_INTEG_WIDTH_B, 0x30C0},     // Slot B: width=6µs, offset=15000ns
    
    // Operating register (0x11)
    {REG_OPERATING, 0x21C5},         // rdout_mode=1, fifo_ovrn_prevent=1, slotb_fifo_mode=6, slotb_en=0, slota_fifo_mode=6, slota_en=1
    
    // TIA gain settings
    {REG_TIA_CFG, 0x0000},           // All TIA gains set to 0
    
    // Slot A AFE Mode Register
    {REG_AFE_CFG_SLOT_A, 0x1C02},    // slota_afe_mode=7, slota_int_gain=0, slota_int_as_buf=0, slota_tia_ind_en=0, slota_tia_vref=0, slota_tia_gain=2
    
    // FIFO Configuration
    {REG_SAMPLE_CLK, 0x3700},        // FIFO threshold=55
    
    // Interrupt Configuration
    {REG_INTERRUPT_CONFIG, 0x00BF},  // fifo_int_mask=0, slotb_int_mask=0, slota_int_mask=0
    
    // GPIO Configuration
    {REG_GPIO_CONFIG, 0x03F7}        // gpio1_drv=1, gpio1_pol=1, gpio0_ena=1, gpio0_drv=1, gpio0_pol=1
};

// Number of registers in default configuration
#define ADPD1080_CONFIG_SIZE (sizeof(adpd1080_default_config) / sizeof(ADPD1080_Register))

// Function prototypes
void ADPD1080_REG_GEN(void);




#endif // ADPD1080_REG_GEN_H