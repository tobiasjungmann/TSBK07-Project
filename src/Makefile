build_dir := ../build
src_dir := .
common_dir := common

includes := -I$(common_dir)/ -I$(common_dir)/Linux
common_objs := $(wildcard $(build_dir)/common/*.o)

dirs := scene .
sources := $(foreach dir,$(dirs),$(wildcard $(dir)/*.cpp))
targets := $(sources:.cpp=.o)
obj_dirs := $(addprefix $(build_dir)/,$(dirs))


all: create_objdirs $(targets)
	$(info $<)
	$(LINK) $(targets) $(common_objs) $(LDLIBS) -o $(build_dir)/main 

.PHONY: create_objdirs all

%.o: %.cpp
	$(CXX) $(CPPOPTS) $(CPPFLAGS) -c $< -o $(build_dir)/$@ $(includes) $(MACROS) $(LDLIBS)

create_objdirs:
	for dir in $(obj_dirs); do \
		mkdir -p $$dir; \
	done
