# Author: M Pieklo
# Date: 11.08.2025
# Project: co_driver.
# License: Opensource

include tools/makefiles/makefile_colors.mk
include tools/makefiles/makefile_info.mk
include tools/makefiles/makefile_dir.mk
include tools/makefiles/makefile_clib.mk
include tools/makefiles/makefile_common.mk

SILENTMODE := yes
USE_SBL := yes
USE_FREERTOS := no
FREERTOS_HEAP := heap_1

NAME := $(OUT_DIR)/TARGET
NAME_STARTUP_FILE := startup_stm32f103c8tx
NAME_LINKER_SCRIPT := STM32F103C8TX_FLASH
NAME_OPENOCD_CFG := stm32f1x
DEVICE := STM32F103xB
SW_FLAG := LORA_E32_RX
MACH := cortex-m3
FLOAT_ABI := soft

include tools/makefiles/makefile_flags.mk

INC := \
	-ICore/MAIN/inc/ \
	-ICore/Flash/inc \
	-ICore/CC1101/inc \
	-Itools/Reuse/inc \
	-IDrivers/STM32F1xx_HAL_Driver/inc/ \
	-IDrivers/CMSIS/Device/ST/STM32F1xx/Include/ \
	-IDrivers/CMSIS/Include/

SRC_CORE_DIRS := Core/MAIN/src Core/Flash/src Core/CC1101/src
SRC_DRIVERS_DIR := Drivers/STM32F1xx_HAL_Driver/src
SRC_SBL := tools/SBL/src

########################################################################################################################

.PHONY: all release

all: check_flags DIR ELF HEX

release : all

include tools/makefiles/makefile_dependencies.mk

include tools/makefiles/target_check_flags.mk
include tools/makefiles/target_chip.mk
include tools/makefiles/target_clean.mk
include tools/makefiles/target_dir.mk
include tools/makefiles/target_doc.mk
include tools/makefiles/target_elf.mk
include tools/makefiles/target_hex.mk
