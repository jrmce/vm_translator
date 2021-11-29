#ifndef code_h
#define code_h

#define ARG_LENGTH 50

typedef struct {
  char command_literal[10];
  int command_type;
  char arg_1[ARG_LENGTH];
  char arg_2[ARG_LENGTH];
  char filename[ARG_LENGTH];
} Code;

void init_code(Code* code, char* buffer, const char* filename);

#endif
