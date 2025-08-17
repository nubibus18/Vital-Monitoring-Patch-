def set_gpio_config_register(gpio1_drv, gpio1_pol, gpio0_ena, gpio0_drv, gpio0_pol):
    """Set GPIO Configuration Register (0x02) with user inputs."""
    
    # Ensure inputs are either 0 or 1
    gpio1_drv = 1 if gpio1_drv else 0
    gpio1_pol = 1 if gpio1_pol else 0
    gpio0_ena = 1 if gpio0_ena else 0
    gpio0_drv = 1 if gpio0_drv else 0
    gpio0_pol = 1 if gpio0_pol else 0

    # Construct register value
    reg_value = (0x00 << 10) |  (gpio1_drv << 9) | (gpio1_pol << 8) | (0x00 << 3)  | (gpio0_ena << 2) |  (gpio0_drv << 1) | (gpio0_pol)
    return 0x02, reg_value


def set_interrupt_register(fifo_int_mask, slotb_int_mask, slota_int_mask):
    """Set Interrupt Configuration Register (0x01) with user inputs."""
    
    # Ensure inputs are either 0 or 1
    fifo_int_mask = 1 if fifo_int_mask else 0
    slotb_int_mask = 1 if slotb_int_mask else 0
    slota_int_mask = 1 if slota_int_mask else 0

    # Construct register value
    reg_value = (0x00 << 9) |  (fifo_int_mask << 8) |(0x1 << 7)  | (slotb_int_mask << 6) | (slota_int_mask << 5) | (0x1F)    

    return 0x01, reg_value



def generate_fifo_config(fifo_threshold):
    """
    Generate FIFO Configuration Register (0x06).

    :param fifo_threshold: FIFO length threshold (0-63)
    :return: Tuple (register_address, register_value)
    """
    # Ensure FIFO threshold is within valid range (0-63)
    fifo_threshold &= 0x3F  # Mask to keep only the lowest 6 bits

    # Shift FIFO_THRESH to bits [13:8], keep reserved bits as 0
    config_value = fifo_threshold << 8

    return 0x06, config_value



def set_slota_afe_mode_register(slota_afe_mode, slota_int_gain, slota_int_as_buf, slota_tia_ind_en, slota_tia_vref, slota_tia_gain):
    """Set Slot A AFE Mode Register (0x42)."""
    register_value = (slota_afe_mode << 10) | (slota_int_gain << 8) | (slota_int_as_buf << 7) | \
                     (slota_tia_ind_en << 6) | (slota_tia_vref << 4) | (0x1 << 2) | slota_tia_gain
    return 0x42, register_value



def set_tia_gain_register(
    slotb_tia_gain_4, slotb_tia_gain_3, slotb_tia_gain_2,
    slota_tia_gain_4, slota_tia_gain_3, slota_tia_gain_2
):
    """Set TIA gain values for Time Slots A and B in register 0x55.
    
    Parameters:
        slotb_tia_gain_4 (int): TIA gain for Time Slot B, Channel 4 (0-3).
        slotb_tia_gain_3 (int): TIA gain for Time Slot B, Channel 3 (0-3).
        slotb_tia_gain_2 (int): TIA gain for Time Slot B, Channel 2 (0-3).
        slota_tia_gain_4 (int): TIA gain for Time Slot A, Channel 4 (0-3).
        slota_tia_gain_3 (int): TIA gain for Time Slot A, Channel 3 (0-3).
        slota_tia_gain_2 (int): TIA gain for Time Slot A, Channel 2 (0-3).
    
    Returns:
        tuple: (register address, register value)
    """
    register_address = 0x55

    # Validate inputs (should be in range 0-3)
    for gain in [slotb_tia_gain_4, slotb_tia_gain_3, slotb_tia_gain_2, 
                 slota_tia_gain_4, slota_tia_gain_3, slota_tia_gain_2]:
        if gain not in range(4):
            raise ValueError("TIA gain values must be between 0 and 3")

    # Construct the register value
    register_value = (
        (slotb_tia_gain_4 << 10) |
        (slotb_tia_gain_3 << 8)  |
        (slotb_tia_gain_2 << 6)  |
        (slota_tia_gain_4 << 4)  |
        (slota_tia_gain_3 << 2)  |
        (slota_tia_gain_2 << 0)
    )

    return register_address, register_value



