#include "code.h"

#include <stdlib.h>
#include <string.h>

#include "parser.h"

void init_code(Code *code, char *buffer) {
  set_command_literal(code->command_literal, buffer);
  set_command_type(code);

  if (!code->command_type) {
    printf("Error parsing command type: %s\n", code->command_literal);
    exit(1);
  }

  set_arg_1(code, buffer);
  set_arg_2(code, buffer);
}