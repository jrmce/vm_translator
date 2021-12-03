#ifndef writer_h
#define writer_h

#include "code.h"
#include <stdio.h>

void write_arithmetic(Code *code);
void write_push_pop(Code *code);
void init_writer(char *filename);
void close_writer(void);
void write_init(void);
void write_label(Code *code);
void write_goto(Code *code);
void write_if(Code *code);
// void write_call(Code *code);
void write_call(char *arg_1, char *arg_2);
void write_function(Code *code);
void write_return(void);

#endif