def set_led_current_registers(led1_coarse, led1_slew, led1_scale, 
                              led2_coarse, led2_slew, led2_scale, 
                              led3_coarse, led3_slew, led3_scale, 
                              led1_fine, led2_fine, led3_fine):
    """Set LED Current Registers (0x22, 0x23, 0x24, 0x25) for ADPD1080."""
    
    # Validate Coarse Current Settings (Bits [3:0], Range 0-15)
    def validate_coarse(value, led_name):
        if not (0 <= value <= 15):
            print(f"Warning: Invalid {led_name}_COARSE! Must be between 0-15. Defaulting to 0.")
            return 0
        return value

    # Validate Slew Rate (Bits [6:4], Range 0-7)
    def validate_slew(value, led_name):
        if not (0 <= value <= 7):
            print(f"Warning: Invalid {led_name}_SLEW! Must be between 0-7. Defaulting to 0.")
            return 0
        return value

    # Validate Scale Factor (Bit 13, 0 or 1)
    def validate_scale(value, led_name):
        if value not in [0, 1]:
            print(f"Warning: Invalid {led_name}_SCALE! Must be 0 or 1. Defaulting to 1 (100%).")
            return 1
        return value

    # Validate Fine Current Settings (Bits [4:0], Range 0-31)
    def validate_fine(value, led_name):
        if not (0 <= value <= 31):
            print(f"Warning: Invalid {led_name}_FINE! Must be between 0-31. Defaulting to 12.")
            return 12
        return value

    # Apply validation
    led1_coarse = validate_coarse(led1_coarse, "LED1")
    led1_slew = validate_slew(led1_slew, "LED1")
    led1_scale = validate_scale(led1_scale, "LED1")

    led2_coarse = validate_coarse(led2_coarse, "LED2")
    led2_slew = validate_slew(led2_slew, "LED2")
    led2_scale = validate_scale(led2_scale, "LED2")

    led3_coarse = validate_coarse(led3_coarse, "LED3")
    led3_slew = validate_slew(led3_slew, "LED3")
    led3_scale = validate_scale(led3_scale, "LED3")

    led1_fine = validate_fine(led1_fine, "LED1")
    led2_fine = validate_fine(led2_fine, "LED2")
    led3_fine = validate_fine(led3_fine, "LED3")

    # Construct register values
    reg_iled3_value = (led3_scale << 13) | (1 << 12) | (led3_slew << 4) | led3_coarse
    reg_iled1_value = (led1_scale << 13) | (1 << 12) | (led1_slew << 4) | led1_coarse
    reg_iled2_value = (led2_scale << 13) | (1 << 12) | (led2_slew << 4) | led2_coarse

    reg_iled_fine_value = (led3_fine << 11) | (led2_fine << 6) | (led1_fine)

    # Return register addresses and values
    return [(0x22,0x23,0x24,0x25),
        ( reg_iled3_value,
         reg_iled1_value,
         reg_iled2_value,
         reg_iled_fine_value)
    ]


def set_register_0x11(rdout_mode, fifo_ovrn_prevent, slotb_fifo_mode, slotb_en, slota_fifo_mode, slota_en):
    """
    Configure ADPD1080 register 0x11.
    
    Parameters:
        rdout_mode (int): Readback data mode (0: block sum, 1: block average)
        fifo_ovrn_prevent (int): FIFO overrun prevention (0: wrap around, 1: prevent overrun)
        slotb_fifo_mode (int): Time Slot B FIFO data format (0,1,2,4,6)
        slotb_en (int): Enable Time Slot B (0: disable, 1: enable)
        slota_fifo_mode (int): Time Slot A FIFO data format (0,1,2,4,6)
        slota_en (int): Enable Time Slot A (0: disable, 1: enable)
    
    Returns:
        tuple: (register_address, register_value)
    """
    if rdout_mode not in [0, 1]:
        raise ValueError("rdout_mode must be 0 or 1")
    if fifo_ovrn_prevent not in [0, 1]:
        raise ValueError("fifo_ovrn_prevent must be 0 or 1")
    if slotb_fifo_mode not in [0, 1, 2, 4, 6]:
        raise ValueError("Invalid slotb_fifo_mode. Allowed values: 0,1,2,4,6")
    if slotb_en not in [0, 1]:
        raise ValueError("slotb_en must be 0 or 1")
    if slota_fifo_mode not in [0, 1, 2, 4, 6]:
        raise ValueError("Invalid slota_fifo_mode. Allowed values: 0,1,2,4,6")
    if slota_en not in [0, 1]:
        raise ValueError("slota_en must be 0 or 1")
    
    register_value = (
        (rdout_mode << 13) |
        (fifo_ovrn_prevent << 12) |
        (slotb_fifo_mode << 6) |
        (slotb_en << 5) |
        (slota_fifo_mode << 2) |
        (slota_en << 0)
    )
    
    return 0x11, register_value

