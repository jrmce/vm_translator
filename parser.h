#ifndef parser_h
#define parser_h

#include <stdbool.h>
#include <stdio.h>

#include "code.h"

bool get_next_line(char *buffer);
void set_command_literal(char *command, char *buffer);
int set_command_type(Code *code);
void set_arg_1(Code *code, char *buffer);
void set_arg_2(Code *code, char *buffer);
void close_parser(void);
void parse_dir(char *buffer, char *dirname);
void parse_file(char *buffer, char *filename, char *filename_no_ext);

#endif
