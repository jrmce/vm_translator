#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"
#include "constants.h"

static int arithmetic_counter = 0;
static FILE *fp_out;

void init_writer(char const *filename) {
  char output_filename[20];
  strncpy(output_filename, filename, 20);
  strtok(output_filename, ".");
  strcat(output_filename, ".asm");
  fp_out = fopen(output_filename, "w");
  if (fp_out == NULL) {
    printf("Error creating dest file: %s\n", filename);
    exit(1);
  }
}

void write_command(char *command) {
  while (*command != '\0') {
    if (isspace(*command)) {
      putc('\n', fp_out);
      command++;
      continue;
    }

    putc(*command, fp_out);
    command++;
  }
}

void write_arithmetic(Code *code) {
  arithmetic_counter++;

  if (strcmp(code->arg_1, "add") == 0) {
    write_command("@SP M=M-1 A=M D=M @SP M=M-1 A=M M=D+M @SP M=M+1\n");
  } else if (strcmp(code->arg_1, "sub") == 0) {
    write_command("@SP M=M-1 A=M D=M @SP M=M-1 A=M M=M-D @SP M=M+1\n");
  } else if (strcmp(code->arg_1, "neg") == 0) {
    write_command("@SP M=M-1 A=M M=-M @SP M=M+1\n");
  } else if (strcmp(code->arg_1, "and") == 0) {
    write_command("@SP M=M-1 A=M D=M @SP M=M-1 A=M M=D&M @SP M=M+1\n");
  } else if (strcmp(code->arg_1, "or") == 0) {
    write_command("@SP M=M-1 A=M D=M @SP M=M-1 A=M M=D|M @SP M=M+1\n");
  } else if (strcmp(code->arg_1, "not") == 0) {
    write_command("@SP M=M-1 A=M M=!M @SP M=M+1\n");
  } else if (strcmp(code->arg_1, "eq") == 0) {
    write_command("@SP M=M-1 A=M D=M @SP M=M-1 A=M D=D-M @EQ_");
    fprintf(fp_out, "%d\n", arithmetic_counter);
    write_command("D;JEQ @SP A=M M=0 @EQEND_");
    fprintf(fp_out, "%d\n", arithmetic_counter);
    write_command("0;JMP (EQ_");
    fprintf(fp_out, "%d", arithmetic_counter);
    write_command(") @SP A=M M=-1 (EQEND_");
    fprintf(fp_out, "%d", arithmetic_counter);
    write_command(") @SP M=M+1\n");
  } else if (strcmp(code->arg_1, "lt") == 0) {
    write_command("@SP M=M-1 A=M D=M @SP M=M-1 A=M D=M-D @LT_");
    fprintf(fp_out, "%d\n", arithmetic_counter);
    write_command("D;JLT @SP A=M M=0 @LTEND_");
    fprintf(fp_out, "%d\n", arithmetic_counter);
    write_command("0;JMP (LT_");
    fprintf(fp_out, "%d", arithmetic_counter);
    write_command(") @SP A=M M=-1 (LTEND_");
    fprintf(fp_out, "%d", arithmetic_counter);
    write_command(") @SP M=M+1\n");
  } else if (strcmp(code->arg_1, "gt") == 0) {
    write_command("@SP M=M-1 A=M D=M @SP M=M-1 A=M D=M-D @GT_");
    fprintf(fp_out, "%d\n", arithmetic_counter);
    write_command("D;JGT @SP A=M M=0 @GTEND_");
    fprintf(fp_out, "%d\n", arithmetic_counter);
    write_command("0;JMP (GT_");
    fprintf(fp_out, "%d", arithmetic_counter);
    write_command(") @SP A=M M=-1 (GTEND_");
    fprintf(fp_out, "%d", arithmetic_counter);
    write_command(") @SP M=M+1\n");
  }
}

void write_push_pop(Code *code) {
  if (strcmp(code->arg_1, "constant") == 0) {
    fprintf(fp_out, "@%s\n", code->arg_2);
    write_command("D=A @SP A=M M=D @SP M=M+1\n");
  }
}

void close_writer() {
  write_command("(END) @END 0;JEQ");
  fclose(fp_out);
}
