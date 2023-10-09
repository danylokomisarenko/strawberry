#include "parameter.h"

void sb_parameter_initialize_parser(sb_parameter_parser* out, int argc, char** argv) {
	sb_parameter_parser parser;
	parser.arg_count = argc;
	parser.args = argv;
	parser.par_count = 0;
	*out = parser;
}

void sb_parameter_initialize(sb_parameter* out, char* name, char* default_value, int flag, int required) {
	sb_parameter parser;
	parser.name = name;
	parser.default_value = default_value;
	parser.flag = flag;
	parser.required = required;
	*out = parser;
}

void sb_parameter_create(sb_parameter_parser* parser, char* name, char* default_value, int flag, int required) {
	sb_parameter parameter;
	sb_parameter_initialize(&parameter, name, default_value, flag, required);
	parser->parameters[parser->par_count] = parameter;
	parser->par_count++;
}

sb_code sb_parameter_get(sb_parameter_parser* parser, sb_parameter* out, char* name) {
	for (int i = 0; i < parser->par_count; i++) {
		sb_parameter parameter = parser->parameters[i];
		if (strcmp(parameter.name, name) == SB_OK) {
			*out = parameter;
			return SB_OK;
		}
	}
	return SB_NOT_FOUND;
}

sb_code sb_parameter_get_value(sb_parameter_parser* parser, char** out, char* name) {
	sb_parameter parameter;
	sb_parameter_get(parser, &parameter, name);
	for (int i = 0; i < parser->arg_count; i++) {
		if (strcmp(parameter.name, parser->args[i]) != SB_OK)
			continue;

		if (parameter.flag) {
			*out = "true";
			return SB_OK;
		}

		i++;
		*out = parser->args[i];
		return SB_OK;
	}

	if (parameter.required)
		return SB_REQUIRED;

	if (parameter.flag) {
		*out = "false";
		return SB_OK;
	}

	sb_parameter_get(parser, &parameter, name);
	*out = parameter.default_value;

	return SB_OK;
}
