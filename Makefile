# set this variable to the director in which you saved the common files
BIN_DIR := bin
commondir := ../common
common_objs := $(commondir)/build
DEBUG_COMMON = false

libs := -lXt -lX11 -lGL -lm
deps =

INCLUDES := -I$(commondir)/ -I$(commondir)/Linux
CC_OPTS := -Wall -std=c++17
CC := g++
FLAGS = -DGL_GLEXT_PROTOTYPES

TARGETS := $(basename $(wildcard lab*-*.cpp))

all : build_deps $(TARGETS)


DLEVEL ?= 0
debug: CC_OPTS += -g -O0 -DDLEVEL=$(DLEVEL)
debug: all


profile: CC_OPTS += -g -O3
profile: TARGETS := $(firstword $(TARGETS))
profile:
	$(MAKE) TARGETS=$(TARGETS) DEBUG_COMMON=true CC_OPTS="$(CC_OPTS)"
	valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes $(BIN_DIR)/$(TARGETS)




build_deps:
ifndef deps
ifeq ($(strip $(DEBUG_COMMON)),true)
	$(MAKE) -C $(commondir) debug DLEVEL=$(DLEVEL)
else
	$(MAKE) -C $(commondir)
endif
	$(eval deps := $(wildcard $(common_objs)/*.o))
endif

lab% : lab%.cpp build_deps | $(BIN_DIR)
	$(CC) $(CC_OPTS) -o $(BIN_DIR)/$@ $(INCLUDES) $< $(deps) $(libs)


$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean :
	rm $(addprefix $(BIN_DIR)/, $(TARGETS))
