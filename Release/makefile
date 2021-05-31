################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ..\makefile.init

RM := -del 

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include libraries\TimerOne\subdir.mk
-include core\core\subdir.mk
-include ModBusSlave\subdir.mk
-include DomoBoard\subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ..\makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
D:\ProgramFiles\eclipse\workspace\P11-CPP-master\Release\arduino.ar \

ELF += \
P11-CPP-master.elf \

HEX += \
P11-CPP-master.hex \


# All Target
all: P11-CPP-master

# Tool invocations
P11-CPP-master: $(HEX)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"D:\ProgramFiles\eclipse\arduinoPlugin\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7/bin/avr-size" -A "D:\ProgramFiles\eclipse\workspace\P11-CPP-master\Release/P11-CPP-master.elf"
	@echo 'Finished building target: $@'
	@echo ' '

D:\ProgramFiles\eclipse\workspace\P11-CPP-master\Release\arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"D:\ProgramFiles\eclipse\arduinoPlugin\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7/bin/avr-gcc-ar" rcs     D:\ProgramFiles\eclipse\workspace\P11-CPP-master\Release/arduino.ar $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

P11-CPP-master.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"D:\ProgramFiles\eclipse\arduinoPlugin\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7/bin/avr-gcc" -Wall -Wextra -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega328p   -o "D:\ProgramFiles\eclipse\workspace\P11-CPP-master\Release/P11-CPP-master.elf"   $(LINK_OBJ)    D:\ProgramFiles\eclipse\workspace\P11-CPP-master\Release/arduino.ar  "-LD:\ProgramFiles\eclipse\workspace\P11-CPP-master\Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

P11-CPP-master.hex: $(ELF)
	@echo 'Do all objcopy commands'
	"D:\ProgramFiles\eclipse\arduinoPlugin\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7/bin/avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  "D:\ProgramFiles\eclipse\workspace\P11-CPP-master\Release/P11-CPP-master.elf" "D:\ProgramFiles\eclipse\workspace\P11-CPP-master\Release/P11-CPP-master.eep"
	"D:\ProgramFiles\eclipse\arduinoPlugin\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7/bin/avr-objcopy" -O ihex -R .eeprom  "D:\ProgramFiles\eclipse\workspace\P11-CPP-master\Release/P11-CPP-master.elf" "D:\ProgramFiles\eclipse\workspace\P11-CPP-master\Release/P11-CPP-master.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(PDE_DEPS)$(C_DEPS)$(AR)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(HEX)$(INO_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS)
	-@echo ' '

.PHONY: all clean dependents

-include ..\makefile.targets