# Example usage:


def set_afe_integration(register_map, slot, width_us, offset_ns):
    """
    Configure the AFE integration window width and offset for a given time slot.

    Parameters:
        register_map (dict): Dictionary representing the device's register map.
        slot (str): 'A' for Time Slot A or 'B' for Time Slot B.
        width_us (int): AFE integration window width in microseconds (1 µs steps).
        offset_ns (int): AFE integration window offset in nanoseconds.
    
    Returns:
        dict: Updated register map with new AFE settings.
    """

    if slot.upper() == 'A':
        reg_addr = 0x39  # Register for Time Slot A
    elif slot.upper() == 'B':
        reg_addr = 0x3B  # Register for Time Slot B
    else:
        raise ValueError("Invalid slot! Use 'A' or 'B'.")

    if not (0 <= width_us <= 31):  # Width is 5 bits (15:11)
        raise ValueError("AFE width must be between 0 and 31 µs.")

    # Convert offset from nanoseconds to multiples of 31.25 ns (rounded)
    offset_steps = round(offset_ns / 31.25)

    if not (0 <= offset_steps <= 0x7FF):  # Offset is 11 bits (10:0)
        raise ValueError("AFE offset must be between 0 and 2047 (31.25 ns steps).")

    # Encode the width and offset into a 16-bit register value
    register_value = (width_us << 11) | offset_steps

    # Update the register map
    register_map[reg_addr] = register_value

    return register_map



def set_num_avg_register(slot_a_num_avg, slot_b_num_avg):
    """Set Number of Averages Register (0x15) with fixed reserved bits."""
    
    # Validate Slot A averaging factor (NA) - Bits [6:4]
    if not (0 <= slot_a_num_avg <= 7):
        print("Warning: Invalid SLOTA_NUM_AVG! Must be between 0-7. Defaulting to 0 (1 sample).")
        slot_a_num_avg = 0x0  

    # Validate Slot B averaging factor (NB) - Bits [10:8]
    if not (0 <= slot_b_num_avg <= 7):
        print("Warning: Invalid SLOTB_NUM_AVG! Must be between 0-7. Defaulting to 6 (64 samples).")
        slot_b_num_avg = 0x6  

    # Construct register value
    register_value = (slot_b_num_avg << 8) | (slot_a_num_avg << 4)

    return 0x15, register_value


def set_mode_register(mode):
    """Set Mode Register (0x10) with fixed bits [15:2] = 0x0000."""
    
    # Validate mode input
    if mode not in [0, 1, 2]:
        print("Warning: Invalid Mode! Must be 0 (standby), 1 (program), or 2 (normal). Defaulting to standby (0).")
        mode = 0x0  # Default to standby mode
    
    # Construct register value (Bits [15:2] = 0x0000, Bits [1:0] = mode)
    register_value = mode  # Since upper 14 bits are always 0
    
    return 0x10, register_value


