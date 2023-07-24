#include <stdio.h>
#include "../includes/CPU/kCPU.h"

int main()
{
  startCPU();
  qword *instructions = (qword *)malloc(sizeof(qword) * 10);
  // program for 1 billion loops
  // instructions[0] = 0x280000000000401;
  // instructions[0] = 0x280003B9ACACA01;
  // instructions[1] = 0x280000000000000;
  // instructions[2] = 0x100000000000000;
  // instructions[3] = 0x300000000000001;
  // instructions[4] = 0x400000000000002;
  // instructions[5] = 0x500000000000000;
  // instructions[6] = 0x200000000000000;

  //program for kALU add and iadd testing: since other instruction are similar: this testing would suffice
  instructions[0] = 0x801000000000800; // mov ka, 8
  instructions[1] = 0x801000000000801; // mov kb, 8
  instructions[2] = 0x100000000000001; // add ka, kb ; also store the result in ka
  instructions[3] = 0x500000000000001; // iadd ka, kb ; the values where automatically put there beforehand ka = kb = -8
  instructions[4] = 0x801000000001001; // mov kb, 16
  instructions[5] = 0x500000000000001; // iadd ka, kb ; should be 0
  loadInstructions(instructions, 3);
  execute();
}
