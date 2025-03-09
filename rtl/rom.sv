module rom #(parameter N = 15, M = 16)
            (input  logic [N-1:0] adr,
             output logic [M-1:0] dout);

    logic [15:0] ROM [2**N-1:0];

    initial
        $readmemb("../rect.hack", ROM);

    assign dout = ROM[adr];
endmodule
