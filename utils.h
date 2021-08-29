#include <stdio.h>

void malloc_with_error(void **destination, int allocation_size, char *error_message);
void skip_white_space(char **str);
void copy_char_array(char *src, char *dest, int *dest_position);
void change_path_extension(char **result, char *file_path, char *new_extension);
void num2bin(long int num, char *result_buffer, int result_buffer_size);
void format_output_bytes(char *current_output_bytes, int bytes_count, FILE *fp);
void binary_32_to_bytes(char **result, char *as_binary, int is_litte_endian);
