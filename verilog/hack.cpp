#include "Vhack.h"
#include "verilated.h"
#include "../util.h"
#include "io.hpp"
#include <csignal>
#include <memory>
#include <thread>

static bool sim_running{};
static std::unique_ptr<GUI> gui;

extern "C" short readkey() {
    //fprintf(stderr, "INFO: %s\n", __func__);
    return gui->keysym;
}

extern "C" void write_disp(short adr, short din) {
    fprintf(stderr, "INFO: %s(%d, %d)\n", __func__, adr, din);
    VALIDATE(SDL_LockSurface(gui->surf));
    ((short *)gui->surf->pixels)[adr] = din;
    SDL_UnlockSurface(gui->surf);
}

static void do_sim(VerilatedContext *ctx, Vhack *tb) {
    sim_running = true;
    tb->reset = 1;
    tb->clk = 1;
    tb->eval();
    ctx->timeInc(1);
    tb->reset = 0;
    tb->clk = 0;
    tb->eval();
    while (!ctx->gotFinish() && sim_running) {
        ctx->timeInc(1);
        tb->clk = !tb->clk;
        tb->eval();
    }
}

static void handle_sig(int) {
    sim_running = false;
}

static void setup_signal() {
    struct sigaction sa{};
    sa.sa_handler = handle_sig;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction"), exit(1);
    }
}

int main(int argc, char *argv[]) {
    Verilated::mkdir("logs");
    auto ctxp = std::make_unique<VerilatedContext>();
    ctxp->traceEverOn(true);
    ctxp->commandArgs(argc, argv);
    auto tb = std::make_unique<Vhack>(ctxp.get(), "TOP");
    setup_signal();
    gui = std::make_unique<GUI>();
    //auto start_time = gettime();
    std::thread thr(do_sim, ctxp.get(), tb.get());
    while (gui->running) {
        gui->frame();
    }
    thr.join();
    //printf("Elapsed: %lu\n", gettime() - start_time);
    tb->final();
}
