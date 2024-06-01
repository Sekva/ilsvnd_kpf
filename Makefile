OUTPUT:=ilsvnd_kpf

SRC_DIR=./src/
OBJS_DIR=./obj
BUILD_DIR=./build/

# Coment this to enable assert (integrity checks)
ASSERT_OPT=-DNDEBUG

OTIM_OPT=-O3 -O
DEBUG_OPT=-g -ggdb3

COMP_FLAGS=-m64 -fexceptions -DIL_STD -static-libgcc -Wall -Wextra -Wextra -Wpedantic -pedantic-errors -std=c++17

ifeq ($(DEBUG_OPT),)
	COMP_FLAGS+=-s
endif

COMP_FLAGS+=$(OTIM_OPT) $(DEBUG_OPT) $(ASSERT_OPT)

# Time Limit
COMP_FLAGS+=-DTIME_LIMIT=10

# swap-01 and swap-10
COMP_FLAGS+=-DADDRM

# swap-11 e swap-21
COMP_FLAGS+=-DSWAPS

# Tabu
COMP_FLAGS+=-DTABU

# Threads
COMP_FLAGS+=-DNUM_THREADS=1

LIBS=-lm -pthread -ldl

CC=g++

C_SRCS:=$(wildcard $(SRC_DIR)*.cpp)
SOURCE_DIRS+=$(dir $(C_SRCS))
VPATH=$(sort $(SOURCE_DIRS))
C_FILENAMES:=$(notdir $(C_SRCS))
OBJ_FILES:=$(patsubst %.cpp, $(OBJS_DIR)/%.o, $(C_FILENAMES))

TARGET:=$(BUILD_DIR)$(OUTPUT)

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(shell mkdir -p $(BUILD_DIR))
	$(CC) $(COMP_FLAGS) $(LIBS) -o $@ $(OBJS_DIR)/*.o
	@mv -v $@ .
	@rm -rvf $(BUILD_DIR)

$(OBJS_DIR)/%.o : %.cpp
	$(shell mkdir -p $(OBJS_DIR))
	$(CC) $(COMP_FLAGS) -c $< -o $@

clean:
	@rm -vf $(OBJS_DIR)/*
	@rmdir --ignore-fail-on-non-empty -v $(OBJS_DIR) || true
	@rm -vf $(BUILD_DIR)
	@rm -vf $(OUTPUT)
