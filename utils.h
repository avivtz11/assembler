#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

void malloc_with_error(void **destination, int allocation_size);
void skip_white_space(char **str);
void change_path_extension(char **result, char *file_path, char *new_extension);
void num2bin(long int num, char *result_buffer, int result_buffer_size);
void binary_32_to_bytes(char **result, char *as_binary, int is_litte_endian);

#endif/*UTILS_H*/
