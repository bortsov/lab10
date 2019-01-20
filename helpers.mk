#Helper Functions
get_src_from_dir = $(wildcard $1/*.cpp) $(wildcard $1/*.c) $(wildcard $1/*.S) $(wildcard $1/*.het)
get_dirs_from_dirspec  = $(wildcard $1)
get_src_from_dir_list = $(foreach dir, $1, $(call get_src_from_dir,$(dir)))
get_asm_from_dir_list = $(foreach dir, $1, $(call get_asm_from_dir,$(dir)))
__src_to = $(subst .het,$1, $(subst .S,$1, $(subst .c,$1, $(subst .cpp,$1,$2))))
src_to = $(addprefix $3/,$(call __src_to,$1,$2))
src_to_o = $(sort $(call src_to,.o,$1,$2))
src_to_d = $(sort $(call src_to,.d,$1,$2))
print_value = echo     $1&
debug_print_list = $(foreach word,$1,$(call print_value,$(word)))

SRC_DIRS_EXPANDED = $(call get_dirs_from_dirspec, $(SOURCE_DIRS))
SRC = $(call get_src_from_dir_list, $(SRC_DIRS_EXPANDED))
DEP_FILES = $(call src_to_d,$(SRC),$(OBJS_DIR))
OBJ = $(call src_to_o,$(SRC),$(OBJS_DIR))
INCLUDES = $(foreach dir, $(INCLUDES_DIRS), -I$(dir))
SYS_INCLUDES = $(foreach dir, $(SYS_INCLUDE_DIRS), -I$(dir))
OBJ_DIRS_FOR_SOURCES = $(foreach dir, $(SOURCE_DIRS),$(OBJS_DIR)/$(dir))
