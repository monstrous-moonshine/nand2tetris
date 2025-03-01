`include "alu.sv"
module cpu(input  logic        clk, reset,
           input  logic [15:0] instr, inM,
           output logic        writeM,
           output logic [15:0] outM, addrM, pc);

    logic [15:0] acmux, areg, ammux, dreg;
    logic zr, ng, load_A, load_D, load_PC, should_jmp;

    alu alu1(
        .x  (dreg),
        .y  (ammux),
        .zx (instr[11]),
        .nx (instr[10]),
        .zy (instr[ 9]),
        .ny (instr[ 8]),
        .fn (instr[ 7]),
        .no (instr[ 6]),
        .out(outM),
        .zr (zr),
        .ng (ng));

    assign acmux = instr[15] ? outM : instr;
    assign ammux = instr[12] ? inM : areg;
    assign writeM = instr[15] && instr[3];
    assign addrM = areg;
    assign should_jmp = ng && instr[2] ||
                        zr && instr[1] ||
                        !(ng || zr) && instr[0];
    assign load_A = !instr[15] || instr[5];
    assign load_D = instr[15] && instr[4];
    assign load_PC = instr[15] && should_jmp;
    
    always_ff @(posedge clk)
    begin
        if (load_A)
            areg <= acmux;
        if (load_D)
            dreg <= outM;
        if (reset)
            pc <= 0;
        else if (load_PC)
            pc <= areg;
        else
            pc <= pc + 1;
        if (pc == 18)
            $finish;
    end
endmodule
