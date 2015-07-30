PROJ_NAME = main

SRCS = ./Src/main.c  ./Src/stm32f0xx_it.c ./Src/system_stm32f0xx.c

COMMON = ../common/

STARTUP = $(COMMON)Lib/STM32Cube_FW_F0_V1.2.0/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f030x6.s
LINKER_FILE = ./STM32F030F4_FLASH.ld

INCLUDE = -I./Inc
DEFINES =
#MCU Specific Stuff
MCU = STM32F030x6
DEFINES += -D$(MCU)
include $(COMMON)Makefile.common
