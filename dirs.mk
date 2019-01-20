# Top-level directory for object files
GLOBAL_OBJS_DIR=objs

# Directory for 'release' or 'debug' builds
ifeq "$(MAKECMDGOALS)" "debug"
OBJS_DIR =$(GLOBAL_OBJS_DIR)/debug
else
OBJS_DIR =$(GLOBAL_OBJS_DIR)/release
DEFINES +=-DNDEBUG
endif

# Заголовочные файлы
INCLUDES_DIRS =\
 hetlib\
 include\
 halcogen/include\
 src/Motor\

# необходимые каталоги для сборки
SOURCE_DIRS =\
 hetlib\
 halcogen/src\
 src\
 src/monitorCmd\
 src/physv\
 src/Motor\
 src/gccstubs\
 src/startup\
 src/math\
 src/Motor/modules/angle_gen\
 src/Motor/modules/trajectory\
 src/Motor/modules/ipark\
 src/Motor/modules/svgen\
 src/Motor/modules/vs_freq\
 src/Motor/modules/qep\
 src/Motor/modules/speed_est\
 src/Motor/modules/pi\
 src/Motor/modules/clarke\
 src/Motor/modules/park\
 src/Motor/modules/phasor\

HETPROGRAM_DIR=hetlib
