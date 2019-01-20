.SILENT:
.SUFFIXES:

# выберите требуемый компилятор: clang или gcc
COMPILER=gcc
TOOLCHAIN=toolchain.$(COMPILER).mk

include $(TOOLCHAIN)

DEFINES =\
 -D__little_endian__\

ALL_INCLUDES =\
 $(INCLUDES)\
 $(SYS_INCLUDES)\

# расширение выходного файла.
OUTFILE_EXT =elf

# имя выходного файла, а также mapfile (.map), hexfile (.hex)
OUTNAME =motorHw
OUTFILE =$(OUTNAME).$(OUTFILE_EXT)
HEXFILE =$(OUTNAME).hex
SRECFILE=$(OUTNAME).srec
MAPFILE =$(OUTNAME).map
BINFILE =$(OUTNAME).bin

include dirs.mk helpers.mk ld.mk

COMPILE_DEPENDENCIES =$(OBJS_DIR) $(OBJ_DIRS_FOR_SOURCES) $(LDSCRIPT) $(OBJ) Makefile ld.mk

all debug release: $(OBJS_DIR)/$(OUTFILE)

compile: $(COMPILE_DEPENDENCIES)

$(OBJS_DIR)/$(OUTFILE): $(COMPILE_DEPENDENCIES)
	$(LD) $(COMMON_FLAGS) $(CXXFLAGS) $(LDFLAGS) -Wl,-Map,$(OBJS_DIR)/$(MAPFILE) $(OBJ) -o $@
	$(SIZE) $@
	$(FROMELF) --output-target=binary --gap-fill 0xFF -v $@ $(OBJS_DIR)/$(BINFILE)
	$(FROMELF) --output-target=srec -v $@ $(OBJS_DIR)/$(SRECFILE)
#	$(FROMELF) --output-target=ihex -v $@ $(OBJS_DIR)/$(HEXFILE)

$(OBJS_DIR) $(OBJ_DIRS_FOR_SOURCES):
	mkdir.exe -p $@

$(OBJS_DIR)/$(HEXFILE): $(OBJS_DIR)/$(OUTFILE)

clean:
	$(RM) -rf $(GLOBAL_OBJS_DIR) $(wildcard $(HETPROGRAM_DIR)/*.c) $(wildcard $(HETPROGRAM_DIR)/*.h) $(wildcard *.bin)


# компилятор HET
HETCC = ../utils/hetp.exe

# Явное правило для компиляции .het файла для модуля HET1; автоматически не получится из-за параметра -n0 для hetp.exe
$(HETPROGRAM_DIR)/het1program.c: $(HETPROGRAM_DIR)/het1program.het
	$(HETCC) -hc32 -n0 -v2 $<

include calls.mk
-include $(DEP_FILES)

