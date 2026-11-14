################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Saile/Desktop/Github/elo312_2026/MEMS/Drivers/BSP/Components/lps22hb/lps22hb.c \
C:/Users/Saile/Desktop/Github/elo312_2026/MEMS/Drivers/BSP/Components/lps22hb/lps22hb_reg.c 

OBJS += \
./lps22hb/lps22hb.o \
./lps22hb/lps22hb_reg.o 

C_DEPS += \
./lps22hb/lps22hb.d \
./lps22hb/lps22hb_reg.d 


# Each subdirectory must supply rules for building sources it contributes
lps22hb/lps22hb.o: C:/Users/Saile/Desktop/Github/elo312_2026/MEMS/Drivers/BSP/Components/lps22hb/lps22hb.c lps22hb/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Saile/Desktop/Github/Lab_Estructura_Computadores/teclado/Drivers/BSP" -I"C:/Users/Saile/Desktop/Github/Lab_Estructura_Computadores/teclado/X-CUBE-MEMS1" -I"C:/Users/Saile/Desktop/Github/Lab_Estructura_Computadores/teclado/Drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
lps22hb/lps22hb_reg.o: C:/Users/Saile/Desktop/Github/elo312_2026/MEMS/Drivers/BSP/Components/lps22hb/lps22hb_reg.c lps22hb/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Saile/Desktop/Github/Lab_Estructura_Computadores/teclado/Drivers/BSP" -I"C:/Users/Saile/Desktop/Github/Lab_Estructura_Computadores/teclado/X-CUBE-MEMS1" -I"C:/Users/Saile/Desktop/Github/Lab_Estructura_Computadores/teclado/Drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lps22hb

clean-lps22hb:
	-$(RM) ./lps22hb/lps22hb.cyclo ./lps22hb/lps22hb.d ./lps22hb/lps22hb.o ./lps22hb/lps22hb.su ./lps22hb/lps22hb_reg.cyclo ./lps22hb/lps22hb_reg.d ./lps22hb/lps22hb_reg.o ./lps22hb/lps22hb_reg.su

.PHONY: clean-lps22hb

