    @16384
    D=A
    @j
    M=D
    @17984
    D=A
    @h
    M=D
(LOOP)
    @j
    D=M
    @h
    D=D-M
    @END
    D;JGE
    @61680
    D=A
    @j
    A=M
    M=D
    @32
    D=A
    @j
    M=D+M
    @LOOP
    0;JMP
(END)
    @END
    0;JMP
