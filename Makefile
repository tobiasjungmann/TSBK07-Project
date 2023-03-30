# set this variable to the director in which you saved the common files
BUILD_DIR := build
SRC_DIR := src
commondir := $(SRC_DIR)/common
common_objs := $(BUILD_DIR)/common
DEBUG_COMMON = false

libs := -lXt -lX11 -lGL -lm
deps =

INCLUDES := -I$(commondir)/ -I$(commondir)/Linux
CC_OPTS := -Wall -std=c++17
CC := g++
FLAGS = -DGL_GLEXT_PROTOTYPES

TARGETS := $(basename $(wildcard main.cpp))

all : build_deps $(TARGETS)


DLEVEL ?= 0
debug: CC_OPTS += -g -O0 -DDLEVEL=$(DLEVEL)
debug: all


profile: CC_OPTS += -g -O3
profile: TARGETS := $(firstword $(TARGETS))
profile:
	$(MAKE) TARGETS=$(TARGETS) DEBUG_COMMON=true CC_OPTS="$(CC_OPTS)"
	valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes $(BUILD_DIR)/$(TARGETS)


build_deps:
	$(MAKE) -C $(commondir)
	$(eval deps := $(wildcard $(common_objs)/*.o))

%.o : %.cpp | $(BUILD_DIR)
	$(CC) $(CC_OPTS) -o $(BUILD_DIR)/$@ $(INCLUDES) $< $(deps) $(libs)


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean :
	rm $(addprefix $(BUILD_DIR)/, $(TARGETS))
