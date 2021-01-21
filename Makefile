# Project specific
TARGET_NAME = main
SRC_DIR = src/
INC_DIR = include/
BUILD_DIR = build/
VENDOR_ROOT = ./STM32CubeF4

# Toolchain
CC = arm-none-eabi-gcc
DB = arm-none-eabi-gdb
CP = arm-none-eabi-objcopy

# Project sources
SRC_FILES = $(wildcard $(SRC_DIR)*.c) $(wildcard $(SRC_DIR)*/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)*.s) $(wildcard $(SRC_DIR)*/*.s)
LD_SCRIPT = $(SRC_DIR)/device/STM32F429ZITx_FLASH.ld

# Project includes
INCLUDES   = -I$(INC_DIR)

# Vendor sources
SRC_FILES += $(VENDOR_ROOT)/Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_lcd.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_ts.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/BSP/STM32F429I-Discovery/stm32f429i_discovery_sdram.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/BSP/Components/ili9341/ili9341.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/BSP/Components/stmpe811/stmpe811.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma2d.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c
SRC_FILES += $(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
SRC_FILES += $(VENDOR_ROOT)/Middlewares/Third_Party/FatFS/src/ff_gen_drv.c
SRC_FILES += $(VENDOR_ROOT)/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
SRC_FILES += $(VENDOR_ROOT)/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
SRC_FILES += $(VENDOR_ROOT)/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
SRC_FILES += $(VENDOR_ROOT)/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c

# Vendor includes
INCLUDES += -I$(VENDOR_ROOT)/Drivers/CMSIS/Core/Include
INCLUDES += -I$(VENDOR_ROOT)/Drivers/CMSIS/Device/ST/STM32F4xx/Include
INCLUDES += -I$(VENDOR_ROOT)/Drivers/STM32F4xx_HAL_Driver/Inc
INCLUDES += -I$(VENDOR_ROOT)/Drivers/BSP/STM32F429I-Discovery
INCLUDES += -I$(VENDOR_ROOT)/Drivers/BSP/Components/ili9341
INCLUDES += -I$(VENDOR_ROOT)/Drivers/BSP/Components/stmpe811
INCLUDES += -I$(VENDOR_ROOT)/Middlewares/Third_Party/FatFS/src
INCLUDES += -I$(VENDOR_ROOT)/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
INCLUDES += -I$(VENDOR_ROOT)/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc

# Compiler Flags
CFLAGS  = -g -O0 -Wall -Wextra -Warray-bounds
CFLAGS += -mcpu=cortex-m4 -mthumb -mlittle-endian -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -DUSE_HAL_DRIVER -DUSE_STM32F429I_DISCO -DSTM32F429xx -DUSE_USB_OTG_HS -DUSE_EMBEDDED_PHY
CFLAGS += $(INCLUDES)

# Linker Flags
LFLAGS = -Wl,--gc-sections -Wl,-T$(LD_SCRIPT) --specs=rdimon.specs

###############################################################################

# This does an in-source build. An out-of-source build that places all object
# files into BUILD_DIR would be a better solution, but the goal was to keep
# this file very simple.

CXX_OBJS = $(SRC_FILES:.c=.o)
ASM_OBJS = $(ASM_FILES:.s=.o)
ALL_OBJS = $(ASM_OBJS) $(CXX_OBJS)
TARGET_ELF = $(BUILD_DIR)$(TARGET_NAME).elf
TARGET_BIN = $(BUILD_DIR)$(TARGET_NAME).bin

.PHONY: clean debug

all: $(BUILD_DIR) $(TARGET_ELF) $(TARGET_BIN)

# Compile
$(CXX_OBJS): %.o: %.c
$(ASM_OBJS): %.o: %.s
$(ALL_OBJS):
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(TARGET_ELF): $(ALL_OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $(ALL_OBJS) -o $@

# Make .bin
$(TARGET_BIN): $(TARGET_ELF)
	$(CP) -O binary -S $< $@

# Make build directory
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Clean
clean:
	rm -f $(ALL_OBJS)
	rm -rf $(BUILD_DIR)

# Debug
debug:
	$(DB) $(TARGET_ELF)
