#ifndef util_h
#define util_h

#include <stdbool.h>
#include <stdio.h>

bool is_empty(char* line);
bool is_comment(char* line);
void get_filename_no_ext(char *filename, char *dest);

#endif