def set_clock_control_register(clk32k_byp, clk32k_en, clk32k_adjust):
    """Set Clock Control Register (0x4B) with fixed bits [15:9] = 0x13."""
    
    FIXED_BITS = 0x13  # Bits [15:9] are fixed to 0x13

    # Validate input ranges
    if not (0 <= clk32k_byp <= 1):
        print("Warning: Invalid CLK32K_BYP. Must be 0 or 1. Defaulting to 0.")
        clk32k_byp = 0x0

    if not (0 <= clk32k_en <= 1):
        print("Warning: Invalid CLK32K_EN. Must be 0 or 1. Defaulting to 0.")
        clk32k_en = 0x0

    if not (0 <= clk32k_adjust <= 0x3F):  # 6-bit value (0 to 63)
        print("Warning: Invalid CLK32K_ADJUST. Must be 0-63. Defaulting to 0x12 (Typical Center Frequency).")
        clk32k_adjust = 0x12

    # Construct register value
    register_value = (FIXED_BITS << 9) | (clk32k_byp << 8) | (clk32k_en << 7) | (clk32k_adjust)
    
    return 0x4B, register_value


def set_led_control_register(slot_a_led_sel, slot_b_led_sel):
    """Set LED Control Register (0x14) with fixed PD selections (5)."""
    RESERVED_BITS = 0x0  # Reserved bits [15:12] must be 0x0
    FIXED_PD_SEL = 0x5   # Fixed value for both SLOTB_PD_SEL and SLOTA_PD_SEL (bits 11:8 and 7:4)

    # Validate input ranges
    if not (0 <= slot_a_led_sel <= 3):
        print("Warning: Invalid SLOTA_LED_SEL. Must be 0-3. Defaulting to 0x1 (LEDX1).")
        slot_a_led_sel = 0x1

    if not (0 <= slot_b_led_sel <= 3):
        print("Warning: Invalid SLOTB_LED_SEL. Must be 0-3. Defaulting to 0x0 (Float mode).")
        slot_b_led_sel = 0x0

    # Construct register value
    register_value = (RESERVED_BITS << 12) | (FIXED_PD_SEL << 8) | (FIXED_PD_SEL << 4) | (slot_b_led_sel << 2) | slot_a_led_sel
    return 0x14, register_value


def set_slot_timing_register(register, pulses, period_usec):
    """Set Slot A/B Timing Register (0x31 / 0x36)."""
    MIN_LED_PERIOD = 19  # Minimum LED period in µs

    # Ensure the period meets the minimum requirement
    if period_usec < MIN_LED_PERIOD:
        print(f"Warning: Adjusting LED period to {MIN_LED_PERIOD} µs (Min allowed)")
        period_usec = MIN_LED_PERIOD

    # Convert to register format
    register_value = (pulses << 8) | period_usec
    return register, register_value


def set_led_width_offset_register(register, offset_usec, width_usec):
    """Set LED Width and Offset Register (0x30 / 0x35)."""
    
    MIN_LED_OFFSET = 23  # Minimum allowed LED offset
    MAX_LED_WIDTH = 31   # Maximum allowed LED width (5-bit field)

    # Validate LED offset
    if offset_usec < MIN_LED_OFFSET:
        print(f"Warning: Adjusting LED offset to {MIN_LED_OFFSET} µs (Min allowed)")
        offset_usec = MIN_LED_OFFSET
    
    # Validate LED width
    if not (0 <= width_usec <= MAX_LED_WIDTH):
        print(f"Warning: Adjusting LED width to within 0-{MAX_LED_WIDTH} µs range")
        width_usec = min(max(width_usec, 0), MAX_LED_WIDTH)

    # Construct register value
    register_value = ((width_usec & 0x1F) << 8) | (offset_usec & 0xFF)

    return register, register_value

def set_sampling_frequency_register(fsample):
    """Set Sampling Frequency Register (0x12)."""
    if not (0 <= fsample <= 0xFFFF):
        print("Warning: FSAMPLE out of range (0-65535 Hz). Setting to default 0x0028 (40 Hz)")
        fsample = 0x0028  # Default value

    return 0x12, fsample


def calculate_timing(slot_pulses, slot_period, slot_offset):
    """Calculate time slot duration in µs."""
    return slot_offset + (slot_pulses * slot_period)

