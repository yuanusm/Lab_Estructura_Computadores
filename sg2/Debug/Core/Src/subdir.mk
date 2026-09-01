################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Actividad3.c \
../Core/Src/MaquinaDeEstado.c \
../Core/Src/gpio.c \
../Core/Src/main.c \
../Core/Src/main_actividad3.c \
../Core/Src/main_tomas.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/template_1.c 

OBJS += \
./Core/Src/Actividad3.o \
./Core/Src/MaquinaDeEstado.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/main_actividad3.o \
./Core/Src/main_tomas.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/template_1.o 

C_DEPS += \
./Core/Src/Actividad3.d \
./Core/Src/MaquinaDeEstado.d \
./Core/Src/gpio.d \
./Core/Src/main.d \
./Core/Src/main_actividad3.d \
./Core/Src/main_tomas.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/template_1.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32L4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Actividad3.cyclo ./Core/Src/Actividad3.d ./Core/Src/Actividad3.o ./Core/Src/Actividad3.su ./Core/Src/MaquinaDeEstado.cyclo ./Core/Src/MaquinaDeEstado.d ./Core/Src/MaquinaDeEstado.o ./Core/Src/MaquinaDeEstado.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/main_actividad3.cyclo ./Core/Src/main_actividad3.d ./Core/Src/main_actividad3.o ./Core/Src/main_actividad3.su ./Core/Src/main_tomas.cyclo ./Core/Src/main_tomas.d ./Core/Src/main_tomas.o ./Core/Src/main_tomas.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su ./Core/Src/template_1.cyclo ./Core/Src/template_1.d ./Core/Src/template_1.o ./Core/Src/template_1.su

.PHONY: clean-Core-2f-Src

