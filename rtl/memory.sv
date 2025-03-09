module memory #(parameter N = 15)
               (input  logic         clk,
                input  logic         we,
                input  logic [N-1:0] adr,
                input  logic [15:0]  din,
                output logic [15:0]  dout);

    logic [15:0] dout1, dout2, dout3;
    logic wr_mem, wr_scr;

    assign wr_mem = we & ~adr[N-1];
    assign wr_scr = we &  adr[N-1] & ~adr[N-2];

    ram #(14, 16) blk(clk, wr_mem, adr[N-2:0], din, dout1);
    ram #(13, 16) scr(clk, wr_scr, adr[N-3:0], din, dout2);

    import "DPI-C" function shortint readkey();
    always_ff @(posedge clk)
        if (adr[N-1] & adr[N-2]) dout3 <= readkey();

    import "DPI-C" function void write_disp(input shortint adr,
                                            input shortint din);
    always_ff @(posedge clk)
        if (wr_scr) write_disp({3'b0, adr[N-3:0]}, din);

    assign dout = ~adr[N-1] ? dout1 :
                  ~adr[N-2] ? dout2 :
                  //~|adr[N-3:0] ? 
                  dout3
                  //: 16'b0
                  ;
endmodule
