#ifndef INCLUDE_CONSOLE_H
#define INCLUDE_CONSOLE_H

#include "common.h"

typedef struct {
	char* name;
	char* default_value;
	int flag;
	int required;
} sb_parameter;

typedef struct {
	int arg_count;
	char** args;
	int par_count;
	sb_parameter parameters[128];
} sb_parameter_parser;

SB_EXTERN(void) sb_parameter_initialize_parser(sb_parameter_parser* parser, int argc, char** argv);
SB_EXTERN(void) sb_parameter_create(sb_parameter_parser* parser, char* name, char* default_value, int flag, int required);
SB_DEF_EXTERN sb_parameter_get_value(sb_parameter_parser* parser, char** out, char* name);

#endif
