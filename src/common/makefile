BUILD_DIR := build
CC := g++
CC_OPTS := -Wall -std=c++17

PROVIDED := Linux/MicroGlut.c ./GL_utilities.c ./LoadTGA.c ./LittleOBJLoader.c
HOMEMADE := $(filter-out $(PROVIDED),$(wildcard ./*.cpp ./*.c))

SOURCES := $(PROVIDED) $(HOMEMADE)

LIBS := -lXt -lX11 -lGL -lm
FLAGS := -DGL_GLEXT_PROTOTYPES
INCLUDES := -ILinux/


TARGETS_OBJS := $(SOURCES:.c=.c.o)
TARGETS_OBJS := $(TARGETS_OBJS:.cpp=.o)

all: $(TARGETS_OBJS) | $(BUILD_DIR)

DLEVEL ?= 0
debug: CC_OPTS += -g -O0 -DDLEVEL=$(DLEVEL)
debug: all

%.c.o: %.c
	$(CC) $(CC_OPTS) -c $< -o $(BUILD_DIR)/$(notdir $(@:.c.o=.o)) $(FLAGS) $(INCLUDES) $(LIBS) -lstdc++

%.o: %.cpp
	@echo $(shell pwd)
	$(CC) $(CC_OPTS) -c $< -o $(BUILD_DIR)/$(notdir $@) $(FLAGS) $(INCLUDES) $(LIBS)


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm $(BUILD_DIR)/*.o