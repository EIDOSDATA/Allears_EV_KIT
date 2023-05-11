################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/stim_lib.c \
../Src/stim_lib_common.c \
../Src/stim_lib_current.c \
../Src/stim_lib_error.c \
../Src/stim_lib_init.c \
../Src/stim_lib_st_inc.c \
../Src/stim_lib_st_inc_st.c \
../Src/stim_lib_st_init.c \
../Src/stim_lib_st_msp.c \
../Src/stim_lib_state.c \
../Src/stim_lib_stim_cfg.c \
../Src/stim_lib_stim_cfg_st.c \
../Src/stim_lib_volt.c \
../Src/stim_lib_volt_st.c \
../Src/stim_lib_volt_table.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/stim_lib.o \
./Src/stim_lib_common.o \
./Src/stim_lib_current.o \
./Src/stim_lib_error.o \
./Src/stim_lib_init.o \
./Src/stim_lib_st_inc.o \
./Src/stim_lib_st_inc_st.o \
./Src/stim_lib_st_init.o \
./Src/stim_lib_st_msp.o \
./Src/stim_lib_state.o \
./Src/stim_lib_stim_cfg.o \
./Src/stim_lib_stim_cfg_st.o \
./Src/stim_lib_volt.o \
./Src/stim_lib_volt_st.o \
./Src/stim_lib_volt_table.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/stim_lib.d \
./Src/stim_lib_common.d \
./Src/stim_lib_current.d \
./Src/stim_lib_error.d \
./Src/stim_lib_init.d \
./Src/stim_lib_st_inc.d \
./Src/stim_lib_st_inc_st.d \
./Src/stim_lib_st_init.d \
./Src/stim_lib_st_msp.d \
./Src/stim_lib_state.d \
./Src/stim_lib_stim_cfg.d \
./Src/stim_lib_stim_cfg_st.d \
./Src/stim_lib_volt.d \
./Src/stim_lib_volt_st.d \
./Src/stim_lib_volt_table.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DTD_DEBUG_ENABLED -DEVKIT_CC -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I"C:/Users/eidos/GitHub/Allears_EV_KIT/allears_EVK_LIB/Inc/Inc_prv" -I"C:/Users/eidos/GitHub/Allears_EV_KIT/allears_EVK_LIB/Inc/Inc_pub" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/stim_lib.cyclo ./Src/stim_lib.d ./Src/stim_lib.o ./Src/stim_lib.su ./Src/stim_lib_common.cyclo ./Src/stim_lib_common.d ./Src/stim_lib_common.o ./Src/stim_lib_common.su ./Src/stim_lib_current.cyclo ./Src/stim_lib_current.d ./Src/stim_lib_current.o ./Src/stim_lib_current.su ./Src/stim_lib_error.cyclo ./Src/stim_lib_error.d ./Src/stim_lib_error.o ./Src/stim_lib_error.su ./Src/stim_lib_init.cyclo ./Src/stim_lib_init.d ./Src/stim_lib_init.o ./Src/stim_lib_init.su ./Src/stim_lib_st_inc.cyclo ./Src/stim_lib_st_inc.d ./Src/stim_lib_st_inc.o ./Src/stim_lib_st_inc.su ./Src/stim_lib_st_inc_st.cyclo ./Src/stim_lib_st_inc_st.d ./Src/stim_lib_st_inc_st.o ./Src/stim_lib_st_inc_st.su ./Src/stim_lib_st_init.cyclo ./Src/stim_lib_st_init.d ./Src/stim_lib_st_init.o ./Src/stim_lib_st_init.su ./Src/stim_lib_st_msp.cyclo ./Src/stim_lib_st_msp.d ./Src/stim_lib_st_msp.o ./Src/stim_lib_st_msp.su ./Src/stim_lib_state.cyclo ./Src/stim_lib_state.d ./Src/stim_lib_state.o ./Src/stim_lib_state.su ./Src/stim_lib_stim_cfg.cyclo ./Src/stim_lib_stim_cfg.d ./Src/stim_lib_stim_cfg.o ./Src/stim_lib_stim_cfg.su ./Src/stim_lib_stim_cfg_st.cyclo ./Src/stim_lib_stim_cfg_st.d ./Src/stim_lib_stim_cfg_st.o ./Src/stim_lib_stim_cfg_st.su ./Src/stim_lib_volt.cyclo ./Src/stim_lib_volt.d ./Src/stim_lib_volt.o ./Src/stim_lib_volt.su ./Src/stim_lib_volt_st.cyclo ./Src/stim_lib_volt_st.d ./Src/stim_lib_volt_st.o ./Src/stim_lib_volt_st.su ./Src/stim_lib_volt_table.cyclo ./Src/stim_lib_volt_table.d ./Src/stim_lib_volt_table.o ./Src/stim_lib_volt_table.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

