#include "ALU.hpp"
#include <cstdio>

int main() {
    Wire16 x, y, out;
    Wire zx, nx, zy, ny, f, no, zr, ng;
    ALU alu{&x, &y, &zx, &nx, &zy, &ny, &f, &no, &out, &zr, &ng};
    x = 1;
    y = 100;
    // x - y
    zx = 0;
    nx = 1;
    zy = 0;
    ny = 0;
    f  = 1;
    no = 1;
    alu();
    printf("%d - %d = %d\n", static_cast<int>(x), static_cast<int>(y),
           static_cast<int>(out));
}