def configure_adpd1080(slot_a_pulses, slot_a_period_usec, slot_b_pulses, slot_b_period_usec,
                        slot_a_offset, slot_b_offset, slot_a_led_width, slot_b_led_width, fsample, slot_a_led_sel, slot_b_led_sel,
                        clk32k_byp, clk32k_en, clk32k_adjust, mode, slot_a_num_avg, slot_b_num_avg,
                        slot_a_afe_width, slot_a_afe_offset_ns, slot_b_afe_width, slot_b_afe_offset_ns,
                        rdout_mode, fifo_ovrn_prevent, slotb_fifo_mode, slotb_en, slota_fifo_mode, slota_en,
                        led1_coarse, led1_slew, led1_scale, led2_coarse, led2_slew, led2_scale, 
                        led3_coarse, led3_slew, led3_scale, led1_fine, led2_fine, led3_fine,
                        slotb_tia_gain_4, slotb_tia_gain_3, slotb_tia_gain_2, 
                        slota_tia_gain_4, slota_tia_gain_3, slota_tia_gain_2,
                        slota_afe_mode, slota_int_gain, slota_int_as_buf, 
                        slota_tia_ind_en, slota_tia_vref, slota_tia_gain,FIFOThresh,
                        fifo_int_mask,slotb_int_mask,slota_int_mask,
                        gpio1_drv, gpio1_pol, gpio0_ena, gpio0_drv, gpio0_pol
                        
                        
                        ):
    """Configure all ADPD1080 registers including AFE Mode for Slot A."""
    registers = {}

    # Set slot timing registers
    reg, val = set_slot_timing_register(0x31, slot_a_pulses, slot_a_period_usec)
    registers[reg] = val

    reg, val = set_slot_timing_register(0x36, slot_b_pulses, slot_b_period_usec)
    registers[reg] = val

    # Set LED offset registers
    reg, val = set_led_width_offset_register(0x30, slot_a_offset, slot_a_led_width)
    registers[reg] = val

    reg, val = set_led_width_offset_register(0x35, slot_b_offset, slot_b_led_width)
    registers[reg] = val

    # Set LED current registers
    reg, val = set_led_current_registers(led1_coarse, led1_slew, led1_scale, 
                                         led2_coarse, led2_slew, led2_scale, 
                                         led3_coarse, led3_slew, led3_scale, 
                                         led1_fine, led2_fine, led3_fine)
    for i in range(3):
        registers[reg[i]] = val[i]

    # Set sampling frequency
    reg, val = set_sampling_frequency_register(fsample)
    registers[reg] = val

    # Set LED control register
    reg, val = set_led_control_register(slot_a_led_sel, slot_b_led_sel)
    registers[reg] = val

    # Set clock control register (0x4B)
    reg, val = set_clock_control_register(clk32k_byp, clk32k_en, clk32k_adjust)
    registers[reg] = val

    # Set mode register (0x10)
    reg, val = set_mode_register(mode)
    registers[reg] = val

    # Set averaging register (0x15)
    reg, val = set_num_avg_register(slot_a_num_avg, slot_b_num_avg)
    registers[reg] = val

    # Set AFE integration settings
    registers = set_afe_integration(registers, 'A', slot_a_afe_width, slot_a_afe_offset_ns)
    registers = set_afe_integration(registers, 'B', slot_b_afe_width, slot_b_afe_offset_ns)

    # Set register 0x11
    reg, val = set_register_0x11(rdout_mode, fifo_ovrn_prevent, slotb_fifo_mode, slotb_en, slota_fifo_mode, slota_en)
    registers[reg] = val

    # Set TIA gain settings in 0x55
    reg, val = set_tia_gain_register(slotb_tia_gain_4, slotb_tia_gain_3, slotb_tia_gain_2, 
                                     slota_tia_gain_4, slota_tia_gain_3, slota_tia_gain_2)
    registers[reg] = val

    # Set Slot A AFE Mode Register (0x42)
    reg, val = set_slota_afe_mode_register(slota_afe_mode, slota_int_gain, slota_int_as_buf, 
                                           slota_tia_ind_en, slota_tia_vref, slota_tia_gain)
    
    
    registers[reg] = val

    reg, val = generate_fifo_config(FIFOThresh)
    registers[reg] = val
    
    reg, val = set_interrupt_register(fifo_int_mask,slotb_int_mask,slota_int_mask)
    registers[reg] = val
    
    reg, val = set_gpio_config_register(gpio1_drv, gpio1_pol, gpio0_ena, gpio0_drv, gpio0_pol)
    registers[reg] = val
    
    
    
    print(registers)
    return registers




