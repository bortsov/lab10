#GNU_ARM_TOOLS=Y:/GNU Tools ARM Embedded/8-2018-q4-major

GCC_PREFIX=arm-none-eabi

AS         =$(GNU_ARM_TOOLS)/bin/$(GCC_PREFIX)-as
CC         =$(GNU_ARM_TOOLS)/bin/$(GCC_PREFIX)-gcc
CXX        =$(GNU_ARM_TOOLS)/bin/$(GCC_PREFIX)-g++
AR         =$(GNU_ARM_TOOLS)/bin/$(GCC_PREFIX)-ar
RANLIB     =$(GNU_ARM_TOOLS)/bin/$(GCC_PREFIX)-ranlib
LD         =$(GNU_ARM_TOOLS)/bin/$(GCC_PREFIX)-g++
FROMELF    =$(GNU_ARM_TOOLS)/bin/$(GCC_PREFIX)-objcopy
SIZE       =$(GNU_ARM_TOOLS)/bin/$(GCC_PREFIX)-size

# CPU architecture
CPU =cortex-r4f

# ARM/THUMB mode
CPU_MODE =thumb

TARGET_ARCH =-mcpu=$(CPU) -m$(CPU_MODE)

# уровень оптимизации, передаваемый компляторам и линкеру
ifeq "$(MAKECMDGOALS)" "debug"
OPTFLAGS =-O0
else
OPTFLAGS =-O3 -flto
endif

COMMON_FLAGS =\
 -fdiagnostics-color=always\
 -fmessage-length=0\
 -fno-builtin\
 -Wall\
 -Wmissing-declarations\
 -Wno-comment\
 -mfloat-abi=hard\
 -mfpu=vfpv3-d16\
 -Wpedantic\

ASMFLAGS =-EL

ifeq "$(MAKECMDGOALS)" "debug"
COMMON_FLAGS +=-gdwarf-3 -gstrict-dwarf -g -g3
ASMFLAGS +=-g
endif

CFLAGS =$(TARGET_ARCH) -std=c99

CXXFLAGS =$(TARGET_ARCH)\
 -std=c++14\
 -fno-rtti\
 -fno-exceptions\
 -Weffc++\
 -fno-use-cxa-atexit\

