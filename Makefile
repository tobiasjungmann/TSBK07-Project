build_dir := build
src_dir := src
common_dir := $(src_dir)/common

export MACROS = -DGL_GLEXT_PROTOTYPES
export LDLIBS := -lXt -lX11 -lGL -lm
export CPPOPTS := -std=c++17 -Wall
 
TARGET := deep_sea

.PHONY: clean profile $(common_dir) $(src_dir)

all: $(common_dir) $(src_dir)


# active debugging and level with CPPFLAGS="-g3 -Og -DDLEVEL="

profile: CPPFLAGS = -g -O3
profile: all
	valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes $(build_dir)/$(TARGET)

run: $(src_dir)
	./$(build_dir)/$(TARGET)

$(common_dir):
	$(MAKE) -C $@

$(src_dir): $(common_dir)
	$(MAKE) -C $@ TARGET=$(TARGET)

clean:
	rm -r $(build_dir)
	mkdir -p $(build_dir)/common/Linux
	mkdir -p $(build_dir)/scene/gameobj
