#include "cpp/CPU.hpp"
#include "util.h"

bool CPU::sim_done = false;

int main() {
    Computer com;
    com.load(stdin);
    auto start_time = gettime();
    while (!CPU::sim_done) com.step();
    auto end_time = gettime();
    printf("Elapsed: %lu\n", end_time - start_time);
}
