#include "cpp/CPU.hpp"

int main() {
    Computer com;
    com.load(stdin);
    for (;;) com.step();
}
