## ABOUT
Design implementation of the RISC-V Integer core in Verilog HDL. The core is currently FSM-based (no pipelining) and no Control Status Registers (CSR) yet.   
Inside the `rtl` folder are the following:  

 - `rv32i_soc.v` = complete package containing the rv32i_core , ROM (for instruction memory) , and RAM (for data memory)  
 - `rv32i_core.v` = top module for the RV32I core  
 - `rv32i_fsm.v` = FSM controller for the fetch, decode, execute, memory access, and writeback processes
 - `rv32i_basereg.v` = interface for the regfile of the 32 integer base registers 
 - `rv32i_decoder.v`= logic for the decoding of a 32 bit instruction [DECODE STAGE]
 - `rv32i_alu.v` =  arithmetic logic unit [EXECUTE STAGE]
 - `rv32i_memoryaccess.v` = logic controller for data memory access [MEMORYACCESS STAGE]
 - `rv32i_writeback.v` = logic controller for determining the next `PC` and `rd` value [WRITEBACK STAGE]
 
Inside the `testbench` folder are the following:
 - `rv32i_soc_TB.v` = testbench for `rv32i_soc`
 - `hexfile` folder = contains the `inst.hex` for the test instructions
 - `scripts/` folder = contains scripts for Modelsim and Vivado simulation   
 
 Other files at the top directory:
 - `test.sh` = script for automated testing with an ISS
 - `rv_asm.bat` = batch file used by `test.sh` for compilation of assembly files
 - `testbank/` folder = contains the assembly files used as testcases for the ISS
 
## INTERFACE
Below is the interface for `rv32i_core`:

