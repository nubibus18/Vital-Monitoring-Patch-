################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Utilities/lpm/tiny_lpm/stm32_lpm.c \
C:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Utilities/sequencer/stm32_seq.c 

OBJS += \
./Utilities/stm32_lpm.o \
./Utilities/stm32_seq.o 

C_DEPS += \
./Utilities/stm32_lpm.d \
./Utilities/stm32_seq.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/stm32_lpm.o: C:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Utilities/lpm/tiny_lpm/stm32_lpm.c Utilities/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB5Mxx -c -I../Core/Inc -I../STM32_WPAN/App -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/STM32WBxx_HAL_Driver/Inc -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Utilities/lpm/tiny_lpm -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/utilities -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/core -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/core/auto -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/core/template -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/svc/Inc -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/svc/Src -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/CMSIS/Device/ST/STM32WBxx/Include -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Utilities/sequencer -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Utilities/stm32_seq.o: C:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Utilities/sequencer/stm32_seq.c Utilities/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB5Mxx -c -I../Core/Inc -I../STM32_WPAN/App -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/STM32WBxx_HAL_Driver/Inc -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Utilities/lpm/tiny_lpm -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/utilities -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/core -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/core/auto -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/core/template -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/svc/Inc -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/svc/Src -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/CMSIS/Device/ST/STM32WBxx/Include -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Utilities/sequencer -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Utilities

clean-Utilities:
	-$(RM) ./Utilities/stm32_lpm.cyclo ./Utilities/stm32_lpm.d ./Utilities/stm32_lpm.o ./Utilities/stm32_lpm.su ./Utilities/stm32_seq.cyclo ./Utilities/stm32_seq.d ./Utilities/stm32_seq.o ./Utilities/stm32_seq.su

.PHONY: clean-Utilities

