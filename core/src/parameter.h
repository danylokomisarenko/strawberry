#ifndef PARAMETER_H
#define PARAMETER_H

#include <strawberry/console.h>

void sb_parameter_initialize_parser(sb_parameter_parser* parser, int argc, char** argv);
void sb_parameter_initialize(sb_parameter* parameter, char* name, char* default_value, int flag, int required);
void sb_parameter_create(sb_parameter_parser* parser, char* name, char* default_value, int flag, int required);
sb_code sb_parameter_get(sb_parameter_parser* parser, sb_parameter* out, char* name);
sb_code sb_parameter_get_value(sb_parameter_parser* parser, char** out, char* name);

#endif
