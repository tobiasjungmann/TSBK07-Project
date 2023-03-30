build_dir := build
src_dir := src
common_dir := $(src_dir)/common

export MACROS = -DGL_GLEXT_PROTOTYPES
export LDLIBS := -lXt -lX11 -lGL -lm
export CPPOPTS := -std=c++17 -Wall
 
.PHONY: clean profile $(common_dir) $(src_dir)

all: $(common_dir) $(src_dir)


# active debugging and level with CPPFLAGS="-g3 -Og -DDLEVEL="

profile: CPPFLAGS = -g -O3
profile: all
	valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes $(build_dir)/main


$(common_dir):
	$(MAKE) -C $@

$(src_dir): $(common_dir)
	$(MAKE) -C $@

clean:
	rm -r $(build_dir)
