#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "./util.h"

bool is_empty(char* line) {
  while (*line != '\0') {
    if (!isspace((unsigned char)*line)) {
      return false;
    }
    line++;
  }
  return true;
}

bool is_comment(char* line) {
  int slashes = 0;
  while (*line != '\0' && *line == '/') {
    slashes++;

    if (slashes == 2) {
      return true;
    }
  }

  return false;
}