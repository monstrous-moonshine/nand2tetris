1. Build the assembler and the hack simulator:
    ```
    g++ -o has has.cc
    g++ -g -o test_CPU test_CPU.cc
    ```
2. Assemble the sum program and feed it to the hack simulator:
    ```
    ./has < sum.asm > sum.hack
    ./test_CPU < sum.hack
    ```

Nothing seems to be going on. That's because the computer has calculated the sum 1 + 2 + 3 + ... + 100 and is now spinning in a loop, which is how it "halts". To see if it calculated the right result, observe that the PC should be 18 when it reaches the infinite loop after finishing the calculation. The variable `i` is at address 0, and `sum` at address 1, so we can examine them in gdb:
```
$ gdb ./test_CPU
(gdb) break Computer::step() if cpu.pc.value == 18
(gdb) run < sum.hack

Breakpoint 1, Computer::step (this=0x7fffffffe2a0) at /home/arch/proj/hardware/nand2tetris/CPU.hpp:50
50              rom();
(gdb) p ram.mem[0].value
$1 = 101
(gdb) p ram.mem[1].value
$2 = 5050
```
So `i` is 101 when it exceeded 100 and the sum loop ended, and the sum is 5050, which Gauss tells us is the right answer.
