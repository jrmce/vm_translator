#ifndef writer_h
#define writer_h

#include "code.h"
#include <stdio.h>

void write_arithmetic(Code *code);
void write_push_pop(Code *code);
void init_writer(char const *filename);
void close_writer(void);

#endif