# Makefile for STM32F051
# 02-06-2014 E. Brombaugh

# sub directories
VPATH = .:CMSIS:StdPeriph

# Object files
OBJECTS =	startup_stm32f051.o system_stm32f0xx.o main.o systick.o led.o \
			lpc.o pwm.o debounce.o \
			stm32f0xx_rcc.o stm32f0xx_gpio.o stm32f0xx_misc.o stm32f0xx_tim.o

# Must define the MCU type
CDEFS=-DSTM32F051 -DUSE_STDPERIPH_DRIVER

# Linker script
LDSCRIPT = STM32F051R8_FLASH.ld

# Compiler Flags
OPTLVL:=s # Optimization level, can be [0, 1, 2, 3, s].
COMMONFLAGS=-O$(OPTLVL) -g -ffunction-sections -std=c99 -Wall
MCUFLAGS=-mthumb -mcpu=cortex-m0
CFLAGS=$(COMMONFLAGS) $(MCUFLAGS) -I. -ICMSIS -IStdPeriph $(CDEFS)
LDFLAGS=$(COMMONFLAGS) $(MCUFLAGS) -fno-exceptions \
	-fdata-sections -nostartfiles -Wl,--gc-sections,-T$(LDSCRIPT),-Map=main.map

# Executables
ARCH = arm-none-eabi
CC = $(ARCH)-gcc
LD = $(ARCH)-ld -v
AS = $(ARCH)-as
OBJCPY = $(ARCH)-objcopy
OBJDMP = $(ARCH)-objdump
GDB = $(ARCH)-gdb
OPENOCD = openocd

CPFLAGS = --output-target=binary
ODFLAGS	= -x --syms

# Targets
all: main.bin

clean:
	-rm -f $(OBJECTS) *.lst *.elf *.bin *.dmp 

flash: main.elf
	$(OPENOCD) -f openocd.cfg -c flash_chip

disassemble: main.elf
	$(OBJDMP) -d main.elf > main.dis
	
dist:
	tar -c *.h *.c *.s Makefile *.cmd *.cfg openocd_doflash | gzip > minimal_hello_world.tar.gz

main.ihex: main.elf
	$(OBJCPY) --output-target=ihex main.elf main.ihex

main.bin: main.elf 
	$(OBJCPY) $(CPFLAGS) main.elf main.bin
	$(OBJDMP) $(ODFLAGS) main.elf > main.dmp
	ls -l main.elf main.bin

main.elf: $(OBJECTS) $(LDSCRIPT)
	$(CC) -o main.elf $(LDFLAGS) $(OBJECTS) -lnosys

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

