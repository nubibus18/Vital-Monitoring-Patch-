################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ADPD1080.c \
../Core/Src/LSM6DSL.c \
../Core/Src/app_debug.c \
../Core/Src/app_entry.c \
../Core/Src/gpio.c \
../Core/Src/hw_timerserver.c \
../Core/Src/i2c.c \
../Core/Src/ipcc.c \
../Core/Src/main.c \
../Core/Src/memorymap.c \
../Core/Src/rf.c \
../Core/Src/rtc.c \
../Core/Src/stm32_lpm_if.c \
../Core/Src/stm32wbxx_hal_msp.c \
../Core/Src/stm32wbxx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32wbxx.c 

OBJS += \
./Core/Src/ADPD1080.o \
./Core/Src/LSM6DSL.o \
./Core/Src/app_debug.o \
./Core/Src/app_entry.o \
./Core/Src/gpio.o \
./Core/Src/hw_timerserver.o \
./Core/Src/i2c.o \
./Core/Src/ipcc.o \
./Core/Src/main.o \
./Core/Src/memorymap.o \
./Core/Src/rf.o \
./Core/Src/rtc.o \
./Core/Src/stm32_lpm_if.o \
./Core/Src/stm32wbxx_hal_msp.o \
./Core/Src/stm32wbxx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32wbxx.o 

C_DEPS += \
./Core/Src/ADPD1080.d \
./Core/Src/LSM6DSL.d \
./Core/Src/app_debug.d \
./Core/Src/app_entry.d \
./Core/Src/gpio.d \
./Core/Src/hw_timerserver.d \
./Core/Src/i2c.d \
./Core/Src/ipcc.d \
./Core/Src/main.d \
./Core/Src/memorymap.d \
./Core/Src/rf.d \
./Core/Src/rtc.d \
./Core/Src/stm32_lpm_if.d \
./Core/Src/stm32wbxx_hal_msp.d \
./Core/Src/stm32wbxx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32wbxx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB5Mxx -c -I../Core/Inc -I../STM32_WPAN/App -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/STM32WBxx_HAL_Driver/Inc -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Utilities/lpm/tiny_lpm -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/utilities -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/core -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/core/auto -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/core/template -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/svc/Inc -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/svc/Src -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/CMSIS/Device/ST/STM32WBxx/Include -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Utilities/sequencer -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ADPD1080.cyclo ./Core/Src/ADPD1080.d ./Core/Src/ADPD1080.o ./Core/Src/ADPD1080.su ./Core/Src/LSM6DSL.cyclo ./Core/Src/LSM6DSL.d ./Core/Src/LSM6DSL.o ./Core/Src/LSM6DSL.su ./Core/Src/app_debug.cyclo ./Core/Src/app_debug.d ./Core/Src/app_debug.o ./Core/Src/app_debug.su ./Core/Src/app_entry.cyclo ./Core/Src/app_entry.d ./Core/Src/app_entry.o ./Core/Src/app_entry.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/hw_timerserver.cyclo ./Core/Src/hw_timerserver.d ./Core/Src/hw_timerserver.o ./Core/Src/hw_timerserver.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/ipcc.cyclo ./Core/Src/ipcc.d ./Core/Src/ipcc.o ./Core/Src/ipcc.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/memorymap.cyclo ./Core/Src/memorymap.d ./Core/Src/memorymap.o ./Core/Src/memorymap.su ./Core/Src/rf.cyclo ./Core/Src/rf.d ./Core/Src/rf.o ./Core/Src/rf.su ./Core/Src/rtc.cyclo ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/rtc.su ./Core/Src/stm32_lpm_if.cyclo ./Core/Src/stm32_lpm_if.d ./Core/Src/stm32_lpm_if.o ./Core/Src/stm32_lpm_if.su ./Core/Src/stm32wbxx_hal_msp.cyclo ./Core/Src/stm32wbxx_hal_msp.d ./Core/Src/stm32wbxx_hal_msp.o ./Core/Src/stm32wbxx_hal_msp.su ./Core/Src/stm32wbxx_it.cyclo ./Core/Src/stm32wbxx_it.d ./Core/Src/stm32wbxx_it.o ./Core/Src/stm32wbxx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32wbxx.cyclo ./Core/Src/system_stm32wbxx.d ./Core/Src/system_stm32wbxx.o ./Core/Src/system_stm32wbxx.su

.PHONY: clean-Core-2f-Src

