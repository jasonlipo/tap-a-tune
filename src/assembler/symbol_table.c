#include "assemble.h"

/**
 * The first-pass of the assembler is to find all labels and put them in
 * the symbol table.
 * Whilst doing this, keep a track of the total number of instructions
 * excluding the labels, as this cannot be calculated from the file size.
 * @param total_size - The total size of the file
 * @param instructions - The pointer to the instructions array
 * @param st - A pointer to the symbol table to load the labels into
 * @returns the new instruction count (without labels)
 */
int build_symbol_table(int total_size, char **instructions, SymbolTable *st) {

  int instruction_count = 0;

  // Create label pointer
  char* label;

  for (int i=0; i<total_size; i++) {

    // Find the label by splitting at ':'
    char *pos = strchr(instructions[i], ':');

    if (pos != NULL) {

      // This line is a label
      int index = pos - instructions[i];
      label = calloc(index+1, sizeof(char));

      if (label == NULL) {
        perror("label calloc in build_symbol_table");
        exit(EXIT_FAILURE);
      }


      strncpy(label, instructions[i], index);

      // Store in symbol table
      int address = instruction_count * WORD_SIZE;
      add_label(st, label, address);


    }
    else if (strcmp(trim_whitespace(instructions[i]), "") && instructions[i][0] != '-') {
      // This is not a label, empty line or a comment, add it to the instruction count
      instruction_count++;
    }
  }
  return instruction_count;

}
