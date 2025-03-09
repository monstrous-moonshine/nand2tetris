#include "Comb.hpp"
#include "Seq.hpp"
#include "ALU.hpp"

struct CPU {
    CPU(const Wire16 *inM, const Wire16 *instr, const Wire *reset,
        Wire16 *outM, Wire16 *addressM, Wire16 *pc, Wire *writeM)
        : inM(inM), instr(instr), reset(reset), outM(outM),
          addressM(addressM), pc(pc), writeM(writeM) {
        for (int i = 0; i < 16; i++) instbits[i] = Wire{instr, i};
    }
    void step() {
        if (*pc == 18) sim_done = true;
        A_M_sel();
        alu();
        A_C_sel();
        load_A = !instbits[15] || instbits[5];
        load_D = instbits[15] && instbits[4];
        load_PC = instbits[15] && (ng && instbits[2] || zr && instbits[1] ||
                                   !(ng || zr) && instbits[0]);
        pc_reg.step();
        A.step();
        D.step();
        *writeM = instbits[15] && instbits[3];
        *addressM = static_cast<int>(Aout);
    }
    static bool sim_done;
private:
    // inputs
    const Wire16 *inM, *instr;
    const Wire *reset;
    // outputs
    Wire16 *outM, *addressM, *pc;
    Wire *writeM;
    // internal
    Wire instbits[16];
    Wire load_A, load_D, load_PC, zr, ng, one{1};
    Wire16 decout, Aout, Dout, A_M_out;
    Mux16 A_C_sel{instr, outM, &instbits[15], &decout};
    Register A{&decout, &load_A, &Aout};
    Mux16 A_M_sel{&Aout, inM, &instbits[12], &A_M_out};
    Register D{outM, &load_D, &Dout};
    ALU alu{&Dout, &A_M_out, &instbits[11], &instbits[10],
            &instbits[9], &instbits[8], &instbits[7], &instbits[6],
            outM, &zr, &ng};
    PC pc_reg{&Aout, &one, &load_PC, reset, pc};
};

struct Computer {
    Computer() { cpu.step(); reset = 0; }
    void step() {
        rom();
        cpu.step();
        ram.step();
    }
    void load(FILE *prog) { rom.load(prog); }
private:
    Wire reset{1}, writeM;
    Wire16 pc, instr, inM, outM, addressM;
    ROM<32 * 1024> rom{&pc, &instr};
    RAM<16 * 1024> ram{&outM, &addressM, &writeM, &inM};
    CPU cpu{&inM, &instr, &reset, &outM, &addressM, &pc, &writeM};
};
