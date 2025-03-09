CXXFLAGS = -Os
test_CPU: test_CPU.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)

test_ALU: test_ALU.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)
