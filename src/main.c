#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "code.h"
#include "constants.h"
#include "parser.h"
#include "util.h"
#include "writer.h"

int main(int argc, char const *argv[]) {
  clock_t start;
  clock_t end;
  double cpu_time_used;
  start = clock();
  char buffer[BUFFER_LENGTH];

  if (argc < 2) {
    exit(1);
  }

  char const *filename = argv[1];
  init_parser(filename);
  init_writer(filename);

  // Writes line to buffer and returns whether there are
  // more commands
  bool has_more_commands = get_next_line(buffer);

  while (has_more_commands) {
    Code code;
    init_code(&code, buffer);

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

  end = clock();
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("main() took %f seconds to execute \n", cpu_time_used);
  return 0;
}
