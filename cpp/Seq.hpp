#pragma once
#include "Comb.hpp"
#include <cstdio>
#include <memory>

struct Register {
    const Wire16 *in;
    const Wire *load;
    Wire16 *out;
    void step() {
        *out = (*load) ? static_cast<int>(*in) : static_cast<int>(*out);
    }
};

template <unsigned N>
struct RAM {
    const Wire16 *in, *address;
    const Wire *load;
    Wire16 *out;
    RAM(const Wire16 *in, const Wire16 *address,
        const Wire *load, Wire16 *out)
        : in(in), address(address), load(load), out(out),
          mem(new Wire16[N]) {}
    void step() {
        auto addr = static_cast<int>(*address);
        mem[addr] = (*load) ? static_cast<int>(*in) : static_cast<int>(mem[addr]);
        *out = mem[addr];
    }
private:
    std::unique_ptr<Wire16[]> mem;
};

template <unsigned N>
struct ROM {
    const Wire16 *address;
    Wire16 *out;
    ROM(const Wire16 *address, Wire16 *out) : address(address), out(out), mem(new Wire16[N]) {}
    void operator()() { *out = mem[static_cast<int>(*address)]; }
    void load(FILE *prog) {
        char buf[20];
        int count = 0;
        while (fgets(buf, sizeof buf, prog) != NULL && count < N) {
            int word = 0;
            for (int i = 0; i < 16; i++)
                word = (word << 1) | (buf[i] - '0');
            mem[count++] = word;
        }
    }
private:
    std::unique_ptr<Wire16[]> mem;
};

struct PC {
    const Wire16 *in;
    const Wire *inc, *load, *reset;
    Wire16 *out;
    void step() {
        auto vin = static_cast<int>(*in);
        auto vout = static_cast<int>(*out);
        auto v1 = (*inc) ? (vout + 1) & 0xffff : vout;
        auto v2 = (*load) ? vin : v1;
        *out = (*reset) ? 0 : v2;
    }
};
