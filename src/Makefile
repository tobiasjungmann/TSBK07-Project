build_dir := ../build
src_dir := .
common_dir := common

includes := -I$(common_dir)/ -I$(common_dir)/Linux -I$(src_dir)
common_objs := $(wildcard $(build_dir)/$(common_dir)/*.o $(build_dir)/$(common_dir)/Linux/*.o)

dirs := scene . scene/gameobj
sources := $(foreach dir,$(dirs),$(wildcard $(dir)/*.cpp))
targets := $(sources:.cpp=.o)
targets := $(addprefix $(build_dir)/,$(targets))
obj_dirs := $(addprefix $(build_dir)/,$(dirs))

PROGRAM := $(build_dir)/$(TARGET) 


all: $(targets) | $(obj_dirs)
	$(CXX) $(targets) $(common_objs) $(LDLIBS) -o $(PROGRAM)

.PHONY: create_objdirs all

$(build_dir)/%.o: %.cpp
	$(CXX) $(CPPOPTS) $(CPPFLAGS) -c $< -o $@ $(includes) $(MACROS) $(LDLIBS)

$(obj_dirs):
	for dir in $(obj_dirs); do \
		mkdir -p $$dir; \
	done
