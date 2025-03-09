`include "cpu.sv"
`include "ram.sv"
`include "rom.sv"
module hack(input logic clk, reset);
    logic writeM;
    logic [15:0] instr, inM, outM;
    logic [14:0] addrM, pc;

    cpu m_cpu(
        .clk   (clk),
        .reset (reset),
        .instr (instr),
        .inM   (inM),
        .writeM(writeM),
        .outM  (outM),
        // verilator lint_off WIDTHEXPAND
        .addrM (addrM),
        .pc    (pc)
        // verilator lint_on WIDTHEXPAND
    );

    memory m_mem(
        .clk (clk),
        .we  (writeM),
        .adr (addrM),
        .din (outM),
        .dout(inM));

    rom m_rom(
        .adr (pc),
        .dout(instr));

    initial begin
        if ($test$plusargs("trace") != 0) begin
            $dumpfile("logs/vlt_dump.vcd");
            $dumpvars();
        end
    end
endmodule
