#include "Vhack.h"
#include "Vhack___024root.h"
#include "Vhack__Syms.h"
#include "verilated.h"
#include <memory>

int main(int argc, char *argv[]) {
    Verilated::mkdir("logs");
    auto ctxp = std::make_unique<VerilatedContext>();
    ctxp->traceEverOn(true);
    ctxp->commandArgs(argc, argv);
    auto tb = std::make_unique<Vhack>(ctxp.get(), "TOP");
    tb->reset = 1;
    tb->clk = 0;
    tb->eval();
    tb->clk = 1;
    tb->eval();
    tb->reset = 0;
    auto &root = *tb->rootp;
    while (!ctxp->gotFinish()) {
        ctxp->timeInc(1);
        tb->clk = !tb->clk;
        tb->eval();
    }
    tb->final();
    ctxp->statsPrintSummary();
}
