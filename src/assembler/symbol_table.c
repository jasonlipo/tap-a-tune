#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assemble.h"

int build_symbol_table(int total_size, char** instructions, SymbolTable* st) {

  int instruction_count = 0;

  for (int i=0; i<total_size; i++) {

    // Find the label by splitting at ':'
    char* pos = strchr(instructions[i], ':');

    if (pos != NULL) {

      int index = pos - instructions[i];
      char* label = malloc(index+1);
      strncpy(label, instructions[i], index);
      label[index] = '\0';

      // Found a label, store in symbol table
      int address = instruction_count * WORD_SIZE;
      add_label(st, label, address);

    }
    else if (strcmp(trim_whitespace(instructions[i]), "")) {
      instruction_count++;
    }

  }

  return instruction_count;

}
