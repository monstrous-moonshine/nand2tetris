CXXFLAGS = -Os
test_CPU: test_CPU.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)
