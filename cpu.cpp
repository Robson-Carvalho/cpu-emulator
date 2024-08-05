#include <bits/stdc++.h>

using namespace std;

#define NUM_REGISTERS 8
#define MEMORY_SIZE 256

typedef struct
{
  int registers[NUM_REGISTERS];
  int pc;
  int memory[MEMORY_SIZE];
  int running;
} CPU;

void initializeCPU(CPU *cpu)
{
  memset(cpu->registers, 0, sizeof(cpu->registers));
  cpu->pc = 16;
  memset(cpu->memory, 0, sizeof(cpu->memory));
  cpu->running = 1;
}

#define LOAD 0x01
#define STORE 0x02
#define ADD 0x03
#define SUB 0x04
#define JMP 0x05
#define JZ 0x06
#define JNZ 0x07
#define HALT 0xFF

void executeLOAD(CPU *cpu, int reg, int address)
{
  cpu->registers[reg] = cpu->memory[address];
}

void executeSTORE(CPU *cpu, int reg, int address)
{
  cpu->memory[address] = cpu->registers[reg];
}

void executeADD(CPU *cpu, int reg1, int reg2, int reg3)
{
  cpu->registers[reg1] = cpu->registers[reg2] + cpu->registers[reg3];
}

void executeSUB(CPU *cpu, int reg1, int reg2, int reg3)
{
  cpu->registers[reg1] = cpu->registers[reg2] - cpu->registers[reg3];
}

void executeJMP(CPU *cpu, int address)
{
  cpu->pc = address;
}

void executeJZ(CPU *cpu, int reg, int address)
{
  if (cpu->registers[reg] == 0)
  {
    cpu->pc = address;
  }
  else
  {
    cpu->pc += 3;
  }
}

void executeJNZ(CPU *cpu, int reg, int address)
{
  if (cpu->registers[reg] != 0)
  {
    cpu->pc = address;
  }
  else
  {
    cpu->pc += 3;
  }
}

void executeHALT(CPU *cpu)
{
  cpu->running = 0;
}

void runCPU(CPU *cpu)
{
  while (cpu->running)
  {
    int opcode = cpu->memory[cpu->pc];
    switch (opcode)
    {
    case LOAD:
      executeLOAD(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
      cpu->pc += 3;
      break;
    case STORE:
      executeSTORE(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
      cpu->pc += 3;
      break;
    case ADD:
      executeADD(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2], cpu->memory[cpu->pc + 3]);
      cpu->pc += 4;
      break;
    case SUB:
      executeSUB(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2], cpu->memory[cpu->pc + 3]);
      cpu->pc += 4;
      break;
    case JMP:
      executeJMP(cpu, cpu->memory[cpu->pc + 1]);
      break;
    case JZ:
      executeJZ(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
      break;
    case JNZ:
      executeJNZ(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
      break;
    case HALT:
      executeHALT(cpu);
      break;
    default:
      printf("Unknown opcode %02X at address %02X\n", opcode, cpu->pc);
      cpu->running = 0;
      break;
    }
  }
}

void loadProgram(CPU *cpu, int *program, int programSize)
{
  for (int i = 0; i < programSize; i++)
  {
    cpu->memory[cpu->pc + i] = program[i];
  }
}

int main()
{
  CPU cpu;
  initializeCPU(&cpu);

  int program[] =
      {LOAD, 0, 0,
       LOAD, 1, 1,
       ADD, 2, 0, 1,
       STORE, 2, 5,
       HALT};

  cpu.memory[0] = 3;
  cpu.memory[1] = 5;

  loadProgram(&cpu, program, sizeof(program) / sizeof(int));

  runCPU(&cpu);

  cout << "Value at address 5: " << cpu.memory[5] << endl;

  return 0;
}