# while True:
#     print("\nEnter ADPD1080 Configuration (Type 'exit' to stop):")
#     try:
#         # Existing Inputs
#         slot_a_pulses = int(input("Enter Slot A Pulse Count (0-255): ").strip())
#         slot_a_period_usec = int(input("Enter Slot A Pulse Period in µs (>= 19): ").strip())
#         slot_a_offset = int(input("Enter Slot A LED Offset in µs (>= 23): ").strip())
#         slot_a_led_width= int(input("Enter Slot A LED Width in µs: ").strip())

#         slot_b_pulses = int(input("Enter Slot B Pulse Count (0-255): ").strip())
#         slot_b_period_usec = int(input("Enter Slot B Pulse Period in µs (>= 19): ").strip())
#         slot_b_offset = int(input("Enter Slot B LED Offset in µs (>= 23): ").strip())
#         slot_b_led_width= int(input("Enter Slot B LED Width in µs: ").strip())

#         fsample = int(input("Enter Sampling Frequency FSAMPLE (16-bit, 0-65535 Hz): ").strip())

#         slot_a_led_sel = int(input("Enter Slot A LED Selection (0-3): ").strip())
#         slot_b_led_sel = int(input("Enter Slot B LED Selection (0-3): ").strip())

#         # New LED Current Inputs
#         led_current_a = int(input("Enter LED Current for Slot A (0-255): ").strip())
#         led_current_b = int(input("Enter LED Current for Slot B (0-255): ").strip())

#         # LED Current Coarse, Slew, and Scale Inputs
#         led1_coarse = int(input("Enter LED1 Coarse Current (0-15): ").strip())
#         led1_slew = int(input("Enter LED1 Slew Rate (0-255): ").strip())
#         led1_scale = int(input("Enter LED1 Scale (0-1): ").strip())
        
#         led2_coarse = int(input("Enter LED2 Coarse Current (0-15): ").strip())
#         led2_slew = int(input("Enter LED2 Slew Rate (0-255): ").strip())
#         led2_scale = int(input("Enter LED2 Scale (0-1): ").strip())
        
#         led3_coarse = int(input("Enter LED3 Coarse Current (0-15): ").strip())
#         led3_slew = int(input("Enter LED3 Slew Rate (0-255): ").strip())
#         led3_scale = int(input("Enter LED3 Scale (0-1): ").strip())

#         # LED Current Fine Tuning Inputs
#         led1_fine = int(input("Enter LED1 Fine Current (0-31): ").strip())
#         led2_fine = int(input("Enter LED2 Fine Current (0-31): ").strip())
#         led3_fine = int(input("Enter LED3 Fine Current (0-31): ").strip())

#         # Clock Control Inputs
#         clk32k_byp = int(input("Enter CLK32K_BYP (0 or 1): ").strip())
#         clk32k_en = int(input("Enter CLK32K_EN (0 or 1): ").strip())
#         clk32k_adjust = int(input("Enter CLK32K_ADJUST (0-63): ").strip())

#         # Mode Selection Input
#         mode = int(input("Enter Device Mode (0: Standby, 1: Program, 2: Normal): ").strip())

#         # Averaging Factors
#         slot_a_num_avg = int(input("Enter Slot A Averaging Factor (0-7): ").strip())
#         slot_b_num_avg = int(input("Enter Slot B Averaging Factor (0-7): ").strip())

#         # AFE Integration Parameters
#         slot_a_afe_width = int(input("Enter Slot A AFE Width (0-31 µs): ").strip())
#         slot_a_afe_offset_ns = int(input("Enter Slot A AFE Offset (0-2047 ns): ").strip())

#         slot_b_afe_width = int(input("Enter Slot B AFE Width (0-31 µs): ").strip())
#         slot_b_afe_offset_ns = int(input("Enter Slot B AFE Offset (0-2047 ns): ").strip())

