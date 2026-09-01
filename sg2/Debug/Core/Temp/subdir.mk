################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Temp/template_2.c 

OBJS += \
./Core/Temp/template_2.o 

C_DEPS += \
./Core/Temp/template_2.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Temp/%.o Core/Temp/%.su Core/Temp/%.cyclo: ../Core/Temp/%.c Core/Temp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32L4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Temp

clean-Core-2f-Temp:
	-$(RM) ./Core/Temp/template_2.cyclo ./Core/Temp/template_2.d ./Core/Temp/template_2.o ./Core/Temp/template_2.su

.PHONY: clean-Core-2f-Temp

