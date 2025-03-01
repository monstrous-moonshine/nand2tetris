module alu(input  logic [15:0] x, y,
           input  logic        zx, nx, zy, ny, fn, no,
           output logic [15:0] out,
           output logic        zr, ng);

    logic [15:0] zxo, nxo, zyo, nyo, and1, add1, fno;

    assign zxo = zx ? 0 : x;
    assign zyo = zy ? 0 : y;
    assign nxo = nx ? ~zxo : zxo;
    assign nyo = ny ? ~zyo : zyo;
    assign and1 = nxo & nyo;
    assign add1 = nxo + nyo;
    assign fno = fn ? add1 : and1;
    assign out = no ? ~fno : fno;
    assign zr = ~|out;
    assign ng = out[15];
endmodule