#         # Register 0x11 Inputs
#         rdout_mode = int(input("Enter Readout Mode (0-3): ").strip())
#         fifo_ovrn_prevent = int(input("Enable FIFO Overrun Prevention? (0: No, 1: Yes): ").strip())
#         slotb_fifo_mode = int(input("Enter Slot B FIFO Mode (0-3): ").strip())
#         slotb_en = int(input("Enable Slot B? (0: No, 1: Yes): ").strip())
#         slota_fifo_mode = int(input("Enter Slot A FIFO Mode (0-3): ").strip())
#         slota_en = int(input("Enable Slot A? (0: No, 1: Yes): ").strip())

#         registers = configure_adpd1080(slot_a_pulses, slot_a_period_usec, slot_b_pulses, slot_b_period_usec,slot_a_offset, slot_b_offset, slot_a_led_width, slot_b_led_width, fsample, slot_a_led_sel, slot_b_led_sel, clk32k_byp, clk32k_en, clk32k_adjust, mode,  slot_a_num_avg, slot_b_num_avg,slot_a_afe_width, slot_a_afe_offset_ns, slot_b_afe_width, slot_b_afe_offset_ns,rdout_mode, fifo_ovrn_prevent, slotb_fifo_mode, slotb_en, slota_fifo_mode, slota_en,led1_coarse, led1_slew, led1_scale,led2_coarse, led2_slew, led2_scale,led3_coarse, led3_slew, led3_scale,led1_fine, led2_fine, led3_fine)
#     except ValueError:
        
#         print("Invalid input! Please enter a valid integer.")
    
#     command = input("Type 'exit' to stop or press Enter to continue: ").strip().lower()
#     if command == "exit":
#         break

# Set default values directly instead of prompting for input
# Slot A settings
slot_a_pulses = 50         # More pulses improve SNR (typical: 16-64)
slot_a_period_usec = 20    # Higher period to allow complete LED settling
slot_a_offset = 25         # Close to the minimum required offset (23µs)
slot_a_led_width = 5      # Slightly wider pulse width for better energy absorption

# Slot B settings
slot_b_pulses = 50         # Same as Slot A for consistency
slot_b_period_usec = 20    # Same as Slot A
slot_b_offset = 25         # Same as Slot A
slot_b_led_width =5     # Same as Slot A

# Frequency settings
fsample = 50   # 100Hz is ideal for PPG (HRV analysis needs 100Hz or higher)

# LED selection settings
slot_a_led_sel  = 3      # Use LED1 for Slot A
slot_b_led_sel  = 3        # Use LED2 for Slot B

# These values won't be used in the corrected function
led_current_a = 155       # Default LED current for Slot A
led_current_b = 155       # Default LED current for Slot B

# LED1 settings
led1_coarse = 7          # Midrange coarse current (0-15)
led1_slew = 0             # Moderate slew rate (0-7)
led1_scale = 1            # 100% scale factor

# LED2 settings
led2_coarse = 7          # Midrange coarse current (0-15)
led2_slew = 0             # Moderate slew rate (0-7)
led2_scale = 1            # 100% scale factor

# LED3 settings
led3_coarse = 7           # Midrange coarse current (0-15)
led3_slew =0             # Moderate slew rate (0-7)
led3_scale = 1            # 100% scale factor

# Fine current adjustment
led1_fine = 31            # Midrange fine current (0-31)
led2_fine = 31            # Midrange fine current (0-31)
led3_fine = 31            # Midrange fine current (0-31)

# Clock control settings
clk32k_byp = 0            # Normal clock operation (not bypassed)
clk32k_en = 1             # Enable 32kHz clock
clk32k_adjust = 0        # Default clock adjustment value

# Mode setting
mode = 1                  # Normal operating mode

# Averaging settings
slot_a_num_avg = 0        # 8 samples average for Slot A
slot_b_num_avg = 0        # 8 samples average for Slot B

# AFE integration settings
slot_a_afe_width = 7         # Wider AFE window to capture more signal (default: 11)
slot_a_afe_offset_ns = 15000   # Offset slightly increased for settling time (default: 20000)

slot_b_afe_width = 7          # Match Slot A settings for consistency
slot_b_afe_offset_ns = 15000   # Same as Slot A

# Register 0x11 settings
rdout_mode = 0            # Block sum mode
fifo_ovrn_prevent = 1     # Prevent FIFO overrun
slotb_fifo_mode = 6       # Data channel A
slotb_en = 1              # Enable Slot B
slota_fifo_mode = 6       # Data channel A
slota_en = 1              # Enable Slot A

