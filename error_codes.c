#include <stdio.h>

#include "error_codes.h"


void line_error(int err_code, int line_num)
{
	char *err_message;

	switch(err_code)
	{
		case REDUNDANT_COMMA_IN_PARAMS:
			err_message = "line %d: redundant comma between params\n";
			break;
		case MISSING_COMMA_IN_PARAMS:
			err_message = "line %d: missing comma between params\n";
			break;
		case INVALID_PARAM_FORMAT:
			err_message = "line %d: data param is not in its matching format\n";
			break;
		case ILLEGAL_COMMAND:
			err_message = "line %d: command is illegal\n";
			break;
		case PARAM_OUT_OF_RANGE:
			err_message = "line %d: the param is out of its allowed range\n";
			break;
		case TOO_MUCH_ARGS:
			err_message = "line %d: too much args were given\n";
			break;
		case LABEL_NOT_FOUND:
			err_message = "line %d: legal matching label wasn't found\n";
			break;
		default:
			err_message = "line %d: unknown error\n";
	}
	printf(err_message, line_num);
}
