#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char value)
{
  cpu->ram[index] = value;
  return;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
// void cpu_load(struct cpu *cpu, int argc, char *argv[])
void cpu_load(char *filename, struct cpu *cpu)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded
  // if (argc <= 1)
  // {
  //   printf("You must provide a filename with instructions.\n");
  //   exit(1);
  // }
  // else if (argc > 2)
  // {
  //   printf("WARNING: Too many command line elements.\n");
  // }
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
  {
    printf("Error opening the requested file.\n");
    exit(1);
  }
  int address = BASE_PROGRAM;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, fp)) != -1)
  {
    if (!strcmp(line, "\n"))
    {
      continue;
    }
    // int linelength;
    if (strstr(line, "#") != NULL)
    {
      int diff = strstr(line, "#") - line;
      char pre_comment[diff + 1];
      memcpy(pre_comment, line, diff);
      pre_comment[diff] = '\0';
      line = pre_comment;
      printf("%lu\n", strtoul(line, NULL, 2));
    }
    
    // cpu->ram[address++] = strtoul(line, &(line + linelength), 2);
    // cpu->ram[address++] = strtoul(line, NULL, 2);
    cpu_ram_write(cpu, address, strtoul(line, NULL, 2));
    line = NULL;
    address++;
  }
}

/**
 * ALU
 */
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op) {
//     case ALU_MUL:
//       // TODO
//       break;

//     // TODO: implement more ALU ops
//   }
// }

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  cpu->pc = BASE_PROGRAM;

  for (int i = 0; i < 8; i++)
  {
    printf("%d\n", cpu_ram_read(cpu, i));
  }

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    int instruction = cpu_ram_read(cpu, cpu->pc);
    printf("%d\n", instruction);
    // 2. Figure out how many operands this next instruction requires
    int num_operands = instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    int *operands = malloc(num_operands);
    for (int i = 0; i < num_operands; i++)
    {
      operands[i] = cpu->ram[cpu->pc + 1 + i];
    }
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch(instruction)
    {
    // Binary value 130, LDI instruction
    case 0b10000010:
      cpu->reg[operands[0]] = operands[1];
      break;
    // Binary value 71, PRINT instruction
    case 0b01000111:
      printf("%d\n", cpu->reg[operands[0]]);
      break;
    // Binary value 1, HALT instruction
    case 0b00000001:
      running = 0;
      break;
    // Binary value 162, multiply
    case 0b10100010:
      cpu->reg[operands[0]] = cpu->reg[operands[0]] * cpu->reg[operands[1]];
      break;
    // Other value, no matching instruction found
    default:
      printf("That instruction %d was not found.\n", instruction);
      exit(1);
    }
    // 6. Move the PC to the next instruction.
    cpu->pc += (num_operands + 1);
    free(operands);
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  memset(&(cpu->reg), 0, 8 * sizeof(unsigned char));
  memset(&(cpu->ram), 0, 256 * sizeof(unsigned char));
}