![interface_1](https://user-images.githubusercontent.com/87559347/156866977-aa026174-e13a-401c-9ef7-0bc02ba8a12c.png)

And here are the top level blocks for`rv32i_soc`:  

![interface_2](https://user-images.githubusercontent.com/87559347/156867346-322be64d-2f1c-4f70-9980-36776bcec9c0.png)



## SIMPLE TESTBENCH
The hex file `./hexfile/inst.hex`, as shown below, contains the test instructions used in the testbench `rv32i_soc_TB.v`. This is used to initialize the instruction memory regfile of the `rv32i_soc.v` module using the `readmemh` command. In comments are the equivalent assembly codes of each hex instruction and its expected results.

```verilog
//memory address 0x1000 is initialized to 0x12345678
00c00093 //addi x1, x0, 12  (write 12 to x1)
00000193 //addi x3, x0, 0   (write 0 to x3)
000011b7 //lui x3, 0x1      (load value 0x1000 to x3)
0001a183 //lw x3, 0(x3)     (load value of memory address 0x1000 (which is 0x12345678) to x3) 
fff1c193 //xori x3, x3, -1  (write 0xedcba987 to x3)
00000213 //addi x4, x0, 0   (write 0 to x4)
00001237 //lui x4, 0x1      (load value 0x1000 to x4)
00322223 //sw x3, 4(x4)     (store value of x3(which is 0xedcba987) to memory address 0x1000+4 or 0x1004
00000513 //li x10, 0        (write 0 to x10)
05d00893 //li x17, 93       (write 93(0x5d) to x17)
```

The `rv32i_soc_TB` monitors all write access to base register and data memory of the `rv32i_soc`. It displays the address accessed and its new value.
 - `[BASEREG]` pertains to write access to base register
 - `[MEMORY]` for write access to memory data.  

After executing all instructions, the state of the 32 base registers and relevant memory data are displayed.  
 - **To run the simulation via Modelsim, open cmd terminal inside `testbench/scripts/` directory. Run this command: `vsim -do sim.do`**      
 - **To run the simulation via Vivado, open cmd terminal inside `testbench/scripts/` directory. Run this command: `./sim.sh gui`**      
 
Below is the resulting output of this `rv32i_soc_TB` testbench: 

```

Start executing instructions......

# Start executing instructions......
# 
# Monitor All Writes to Base Register and Data Memory
# [BASEREG] address:0x01   value:0x0000000c
# [BASEREG] address:0x03   value:0x00000000
# [BASEREG] address:0x03   value:0x00001000
# [BASEREG] address:0x03   value:0x12345678
# [BASEREG] address:0x03   value:0xedcba987
# [BASEREG] address:0x04   value:0x00000000
# [BASEREG] address:0x04   value:0x00001000
# [MEMORY] address:0x1004   value:0xedcba987 [MASK:1111]
# [BASEREG] address:0x0a   value:0x00000000
# [BASEREG] address:0x11   value:0x0000005d
# 
# All instructions executed......
# 
# Final Register State:
# 0x 0: 0xxxxxxxxx	0x 1: 0x0000000c	0x 2: 0xxxxxxxxx	0x 3: 0xedcba987	
# 0x 4: 0x00001000	0x 5: 0xxxxxxxxx	0x 6: 0xxxxxxxxx	0x 7: 0xxxxxxxxx	
# 0x 8: 0xxxxxxxxx	0x 9: 0xxxxxxxxx	0x10: 0x00000000	0x11: 0xxxxxxxxx	
# 0x12: 0xxxxxxxxx	0x13: 0xxxxxxxxx	0x14: 0xxxxxxxxx	0x15: 0xxxxxxxxx	
# 0x16: 0xxxxxxxxx	0x17: 0x0000005d	0x18: 0xxxxxxxxx	0x19: 0xxxxxxxxx	
# 0x20: 0xxxxxxxxx	0x21: 0xxxxxxxxx	0x22: 0xxxxxxxxx	0x23: 0xxxxxxxxx	
# 0x24: 0xxxxxxxxx	0x25: 0xxxxxxxxx	0x26: 0xxxxxxxxx	0x27: 0xxxxxxxxx	
# 0x28: 0xxxxxxxxx	0x29: 0xxxxxxxxx	0x30: 0xxxxxxxxx	0x31: 0xxxxxxxxx	
# 
# 
# Final Memory State:
# 0x1000: 0x12345678
# 0x1004: 0xedcba987
# 0x1008: 0x00000000
# 0x100c: 0x00000000
# 0x1010: 0x00000000
# 0x1014: 0x00000000
# 0x1018: 0x00000000
```
Below is the screenshot of the waveforms for the relevant base registers and memory data accessed in this testbench:  

![wave](https://user-images.githubusercontent.com/87559347/156799580-2dc78eed-1ef1-4cf0-a64a-b182b0725628.png)  
 - `iaddr` = instruction address (PC value)  
 - `base_regfile[][]` = base register regfile  
 - `data_regfile[][]` = memory data regfile  

## Automated Tests with ISS (Instruction Set Simulator)
The RISCV toolchain `riscv64-unknown-elf-` must be added to PATH. The ISS used here is from https://github.com/wyvernSemi/riscV . The ISS executable file `rv32.exe` must be moved at the top directory of this repository.  

Run the script `test.sh`. Below is the expected output:   

![shelloutput](https://user-images.githubusercontent.com/87559347/162359061-16b70a0b-7856-4230-9fdc-3840c359e3fa.png)




## UTILIZATION [Vivado Synthesis Report]  
```
+-------------------------+------+-------+------------+-----------+-------+
|        Site Type        | Used | Fixed | Prohibited | Available | Util% |
+-------------------------+------+-------+------------+-----------+-------+
| Slice LUTs*             |  727 |     0 |          0 |     14600 |  4.98 |
|   LUT as Logic          |  727 |     0 |          0 |     14600 |  4.98 |
|   LUT as Memory         |    0 |     0 |          0 |      5000 |  0.00 |
| Slice Registers         |  270 |     0 |          0 |     29200 |  0.92 |
|   Register as Flip Flop |  270 |     0 |          0 |     29200 |  0.92 |
|   Register as Latch     |    0 |     0 |          0 |     29200 |  0.00 |
| F7 Muxes                |    0 |     0 |          0 |      7300 |  0.00 |
| F8 Muxes                |    0 |     0 |          0 |      3650 |  0.00 |
+-------------------------+------+-------+------------+-----------+-------+
* Warning! The Final LUT count, after physical optimizations and full implementation, is typically lower. Run opt_design after synthesis, if not already completed, for a more realistic count.
```

## COMING SOON
 - Add more testcases for the core testbench  
 - Add CSR 
 - Convert FSM based core implementation to pipeline   
 - Add RV32 extensions
 
# [UNDER CONSTRUCTION] 
