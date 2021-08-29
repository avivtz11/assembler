#ifndef ERROR_CODES_H
#define ERROR_CODES_H

#define ILLEGAL_COMMAND 2
#define LABEL_NOT_FOUND 5
#define TOO_MUCH_ARGS 4
#define INVALID_PARAM_FORMAT 1
#define PARAM_OUT_OF_RANGE 3

#define REDUNDANT_COMMA_IN_PARAMS -1
#define MISSING_COMMA_IN_PARAMS -2

void line_error(int err_code, int line_num);

#endif
