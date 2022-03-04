# ABOUT
Design implementation of the RISC-V Integer core in Verilog HDL. The core is implemented via FSM. The standard 5 prstages 

## Hierarchy  
-> rv32i_core    
   -> rv32i_basereg  
   -> rv32i_decoder  
## Testbench
The hex file `inst.hex` contains the test instructions.This hex file initializes the instruction memory regfile using the `readmemh` command. Below are the testcases with the expected results of each instruction in comments.

```verilog
//memory address 0x1000 is initialized to 0x12345678
00c00093 //addi x1, x0, 12  (write 12 to x1)
00000193 //addi x3, x0, 0   (write 0 to x3)
000011b7 //lui x3, 0x1      (load value of 0x1000 to x3)
0001a183 //lw x3, 0(x3)     (load memory address 0x1000 (which is 0x12345678) to x3) 
fff1c193 //xori x3, x3, -1  (write 0xedcba987 to x3)
00000213 //addi x4, x0, 0   (write 0 to x4)
00001237 //lui x4, 0x1      (load value of 0x1000 to x4)
00322223 //sw x3, 4(x4)     (store value of x3(which is 0xedcba987) to memory address 0x1000+4 or 0x1004
00000513 //li x10, 0        (write 0 to x10)
05d00893 //li x17, 93       (write 93(0x5d) to x17)
```
# TO DO
 - Add more testcases for the core testbench  
 - Convert FSM to pipeline   
 - Add CSR

# [UNDER CONTRUCTION]
