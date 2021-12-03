#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"
#include "constants.h"
#include "writer.h"

static int arithmetic_counter = 0;
static int call_counter = 0;
static char active_function_name[MAX_FUNC_NAME_LENGTH];
static FILE *fp_out;
static char MAIN_FN[] = "Main";
static char output_filename[MAX_FILENAME_LENGTH];

void init_writer(char *filename, bool is_dir) {
  strncpy(output_filename, filename, MAX_FILENAME_LENGTH - strlen(".asm"));
  if (is_dir) {
    strcat(output_filename, "/");
    strcat(output_filename, filename);
  }
  strcat(output_filename, ".asm");

  fp_out = fopen(output_filename, "w");

  if (fp_out == NULL) {
    printf("Error creating dest file: %s\n", filename);
    exit(1);
  }

  if (is_dir) {
    write_init();
  }
}

char *get_active_fn_name(void) {
  if (active_function_name[0] == '\0') {
    return MAIN_FN;
  } else {
    return active_function_name;
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

void push_basic_mem(Code *code, char *mem_loc) {
  fprintf(fp_out, "%s\n", mem_loc);
  int arg_2 = atoi(code->arg_2);
  write_command("A=M\n");
  if (arg_2 > 0) {
    for (int i = 1; i <= arg_2; i++) {
      write_command("A=A+1\n");
    }
  }
  write_command("D=M @SP A=M M=D @SP M=M+1\n");
}

void pop_basic_mem(Code *code, char *mem_loc) {
  write_command("@SP M=M-1 A=M D=M\n");
  fprintf(fp_out, "%s\n", mem_loc);
  int arg_2 = atoi(code->arg_2);
  write_command("A=M\n");
  if (arg_2 > 0) {
    for (int i = 1; i <= arg_2; i++) {
      write_command("A=A+1\n");
    }
  }
  write_command("M=D\n");
}

void write_init() {
  write_command("@256 D=A @SP M=D\n");
  write_command(
      "@0 D=A D=D-1 @LCL M=D D=D-1 @ARG M=D D=D-1 @THIS M=D D=D-1 @THAT M=D\n");
  write_call("Sys.init", "0");
}

void write_label(Code *code) {
  char *fn_name = get_active_fn_name();
  fprintf(fp_out, "(%s$%s)\n", fn_name, code->arg_1);
}

void write_goto(Code *code) {
  char *fn_name = get_active_fn_name();
  fprintf(fp_out, "@%s$%s\n0;JMP\n", fn_name, code->arg_1);
}

void write_if(Code *code) {
  char *fn_name = get_active_fn_name();
  write_command("@SP M=M-1 A=M D=M\n");
  fprintf(fp_out, "@%s$%s\n", fn_name, code->arg_1);
  write_command("D;JNE\n");
}

void write_call(char *arg_1, char *arg_2) {
  call_counter++;
  fprintf(fp_out, "@%s$ret.%d\n", arg_1, call_counter);
  write_command("D=A @SP A=M M=D @SP M=M+1\n");
  write_command("@LCL D=M @SP A=M M=D @SP M=M+1\n");
  write_command("@ARG D=M @SP A=M M=D @SP M=M+1\n");
  write_command("@THIS D=M @SP A=M M=D @SP M=M+1\n");
  write_command("@THAT D=M @SP A=M M=D @SP M=M+1\n");
  int n_args = atoi(arg_2);
  int arg_pos = 5 + n_args;
  write_command("@SP D=M\n");
  for (int i = 0; i < arg_pos; i++) {
    write_command("D=D-1\n");
  }
  write_command("@ARG M=D\n");
  write_command("@SP D=M @LCL M=D\n");
  fprintf(fp_out, "@%s\n", arg_1);
  write_command("0;JMP\n");
  fprintf(fp_out, "(%s$ret.%d)\n", arg_1, call_counter);
}

void write_function(Code *code) {
  strncpy(active_function_name, code->arg_1, MAX_FUNC_NAME_LENGTH);
  fprintf(fp_out, "(%s)\n", code->arg_1);
  int n_vars = atoi(code->arg_2);
  for (int i = 0; i < n_vars; i++) {
    write_command("@0 D=A @SP A=M M=D @SP M=M+1\n");
  }
}

void write_return(void) {
  write_command("@LCL D=M @END_FRAME M=D\n");
  for (int i = 0; i < 5; i++) {
    write_command("M=M-1\n");
  }
  write_command(
      "A=M D=M @RET_ADDR M=D @SP M=M-1 A=M D=M @ARG A=M M=D @ARG D=M+1 "
      "@SP M=D @END_FRAME M=M+1 A=M D=M @LCL M=D @END_FRAME M=M+1 "
      "A=M D=M @ARG M=D @END_FRAME M=M+1 A=M D=M @THIS M=D "
      "@END_FRAME M=M+1 A=M D=M @THAT M=D @RET_ADDR A=M 0;JMP\n");
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
    write_command("@SP M=M-1 A=M D=M @SP M=M-1 A=M D=D-M\n");
    fprintf(fp_out, "@EQ_%d\n", arithmetic_counter);
    write_command("D;JEQ @SP A=M M=0\n");
    fprintf(fp_out, "@EQEND_%d\n", arithmetic_counter);
    write_command("0;JMP\n");
    fprintf(fp_out, "(EQ_%d)\n", arithmetic_counter);
    write_command("@SP A=M M=-1\n");
    fprintf(fp_out, "(EQEND_%d)\n", arithmetic_counter);
    write_command("@SP M=M+1\n");
  } else if (strcmp(code->arg_1, "lt") == 0) {
    write_command("@SP M=M-1 A=M D=M @SP M=M-1 A=M D=M-D\n");
    fprintf(fp_out, "@LT_%d\n", arithmetic_counter);
    write_command("D;JLT @SP A=M M=0\n");
    fprintf(fp_out, "@LTEND_%d\n", arithmetic_counter);
    write_command("0;JMP\n");
    fprintf(fp_out, "(LT_%d)\n", arithmetic_counter);
    write_command("@SP A=M M=-1\n");
    fprintf(fp_out, "(LTEND_%d)\n", arithmetic_counter);
    write_command("@SP M=M+1\n");
  } else if (strcmp(code->arg_1, "gt") == 0) {
    write_command("@SP M=M-1 A=M D=M @SP M=M-1 A=M D=M-D\n");
    fprintf(fp_out, "@GT_%d\n", arithmetic_counter);
    write_command("D;JGT @SP A=M M=0\n");
    fprintf(fp_out, "@GTEND_%d\n", arithmetic_counter);
    write_command("0;JMP\n");
    fprintf(fp_out, "(GT_%d)\n", arithmetic_counter);
    write_command("@SP A=M M=-1\n");
    fprintf(fp_out, "(GTEND_%d)\n", arithmetic_counter);
    write_command("@SP M=M+1\n");
  }
}

void write_push_pop(Code *code) {
  if (code->command_type == PUSH) {
    if (strcmp(code->arg_1, "constant") == 0) {
      fprintf(fp_out, "@%s\n", code->arg_2);
      write_command("D=A @SP A=M M=D @SP M=M+1\n");
    } else if (strcmp(code->arg_1, "local") == 0) {
      push_basic_mem(code, "@LCL");
    } else if (strcmp(code->arg_1, "argument") == 0) {
      push_basic_mem(code, "@ARG");
    } else if (strcmp(code->arg_1, "this") == 0) {
      push_basic_mem(code, "@THIS");
    } else if (strcmp(code->arg_1, "that") == 0) {
      push_basic_mem(code, "@THAT");
    } else if (strcmp(code->arg_1, "static") == 0) {
      fprintf(fp_out, "@%s.%s\n", code->filename, code->arg_2);
      write_command("D=M @SP A=M M=D @SP M=M+1\n");
    } else if (strcmp(code->arg_1, "pointer") == 0) {
      int arg_2 = atoi(code->arg_2);
      if (arg_2 == 0) {
        write_command("@THIS\n");
      } else if (arg_2 == 1) {
        write_command("@THAT\n");
      }
      write_command("D=M @SP A=M M=D @SP M=M+1\n");
    } else if (strcmp(code->arg_1, "temp") == 0) {
      int arg_2 = atoi(code->arg_2);
      int loc = 5;
      if (arg_2 > 0) {
        for (int i = 1; i <= arg_2; i++) {
          loc++;
        }
        fprintf(fp_out, "@%d\n", loc);
      }
      write_command("D=M @SP A=M M=D @SP M=M+1\n");
    }
  } else if (code->command_type == POP) {
    if (strcmp(code->arg_1, "local") == 0) {
      pop_basic_mem(code, "@LCL");
    } else if (strcmp(code->arg_1, "argument") == 0) {
      pop_basic_mem(code, "@ARG");
    } else if (strcmp(code->arg_1, "this") == 0) {
      pop_basic_mem(code, "@THIS");
    } else if (strcmp(code->arg_1, "that") == 0) {
      pop_basic_mem(code, "@THAT");
    } else if (strcmp(code->arg_1, "static") == 0) {
      write_command("@SP M=M-1 A=M D=M\n");
      fprintf(fp_out, "@%s.%s\n", code->filename, code->arg_2);
      write_command("M=D\n");
    } else if (strcmp(code->arg_1, "pointer") == 0) {
      write_command("@SP M=M-1 A=M D=M\n");
      int arg_2 = atoi(code->arg_2);
      if (arg_2 == 0) {
        write_command("@THIS\n");
      } else if (arg_2 == 1) {
        write_command("@THAT\n");
      }
      write_command("M=D\n");
    } else if (strcmp(code->arg_1, "temp") == 0) {
      write_command("@SP M=M-1 A=M D=M\n");
      int arg_2 = atoi(code->arg_2);
      int loc = 5;
      if (arg_2 > 0) {
        for (int i = 1; i <= arg_2; i++) {
          loc++;
        }
      }
      fprintf(fp_out, "@%d\n", loc);
      write_command("M=D\n");
    }
  }
}

void close_writer() {
  write_command("(END) @END 0;JEQ");
  fclose(fp_out);
}
