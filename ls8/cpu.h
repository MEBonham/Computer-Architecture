#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char pc;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
  // flags
  unsigned char equal_flag;
  unsigned char less_than_flag;
  unsigned char greater_than_flag;
};

// ALU operations
enum alu_op {
	ALU_MUL
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
// TODO: more instructions here. These can be used in cpu_run().

// Memory locations
#define BASE_PROGRAM 0x00
#define STACK_POINTER 7
#define STACK_HIGH 0xf4

// Function declarations

// extern void cpu_load(struct cpu *cpu, int argc, char *argv[]);
extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
