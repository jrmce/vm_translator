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
  char filename_no_ext[MAX_FILENAME_LENGTH];

  if (argc < 2) {
    exit(1);
  }

  char *filename = argv[1];

  char *is_file = strstr(filename, ".vm");

  if (is_file == NULL) { // Directory
    init_writer(filename);
    parse_dir(buffer, filename);
  } else { // File
    get_filename_no_ext(filename, filename_no_ext);
    init_writer(filename_no_ext);
    parse_file(buffer, filename, filename_no_ext);
  }

  close_writer();

  return 0;
}
