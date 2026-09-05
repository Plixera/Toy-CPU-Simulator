#include <iostream>
#include <cstdint>
#include <thread>
#include <chrono>

// Explicit std declarations
using std::cout;
using std::string;
using std::to_string;
using std::hex;
using std::dec;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

// Instruction Set Architecture (ISA)
enum Opcodes : uint8_t 
{
    NOP = 0x00,    // No Operation
    LOAD = 0x01,   // Load a literal value into a register
    ADD = 0x02,    // Add one register into another
    PRINT = 0x03,  // Output a Register's value
    HALT = 0xFF    // Reserving the maximum value for "STOP instruction"
};

struct CPU // A data structure that represents our Virtual CPU
{
    uint8_t registers[4] = {0}; // CPU has 4 registers, where each register will store 8 bit of data
    uint16_t pc = 0;     // PC = Program Counter. It holds the memory address of next instruction. Used a 16 bit PC, althouhgh 8 bit is sufficient. Would change it later
    bool running = true;
    uint8_t memory[32] = {0};   // Reoresents CPU's RAM, which has 32 bytes of storage

    void printState(const string &currentInstruction);
    void runStepByStep();
};

void CPU::printState(const string &currentInstruction)
{
    cout << "\033[2J\033[1;1H";

    cout << "=======================================\n";
    cout << "          CPU SIMULATOR STATE          \n";
    cout << "=======================================\n";
    cout << "Program Counter (PC): 0x" << hex << pc << dec << "\n";
    cout << "Executing           : " << currentInstruction << "\n\n";

    cout << "REGISTERS:\n";
    for (int i = 0; i < 4; ++i)
    {
        cout << "  R" << i << ": " << (int)registers[i] << "\n";
    }

    cout << "\nRAM MEMORY (First 16 Bytes):\n  ";
    for (int i = 0; i < 16; ++i)
    {
        if (i == pc)
            cout << "[" << hex << (int)memory[i] << "] ";
        else
            cout << " " << hex << (int)memory[i] << "  ";
    }
    cout << "\n=======================================\n";
}

void CPU::runStepByStep()
{
    while (running && pc < sizeof(memory))
    {
        uint8_t opcode = memory[pc++];

        switch (opcode)
        {
        case LOAD:
        {
            uint8_t reg = memory[pc++];
            uint8_t val = memory[pc++];
            registers[reg] = val;
            printState("LOAD R" + to_string(reg) + " " + to_string(val));
            break;
        }
        case ADD:
        {
            uint8_t regA = memory[pc++];
            uint8_t regB = memory[pc++];
            registers[regA] += registers[regB];
            printState("ADD R" + to_string(regA) + " R" + to_string(regB));
            break;
        }
        case PRINT:
        {
            uint8_t reg = memory[pc++];
            printState("PRINT R" + to_string(reg));
            cout << ">> OUTPUT: R" << (int)reg << " = " << (int)registers[reg] << "\n";
            break;
        }
        case HALT:
            running = false;
            printState("HALT");
            break;
        }

        sleep_for(milliseconds(1500));
    }
}

int main()
{
    CPU cpu;
    uint8_t program[] = {
        LOAD, 0, 5,  
        LOAD, 1, 10, 
        ADD, 0, 1,   
        PRINT, 0,    
        HALT};

    for (size_t i = 0; i < sizeof(program); ++i)
        cpu.memory[i] = program[i];

    cpu.runStepByStep();
    return 0;
}