# Amplifier Gain
slotb_tia_gain_4 = 0;
slotb_tia_gain_3 = 0;
slotb_tia_gain_2 = 0;
slota_tia_gain_4 = 0;
slota_tia_gain_3 = 0;
slota_tia_gain_2 = 0;

# Slot A AFE Mode Register (0x42)
slota_afe_mode = 0x07   # Default value: 0x07
slota_int_gain = 0x0    # Default: RINT = 400 kΩ
slota_int_as_buf = 0x0  # Default: Normal integrator configuration
slota_tia_ind_en = 0x0  # Default: Disable TIA gain individual setting
slota_tia_vref = 0x0    # Default: 1.27V (Recommended)
slota_tia_gain = 0x2    # Default: 200 kΩ

FIFOThresh=55

fifo_int_mask=0    #0 for enable
slotb_int_mask=0   #1 for enable
slota_int_mask=0

gpio1_drv = 1 
gpio1_pol = 1 
gpio0_ena = 1 
gpio0_drv = 1 
gpio0_pol = 1


# Call the function with these default 
registers = configure_adpd1080(
    slot_a_pulses, slot_a_period_usec, slot_b_pulses, slot_b_period_usec,
    slot_a_offset, slot_b_offset, slot_a_led_width, slot_b_led_width, 
    fsample, slot_a_led_sel, slot_b_led_sel,
    clk32k_byp, clk32k_en, clk32k_adjust, mode, 
    slot_a_num_avg, slot_b_num_avg,
    slot_a_afe_width, slot_a_afe_offset_ns, slot_b_afe_width, slot_b_afe_offset_ns,
    rdout_mode, fifo_ovrn_prevent, slotb_fifo_mode, slotb_en, slota_fifo_mode, slota_en,
    led1_coarse, led1_slew, led1_scale, 
    led2_coarse, led2_slew, led2_scale, 
    led3_coarse, led3_slew, led3_scale, 
    led1_fine, led2_fine, led3_fine,
    slotb_tia_gain_4, slotb_tia_gain_3, slotb_tia_gain_2, 
    slota_tia_gain_4, slota_tia_gain_3, slota_tia_gain_2,
    slota_afe_mode, slota_int_gain, slota_int_as_buf, 
    slota_tia_ind_en, slota_tia_vref, slota_tia_gain,
    FIFOThresh,
    fifo_int_mask,slotb_int_mask,slota_int_mask,
    gpio1_drv, gpio1_pol, gpio0_ena, gpio0_drv, gpio0_pol
    
)


import serial
import time

# Try to open COM14 for serial communication
try:
    ser = serial.Serial("COM14", baudrate=115200, timeout=1)
    serial_available = True
except Exception as e:
    print(f"⚠️ Could not open serial port: {e}")
    ser = None
    serial_available = False

# Always print and attempt STOP
print("STOP")
if serial_available:
    try:
        ser.write(b"STOP\n")
        time.sleep(0.5)  # 500ms delay
    except Exception as e:
        print(f"⚠️ Serial write failed on STOP: {e}")

print("Configured registers:")
for register_addr, register_value in registers.items():
    if isinstance(register_addr, tuple):
        addr_str = ", ".join([f"0x{addr:02X}" for addr in register_addr])
        if isinstance(register_value, tuple):
            val_str = ", ".join([f"0x{val:04X}" for val in register_value])
            command = f"W [{addr_str}] [{val_str}]\n"
        else:
            command = f"W [{addr_str}] 0x{register_value:04X}\n"
    else:
        command = f"W 0x{register_addr:02X} 0x{register_value:04X}\n"

    print(command.strip())  # Always print the command

    if serial_available:
        try:
            ser.write(command.encode())
            time.sleep(0.2)
        except Exception as e:
            print(f"⚠️ Serial write failed: {e}")

# Always print and attempt START
print("START")
if serial_available:
    try:
        ser.write(b"START\n")
    except Exception as e:
        print(f"⚠️ Serial write failed on START: {e}")

    try:
        ser.close()
    except Exception as e:
        print(f"⚠️ Serial close failed: {e}")
