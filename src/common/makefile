build_dir := ../../build/common

includes := -ILinux/

sources := Linux/MicroGlut.c GL_utilities.cpp LoadTGA.cpp LittleOBJLoader.cpp VectorUtils4.cpp
targets := $(sources:.c=.c.o)
targets := $(targets:.cpp=.o)

CPPFLAGS += -w

dirs := $(addprefix $(build_dir)/,$(sort $(dir $(targets:.cpp=.o))))

all: $(targets) | $(dirs)
.PHONY: create_objdirs all 

%.c.o: %.c
	$(CXX) $(CPPOPTS) $(CPPFLAGS) -c $< -o $(build_dir)/$(dir $@)/$(notdir $(@:.c.o=.o)) $(MACROS) $(includes) $(LDLIBS) -lstdc++

%.o: %.cpp
	$(CXX) $(CPPOPTS) $(CPPFLAGS) -c $< -o $(build_dir)/$(dir $@)/$(notdir $@) $(MACROS) $(includes) $(LDLIBS)



$(dirs):
	for dir in $(dirs); do \
		mkdir -p $$dir; \
	done
	