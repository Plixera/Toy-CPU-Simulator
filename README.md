Toy CPU Simulator

A small 8-bit virtual CPU written in C that executes a custom instruction set one step at a time and visualizes the processor state in the terminal after every instruction.

It models the core of the fetch → decode → execute cycle: a program counter, general-purpose registers, a tiny RAM, and a control unit that decides when to stop.

Features
Custom instruction set (ISA) with 5 opcodes
4 general-purpose 8-bit registers (R0–R3)
16 bytes of RAM holding the program
Program Counter (PC) with a highlighted view of the current memory cell
