################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/digit.c \
../src/input.c \
../src/ipc.c \
../src/main.c \
../src/oTimer.c \
../src/scheduler.c \
../src/time.c 

OBJS += \
./src/digit.o \
./src/input.o \
./src/ipc.o \
./src/main.o \
./src/oTimer.o \
./src/scheduler.o \
./src/time.o 

C_DEPS += \
./src/digit.d \
./src/input.d \
./src/ipc.d \
./src/main.d \
./src/oTimer.d \
./src/scheduler.d \
./src/time.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Sebastian\OTP\OTP-018_oTimer\05_Code\OTP18_oTimer\inc" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega168 -DF_CPU=7143000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


