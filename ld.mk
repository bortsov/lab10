# путь и имя скрипта линкера
#LDSCRIPT =resource/rom.ld
LDSCRIPT =resource/ram.ld

LDFLAGS =-Wl,-T,$(LDSCRIPT) -Wl,--gc-sections --specs=nosys.specs --specs=nano.specs

