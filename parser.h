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
void set_with_token_num(char *dest, int token_num, char *buffer);
void init_parser(char const *filename);
void close_parser(void);

#endif