build_dir := ../../build/common

includes := -ILinux/

sources := Linux/MicroGlut.c GL_utilities.c LoadTGA.c LittleOBJLoader.c VectorUtils4.cpp
targets := $(sources:.c=.c.o)
targets := $(targets:.cpp=.o)

dirs := $(addprefix $(build_dir)/,$(sort $(dir $(targets:.cpp=.o))))

all: create_objdirs $(targets)
.PHONY: create_objdirs all 

%.c.o: %.c
	$(CXX) $(CPPOPTS) $(CPPFLAGS) -c $< -o $(build_dir)/$(dir $@)/$(notdir $(@:.c.o=.o)) $(MACROS) $(includes) $(LDLIBS) -lstdc++

%.o: %.cpp
	$(CXX) $(CPPOPTS) $(CPPFLAGS) -c $< -o $(build_dir)/$(dir $@)/$(notdir $@) $(MACROS) $(includes) $(LDLIBS)



create_objdirs:
	for dir in $(dirs); do \
		mkdir -p $$dir; \
	done
	