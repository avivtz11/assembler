#define MAX_LINE 82
#define is_comment_or_empty(X) ((*(X) == '\n') || (*(X) == ';'))
#define is_symbol_def(X) (*((X) + strlen(X) - 1) == ':')
#define is_data_storage_line(X) ((strcmp((X), ".dh") == 0) || (strcmp((X), ".dw") == 0) || (strcmp((X), ".db") == 0) || (strcmp((X), ".asciz") == 0))
#define is_extern_def(X) (strcmp((X), ".extern") == 0)
#define is_entry_def(X) (strcmp((X), ".entry") == 0)

void get_next_word(char **result_word, char **current_char);
