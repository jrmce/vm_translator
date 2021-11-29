#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"
#include "constants.h"
#include "parser.h"
#include "util.h"
#include "writer.h"

int main(int argc, char *argv[]) {
  char buffer[BUFFER_LENGTH];
  char filename_no_ext[150];

  if (argc < 2) {
    exit(1);
  }

  char *filename = argv[1];

  init_parser(filename);
  init_writer(filename);

  int i = 0;
  while (filename[i] != '/') {
    if (filename[i] == '.') {
      i = -1;
      break;
    }
    i++;
  }
  strncpy(filename_no_ext, &filename[i + 1], 150);
  strtok(filename_no_ext, ".");

  // Writes line to buffer and returns whether there are
  // more commands
  bool has_more_commands = get_next_line(buffer);

  while (has_more_commands) {
    Code code;
    init_code(&code, buffer, filename_no_ext);

    switch (code.command_type) {
      case ARITHMETIC:
        write_arithmetic(&code);
        break;
      case PUSH:
      case POP:
        write_push_pop(&code);
        break;
      default:
        break;
    }

    has_more_commands = get_next_line(buffer);
  }

  close_parser();
  close_writer();

  return 0;
}
