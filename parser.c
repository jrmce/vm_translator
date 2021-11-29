#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"
#include "constants.h"
#include "util.h"

static FILE *fp;

void init_parser(char const *filename) {
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error opening source file: %s\n", filename);
    exit(1);
  }
}

void set_with_token_num(char *dest, int token_num, char *buffer) {
  char temp[BUFFER_LENGTH];
  strcpy(temp, buffer);
  char *token = strtok(temp, " ");
  int count = 0;
  while (token && count < token_num) {
    count++;
    token = strtok(NULL, " ");
  }
  strcpy(dest, token);
}

bool get_next_line(char *buffer) {
  if (fgets(buffer, BUFFER_LENGTH, fp) != NULL) {
    while (is_empty(buffer) || is_comment(buffer)) {
      fgets(buffer, BUFFER_LENGTH, fp);
    }
    return true;
  } else {
    return false;
  }
}

void set_command_literal(char *command, char *buffer) {
  int i = 0;
  while (!isspace(buffer[i]) && buffer[i] != '\0') {
    command[i] = buffer[i];
    i++;
  }

  command[i] = '\0';
}

int set_command_type(Code *code) {
  if (strcmp(code->command_literal, "push") == 0) {
    code->command_type = PUSH;
  } else if (strcmp(code->command_literal, "pop") == 0) {
    code->command_type = POP;
  } else if (strcmp(code->command_literal, "add") == 0 ||
             strcmp(code->command_literal, "sub") == 0 ||
             strcmp(code->command_literal, "neg") == 0 ||
             strcmp(code->command_literal, "eq") == 0 ||
             strcmp(code->command_literal, "not") == 0 ||
             strcmp(code->command_literal, "lt") == 0 ||
             strcmp(code->command_literal, "gt") == 0 ||
             strcmp(code->command_literal, "and") == 0 ||
             strcmp(code->command_literal, "or") == 0) {
    code->command_type = ARITHMETIC;
  }
  return 0;
}

void set_arg_1(Code *code, char *buffer) {
  if (code->command_type == ARITHMETIC) {
    sscanf(buffer, "%s %*s %*s", code->arg_1);
  } else {
    sscanf(buffer, "%*s %s %*s", code->arg_1);
  }
}

void set_arg_2(Code *code, char *buffer) {
  if (code->command_type == POP || code->command_type == PUSH ||
      code->command_type == FUNCTION || code->command_type == CALL) {
    sscanf(buffer, "%*s %*s %s", code->arg_2);
  }
}

void close_parser() { fclose(fp); }
