#include "Valu.h"
#include "verilated.h"
#include <memory>

int main(int argc, char *argv[]) {
    Verilated::commandArgs(argc, argv);
    auto tb = std::make_unique<Valu>();
    int fnc[] = {
        0b101010, 0b111111, 0b111010, 0b001100, 0b110000,
        0b001101, 0b110001, 0b001111, 0b110011, 0b011111,
        0b110111, 0b001110, 0b110010, 0b000010, 0b010011,
        0b000111, 0b000000, 0b010101,
    };
    const char *const fns[] = {
        "0", "1", "-1", "x", "y",
        "!x", "!y", "-x", "-y", "x+1",
        "y+1", "x-1", "y-1", "x+y", "x-y",
        "y-x", "x&y", "x|y",
    };
    tb->x = 13;
    tb->y = 8;
    printf("x = %d, y = %d\n", tb->x, tb->y);
    for (int i = 0; i < sizeof fnc / sizeof fnc[0]; i++) {
        tb->zx = (fnc[i] >> 5) & 1;
        tb->nx = (fnc[i] >> 4) & 1;
        tb->zy = (fnc[i] >> 3) & 1;
        tb->ny = (fnc[i] >> 2) & 1;
        tb->fn = (fnc[i] >> 1) & 1;
        tb->no = (fnc[i] >> 0) & 1;
        tb->eval();
        printf("%s = %d\n", fns[i], tb->out);
    }
}
