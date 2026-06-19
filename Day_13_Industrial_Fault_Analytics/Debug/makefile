################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
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

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Day_13_Industrial_Fault_Analytics
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Day_13_Industrial_Fault_Analytics.elf \

MAP_FILES += \
Day_13_Industrial_Fault_Analytics.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Day_13_Industrial_Fault_Analytics.list \

OBJCOPY_BIN += \
Day_13_Industrial_Fault_Analytics.bin \


# All Target
all: main-build

# Main-build Target
main-build: Day_13_Industrial_Fault_Analytics.elf secondary-outputs

# Tool invocations
Day_13_Industrial_Fault_Analytics.elf Day_13_Industrial_Fault_Analytics.map: $(OBJS) $(USER_OBJS) C:\Users\91953\OneDrive\Desktop\workspace\Day_13_Industrial_Fault_Analytics\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Day_13_Industrial_Fault_Analytics.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\91953\OneDrive\Desktop\workspace\Day_13_Industrial_Fault_Analytics\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Day_13_Industrial_Fault_Analytics.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Day_13_Industrial_Fault_Analytics.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Day_13_Industrial_Fault_Analytics.list"
	@echo 'Finished building: $@'
	@echo ' '

Day_13_Industrial_Fault_Analytics.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Day_13_Industrial_Fault_Analytics.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Day_13_Industrial_Fault_Analytics.bin Day_13_Industrial_Fault_Analytics.elf Day_13_Industrial_Fault_Analytics.list Day_13_Industrial_Fault_Analytics.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
