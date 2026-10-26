#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <time.h>

static void sleep_ms(int ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}
#endif

// Instruction Set Architecture (ISA)
enum Opcodes
{
    NOP   = 0x00, // No Operation
    LOAD  = 0x01, // Load a literal value into a register
    ADD   = 0x02, // Add one register into another
    PRINT = 0x03, // Output a register's value
    HALT  = 0xFF  // Reserving the maximum value for "STOP instruction"
};

#define NUM_REGISTERS 4
#define MEMORY_SIZE   16

typedef struct // A data structure that represents our Virtual CPU
{
    uint8_t registers[NUM_REGISTERS]; // 4 registers, each storing 8 bits
    uint16_t pc;                      // Program Counter: address of next instruction
    bool running;                     // Should the CPU keep executing? (Control Unit)
    uint8_t memory[MEMORY_SIZE];      // CPU's RAM: 16 bytes
} CPU;

// In C there are no member functions, so we pass a pointer to the struct instead
void cpu_init(CPU *cpu)
{
    for (int i = 0; i < NUM_REGISTERS; ++i)
        cpu->registers[i] = 0;
    for (int i = 0; i < MEMORY_SIZE; ++i)
        cpu->memory[i] = 0;
    cpu->pc = 0;
    cpu->running = true;
}

void cpu_print_state(const CPU *cpu, const char *current_instruction)
{
    printf("\033[2J\033[1;1H"); // ANSI: clear screen, move cursor to top-left

    printf("\n");
    printf("*************************************\n");
    printf("          TOY CPU SIMULATOR          \n");
    printf("*************************************\n");
    printf("Program Counter (PC): 0x%X\n", cpu->pc);
    printf("Executing           : %s\n\n", current_instruction);

    printf("REGISTERS:\n");
    for (int i = 0; i < NUM_REGISTERS; ++i)
        printf("  R%d: %d\n", i, cpu->registers[i]);

    printf("\nRAM MEMORY (16 Bytes):\n  ");
    for (int i = 0; i < MEMORY_SIZE; ++i)
    {
        if (i == cpu->pc)
            printf("[%x] ", cpu->memory[i]);
        else
            printf(" %x  ", cpu->memory[i]);
    }
    printf("\n");
    fflush(stdout); // make sure it shows up before we sleep
}

void cpu_run_step_by_step(CPU *cpu)
{
    char buffer[64]; // replaces std::string for building the instruction text

    while (cpu->running && cpu->pc < MEMORY_SIZE)
    {
        uint8_t opcode = cpu->memory[cpu->pc++];

        switch (opcode)
        {
        case LOAD:
        {
            uint8_t reg = cpu->memory[cpu->pc++];
            uint8_t val = cpu->memory[cpu->pc++];
            cpu->registers[reg] = val;
            snprintf(buffer, sizeof(buffer), "LOAD R%d %d", reg, val);
            cpu_print_state(cpu, buffer);
            break;
        }
        case ADD:
        {
            uint8_t reg_a = cpu->memory[cpu->pc++];
            uint8_t reg_b = cpu->memory[cpu->pc++];
            cpu->registers[reg_a] += cpu->registers[reg_b];
            snprintf(buffer, sizeof(buffer), "ADD R%d R%d", reg_a, reg_b);
            cpu_print_state(cpu, buffer);
            break;
        }
        case PRINT:
        {
            uint8_t reg = cpu->memory[cpu->pc++];
            snprintf(buffer, sizeof(buffer), "PRINT R%d", reg);
            cpu_print_state(cpu, buffer);
            printf(">> OUTPUT: R%d = %d\n", reg, cpu->registers[reg]);
            fflush(stdout);
            break;
        }
        case HALT:
            cpu->running = false;
            cpu_print_state(cpu, "HALT");
            break;
        default: // NOP or unknown opcode: do nothing
            break;
        }

        sleep_ms(1500);
    }
}

int main(void)
{
    CPU cpu;
    cpu_init(&cpu);

    uint8_t program[] = {
        LOAD, 0, 5,
        LOAD, 1, 10,
        ADD, 0, 1,
        PRINT, 0,
        HALT};

    for (size_t i = 0; i < sizeof(program); ++i)
        cpu.memory[i] = program[i];

    cpu_run_step_by_step(&cpu);
    return 0;
}