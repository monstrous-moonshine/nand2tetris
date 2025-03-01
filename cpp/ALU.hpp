#pragma once
#include "Comb.hpp"
struct ALU {
    ALU(const Wire16 *x, const Wire16 *y,
        const Wire *zx, const Wire *nx,
        const Wire *zy, const Wire *ny,
        const Wire *fn, const Wire *no,
        Wire16 *out, Wire *zr, Wire *ng)
        : x(x), y(y), zx(zx), nx(nx), zy(zy), ny(ny), fn(fn), no(no),
          out(out), zr(zr), ng(ng) {}
    void operator()() {
        zxm();
        zym();
        not1();
        not2();
        nxm();
        nym();
        and1();
        add1();
        fnm();
        not3();
        nom();
        *zr = static_cast<int>(*out) == 0;
        *ng = static_cast<int>(*out) < 0;
    }
private:
    // inputs
    const Wire16 *x, *y;
    const Wire *zx, *nx, *zy, *ny, *fn, *no;
    // outputs
    Wire16 *out;
    Wire *zr, *ng;
    // internal
    Wire16 zero{0}, zxo, zxon, zyo, zyon, nxo, nyo, addo, ando, fno, fnon;
    Mux16 zxm{x, &zero, zx, &zxo},
          zym{y, &zero, zy, &zyo},
          nxm{&zxo, &zxon, nx, &nxo},
          nym{&zyo, &zyon, ny, &nyo},
          fnm{&ando, &addo, fn, &fno},
          nom{&fno, &fnon, no, out};
    Not16 not1{&zxo, &zxon},
          not2{&zyo, &zyon},
          not3{&fno, &fnon};
    And16 and1{&nxo, &nyo, &ando};
    Add16 add1{&nxo, &nyo, &addo};
};
