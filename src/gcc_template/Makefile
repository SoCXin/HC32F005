######################################
# target
######################################
TARGET = Template


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
src/main.c \
src/system_hc32f005.c

# FreeRTOS sources
C_SOURCES += \
$(wildcard lib/FreeRTOS/*.c) \
$(wildcard lib/FreeRTOS/portable/GCC/ARM_CM0/*.c) \
$(wildcard lib/FreeRTOS/portable/MemMang/*.c)

# StdPeri sources
C_SOURCES += \
$(wildcard lib/standard_peripherals/src/*.c)

# ASM sources
ASM_SOURCES =  \
startup_hc32f005.s


######################################
# lib library
######################################
PERIFLIB_SOURCES =


#######################################
# binaries
#######################################
# BINPATH = e:/gcc-arm-none-eabi-7-2018-q2-update-win32/bin
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc.exe
AS = $(PREFIX)gcc.exe -x assembler-with-cpp
CP = $(PREFIX)objcopy.exe
AR = $(PREFIX)ar.exe
SZ = $(PREFIX)size.exe
GDB = $(PREFIX)gdb.exe
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m0plus

# fpu
# FPU = -mfpu=fpv4-sp-d16

# float-abi
# FLOAT-ABI = -mfloat-abi=hard
FLOAT-ABI = -mfloat-abi=soft

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS = \
-DHC32F005


# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES =  \
-Iinclude \
-Ilib/CMSIS/Core/Include

# FreeRTOS includes
C_INCLUDES += \
-Ilib/FreeRTOS/include \
-Ilib/FreeRTOS/portable/GCC/ARM_CM0

# StdPeri includes
C_INCLUDES += \
-Ilib/standard_peripherals/include

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = hc32f005_flash.ld

# libraries
LIBS = -lc -lm -lnosys -specs=nano.specs
LIBDIR =
LDFLAGS = $(MCU) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)

#######################################
# debug
#######################################
debug: all
	gdbgui -g $(GDB) \
	--gdb-args "\
	--symbol=$(BUILD_DIR)/$(TARGET).elf \
	-ex 'target remote localhost:3333' \
	-ex 'monitor program $(BUILD_DIR)/$(TARGET).elf' \
	-ex 'monitor reset halt' \
	-ex 'b main'\
	"

#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
