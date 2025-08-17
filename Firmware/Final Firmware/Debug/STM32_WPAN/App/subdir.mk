################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32_WPAN/App/app_ble.c \
../STM32_WPAN/App/custom_app.c \
../STM32_WPAN/App/custom_stm.c 

OBJS += \
./STM32_WPAN/App/app_ble.o \
./STM32_WPAN/App/custom_app.o \
./STM32_WPAN/App/custom_stm.o 

C_DEPS += \
./STM32_WPAN/App/app_ble.d \
./STM32_WPAN/App/custom_app.d \
./STM32_WPAN/App/custom_stm.d 


# Each subdirectory must supply rules for building sources it contributes
STM32_WPAN/App/%.o STM32_WPAN/App/%.su STM32_WPAN/App/%.cyclo: ../STM32_WPAN/App/%.c STM32_WPAN/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB5Mxx -c -I../Core/Inc -I../STM32_WPAN/App -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/STM32WBxx_HAL_Driver/Inc -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Utilities/lpm/tiny_lpm -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/utilities -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/core -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/core/auto -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/core/template -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/svc/Inc -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble/svc/Src -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/CMSIS/Device/ST/STM32WBxx/Include -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Utilities/sequencer -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Middlewares/ST/STM32_WPAN/ble -IC:/Users/36dhe/STM32Cube/Repository/STM32Cube_FW_WB_V1.21.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-STM32_WPAN-2f-App

clean-STM32_WPAN-2f-App:
	-$(RM) ./STM32_WPAN/App/app_ble.cyclo ./STM32_WPAN/App/app_ble.d ./STM32_WPAN/App/app_ble.o ./STM32_WPAN/App/app_ble.su ./STM32_WPAN/App/custom_app.cyclo ./STM32_WPAN/App/custom_app.d ./STM32_WPAN/App/custom_app.o ./STM32_WPAN/App/custom_app.su ./STM32_WPAN/App/custom_stm.cyclo ./STM32_WPAN/App/custom_stm.d ./STM32_WPAN/App/custom_stm.o ./STM32_WPAN/App/custom_stm.su

.PHONY: clean-STM32_WPAN-2f-App

