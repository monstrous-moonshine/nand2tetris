module rom #(parameter N = 15, M = 16)
            (input  logic [N-1:0] adr,
             output logic [M-1:0] dout);
    always_comb
        case (adr)
0: dout = 16'b0000000000000000;
1: dout = 16'b1110111111001000;
2: dout = 16'b0000000000000001;
3: dout = 16'b1110101010001000;
4: dout = 16'b0000000000000000;
5: dout = 16'b1111110000010000;
6: dout = 16'b0000000001100100;
7: dout = 16'b1110010011010000;
8: dout = 16'b0000000000010010;
9: dout = 16'b1110001100000001;
10: dout = 16'b0000000000000000;
11: dout = 16'b1111110000010000;
12: dout = 16'b0000000000000001;
13: dout = 16'b1111000010001000;
14: dout = 16'b0000000000000000;
15: dout = 16'b1111110111001000;
16: dout = 16'b0000000000000100;
17: dout = 16'b1110101010000111;
18: dout = 16'b0000000000010010;
19: dout = 16'b1110101010000111;
default: dout = 16'b0;
        endcase
endmodule
