################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
OTP18_oTimer.lss \

FLASH_IMAGE += \
OTP18_oTimer.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: OTP18_oTimer.elf secondary-outputs

# Tool invocations
OTP18_oTimer.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,OTP18_oTimer.map -mmcu=atmega168 -o "OTP18_oTimer.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

OTP18_oTimer.lss: OTP18_oTimer.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S OTP18_oTimer.elf  >"OTP18_oTimer.lss"
	@echo 'Finished building: $@'
	@echo ' '

OTP18_oTimer.hex: OTP18_oTimer.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex OTP18_oTimer.elf  "OTP18_oTimer.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: OTP18_oTimer.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=berkeley -t OTP18_oTimer.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) OTP18_oTimer.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
