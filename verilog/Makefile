CXXFLAGS = $(shell pkg-config --cflags verilator) -Iobj_dir/ -Os
VERILATOR_FLAGS = -Wall --cc #--trace
LDLIBS = obj_dir/libverilated.a -lSDL3

hack: hack.cpp obj_dir/libVhack.a
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDLIBS)

hack.cpp: io.hpp
	@touch $@

obj_dir/libVhack.a: hack.sv cpu.sv alu.sv memory.sv ram.sv rom.sv
	verilator $(VERILATOR_FLAGS) $<
	make -C obj_dir/ -f Vhack.mk

cpu: cpu.cpp obj_dir/libVcpu.a
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDLIBS)

obj_dir/libVcpu.a: cpu.sv alu.sv
	verilator $(VERILATOR_FLAGS) $<
	make -C obj_dir/ -f Vcpu.mk

alu: alu.cpp obj_dir/libValu.a
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDLIBS)

obj_dir/libValu.a: alu.sv
	verilator $(VERILATOR_FLAGS) $<
	make -C obj_dir/ -f Valu.mk

clean:
	rm -rf obj_dir/ alu cpu hack
