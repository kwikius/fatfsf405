
TOOLCHAIN_PREFIX = /home/andy/andy_bin/gcc-arm-none-eabi-4_9-2015q3/

QUAN_INCLUDE_PATH = /home/andy/cpp/projects/quan-trunk/
# Change this to your version of gcc.
# (You can find the gcc version by invoking arm-noe-eabi-gcc --version in the $(TOOLCHAIN_PREFIX)/bin/ directory)
#TOOLCHAIN_GCC_VERSION = 4.7.4
#TOOLCHAIN_GCC_VERSION = 5.2.1
TOOLCHAIN_GCC_VERSION = 4.9.3
INIT_LIB_PREFIX = $(TOOLCHAIN_PREFIX)/lib/gcc/arm-none-eabi/$(TOOLCHAIN_GCC_VERSION)/armv7e-m/fpu/

CXX     = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CC      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-gcc
LD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-g++
CP      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objcopy
OD      = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-objdump
SIZ     = $(TOOLCHAIN_PREFIX)bin/arm-none-eabi-size

STM32F4_LINKER_SCRIPT = system/stm32f4.ld

INCLUDES = -I/home/andy/cpp/projects/fat_fs/Inc/  \
 -I/home/andy/cpp/projects/fat_fs/Drivers/STM32F4xx_HAL_Driver/Inc/ \
-I/home/andy/cpp/projects/fat_fs/Drivers/CMSIS/Device/ST/STM32F4xx/Include/ \
-I/home/andy/cpp/projects/fat_fs/Drivers/CMSIS/Include/ \
-I/home/andy/cpp/projects/fat_fs/Middlewares/Third_Party/FatFs/src/ \
-I/home/andy/cpp/projects/fat_fs/Middlewares/Third_Party/FatFs/src/drivers/ \
-I$(QUAN_INCLUDE_PATH)

CFLAG_EXTRAS = -fno-math-errno -DQUAN_STM32F4 -DQUAN_NO_EXCEPTIONS -DSTM32F405xx -DSTM32F40_41xxx -DQUAN_SYSTICK_TIMER_UINT32 -DHSE_VALUE=8000000

# CFLAG_EXTRAS += -DSTM32F4_TEST_AERFLITE_BOARD
#CFLAG_EXTRAS += -DSTM32F4_TEST_QUANTRACKER_BOARD
#CFLAG_EXTRAS += -Wl,-u,vsprintf -lm
#CFLAG_EXTRAS += -DDEBUG
CFLAG_EXTRAS += -Wall -Wno-unused-local-typedefs
CFLAG_EXTRAS += -fmax-errors=1

PROCESSOR_FLAGS = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard

STARTUP = startup.s

OBJDIR = ./obj/

BINDIR = ./bin/

CPFLAGS = -Obinary
ODFLAGS = -d

CPLUSPLUSFLAGS = -std=c++11 -fno-rtti -fno-exceptions
CFLAGS =  -c -Os -g  $(INCLUDES) $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) -fdata-sections -ffunction-sections

INIT_LIBS = $(INIT_LIB_PREFIX)crti.o $(INIT_LIB_PREFIX)crtn.o

LFLAGS = -T$(STM32F4_LINKER_SCRIPT) -Os $(PROCESSOR_FLAGS) $(CFLAG_EXTRAS) $(INIT_LIBS) -nodefaultlibs -nostartfiles \
--specs=nano.specs -Wl,--gc-sections

Src_objects =  $(patsubst %,$(OBJDIR)%, bsp_driver_sd.o  fatfs.o  main.o  stm32f4xx_hal_msp.o  stm32f4xx_it.o spbrk.o)

system_objects = $(patsubst %,$(OBJDIR)%, system_init.o setup.o)

fatfs_src_objects = $(patsubst %,$(OBJDIR)%, diskio.o ff.o ff_gen_drv.o)

fatfs_driver_objects = $(patsubst %,$(OBJDIR)%, sd_diskio.o)

stm32_objects  = $(patsubst %,$(OBJDIR)%, stm32f4xx_hal.o stm32f4xx_hal_cortex.o \
  stm32f4xx_hal_gpio.o stm32f4xx_hal_rcc.o stm32f4xx_hal_sd.o stm32f4xx_hal_tim.o \
  stm32f4xx_hal_tim_ex.o stm32f4xx_hal_uart.o stm32f4xx_hal_dma.o stm32f4xx_ll_sdmmc.o )

objects = $(Src_objects) $(system_objects) $(fatfs_src_objects) $(fatfs_driver_objects) \
 $(OBJDIR)startup.o $(stm32_objects) 

all: test

test: $(BINDIR)main.elf
	@ echo "...copying"
	$(CP) $(CPFLAGS)  $(BINDIR)main.elf  $(BINDIR)main.bin
	$(OD) $(ODFLAGS)  $(BINDIR)main.elf >  $(BINDIR)main.lst
	$(SIZ) -A  $(BINDIR)main.elf

$(BINDIR)main.elf : $(objects)
	@ echo "..linking"
	$(LD) $(LFLAGS) $(objects) -o $(BINDIR)main.elf 

$(stm32_objects) : $(OBJDIR)%.o : Drivers/STM32F4xx_HAL_Driver/Src/%.c
	$(CC) $(CFLAGS) $< -o $@

$(fatfs_src_objects) : $(OBJDIR)%.o : Middlewares/Third_Party/FatFs/src/%.c
	$(CC) $(CFLAGS) $< -o $@

$(fatfs_driver_objects) : $(OBJDIR)%.o : Middlewares/Third_Party/FatFs/src/drivers/%.c
	$(CC) $(CFLAGS) $< -o $@

$(Src_objects) : $(OBJDIR)%.o : Src/%.c
	$(CC) $(CFLAGS) $< -o $@

$(system_objects): $(OBJDIR)%.o : system/%.cpp
	$(CXX) $(CFLAGS) $(CPLUSPLUSFLAGS) $< -o $@

$(OBJDIR)startup.o: system/$(STARTUP)
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -rf $(OBJDIR)*.o $(BINDIR)*.elf $(BINDIR)*.bin $(BINDIR)*.lst
