#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"
#include "constants.h"
#include "util.h"
#include "writer.h"

static FILE *fp;

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

void parse_file(char *buffer, char *filename, char *filename_no_ext) {
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error opening source file: %s\n", filename);
    exit(1);
  }

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
  fclose(fp);
}

void parse_dir(char *buffer, char *dirname) {
  char filename_no_ext[150];
  DIR *dr = opendir(dirname);

  if (dr == NULL) {
    printf("Error opening directory: %s", dirname);
  }

  struct dirent *de;

  while ((de = readdir(dr)) != NULL) {
    if (strstr(de->d_name, ".vm") != NULL) {
      get_filename_no_ext(de->d_name, filename_no_ext);
      char dirname_c[150];
      strcpy(dirname_c, dirname);
      strcat(dirname_c, "/");
      strcat(dirname_c, de->d_name);
      parse_file(buffer, dirname_c, filename_no_ext);
    }
  }

  closedir(dr);
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
