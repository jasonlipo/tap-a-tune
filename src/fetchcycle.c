#include <stdio.h>
#include "emulate.h"

void fetch_decode_execute(struct ARM proc) {

  proc.pc = 0;
  proc.ir = 0;

  do {

    // Get the next instruction and increment PC
    proc.ir = proc.memory[proc.pc];
    proc.pc++;

    if (check_condition_bits(proc) == 1) {

      /*
        switch (get_instruction_type(&ir)) {

          case DATA_PROCESSING:
          data_processing(&ir);
          break;

          case BRANCH:
          branch(&ir);
          break;

          case MULTIPLY:
          multiply(&ir);
          break;

          case SINGLE_DATA_TRANSFER:
          single_data_transfer(&ir);
          break;
        }

      */

    }

  }
  while (proc.ir != 0);

}

int check_condition_bits(struct ARM proc) {

  // Get the 4 most significant bits which is the "Cond"
  int cond = extract_bits(&proc.ir, 28, 4);

  unsigned int v = extract_bit(proc.registers, CPSR_V);
  unsigned int c = extract_bit(proc.registers, CPSR_C);
  unsigned int z = extract_bit(proc.registers, CPSR_Z);
  unsigned int n = extract_bit(proc.registers, CPSR_N);

  switch (cond) {

    // Z set
    case 0:
    return z == 1;

    // Z clear
    case 1:
    return z == 0;

    // N equals V
    case 10:
    return n == v;

    // N not equal to V
    case 11:
    return n != v;

    // Z clear AND (N equals V)
    case 12:
    return z == 0 && n == v;

    // Z set OR (N not equals to V)
    case 13:
    return z == 1 || n != v;

    // Al flag
    case 14:
    return 1;

    default:
    fprintf(stderr, "Unknown condition");
    return 0;

  }

}