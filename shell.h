#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* Buffer sizes for read/write */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* Command chaining types */
#define CMD_NORM 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

/* Number conversion types */
#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

/* Set to 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HISTORY_FILE ".simple_shell_history"
#define HISTORY_MAX 4096

extern char **environ;

/**
 * struct LinkedListString - singly linked list for strings
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct LinkedListString
{
	int num;
	char *str;
	struct LinkedListString *next;
} list_t;

/**
 * struct CommandLineArgs - contains pseudo-arguments to pass into a function,
 * allowing uniform prototype for function pointer struct
 * @arg_str: a string generated from getline containing arguments
 * @argv: an array of strings generated from arg_str
 * @path: a string path for the current command
 * @argc: the argument count
 * @line_count: the error count
 * @err_num: the error code for exit()s
 * @linecount_flag: if on, count this line of input
 * @fname: the program filename
 * @env_list: linked list local copy of environ
 * @custom_environ: custom modified copy of environ from env_list
 * @history: the history node
 * @alias: the alias node
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, on if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd: the fd from which to read line input
 * @histcount: the history line number count
 */
typedef struct CommandLineArgs
{
	char *arg_str;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env_list;
	list_t *history;
	list_t *alias;
	char **custom_environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory management */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
		0, 0, 0}

/**
 * struct BuiltInCommands - contains a built-in string and related function
 * @type: the built-in command flag
 * @func: the function
 */
typedef struct BuiltInCommands
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* Function prototypes */
int shell_loop(info_t *, char **);
int find_builtin_command(info_t *);
void find_command(info_t *);
void fork_command(info_t *);

/* Parser */
int is_command(info_t *, char *);
char *duplicate_chars(char *, int, int);
char *find_path(info_t *, char *, char *);

/* Loop */
int main_loop(char **);

/* Error Handling */
void print_error_string(char *);
int print_error_char(char);
int write_to_fd(char c, int fd);
int write_string_to_fd(char *str, int fd);

/* String Utils */
int string_length(char *);
int compare_strings(char *, char *);
char *starts_with_string(const char *, const char *);
char *string_concat(char *, char *);

/* String Utils 2 */
char *string_copy(char *, char *);
char *string_duplicate(const char *);
void print_string(char *);
int print_char(char);

/* Exits */
char *string_copy_n(char *, char *, int);
char *string_concat_n(char *, char *, int);
char *string_character(char *, char);

/* Tokenizer */
char **string_to_words(char *, char *);
char **string_to_words_2(char *, char);

/* Realloc */
char *memory_set(char *, char, unsigned int);
void free_2D_array(char **);
void *reallocate_memory(void *, unsigned int, unsigned int);

/* Memory Management */
int free_buffer(void **);

/* Atoi */
int check_if_interactive(info_t *);
int is_delimiter(char, char *);
int is_alphabetic(int);
int string_to_integer(char *);

/* Error Handling 2 */
int error_string_to_integer(char *);
void print_error_info(info_t *, char *);
int print_integer_decimal(int, int);
char *convert_number_to_string(long int, int, int);
void remove_comments_from_string(char *);

/* Built-in Commands */
int exit_shell(info_t *);
int change_directory(info_t *);
int help_shell(info_t *);

/* Built-in Commands 2 */
int print_history(info_t *);
int print_alias(info_t *);

/* Getline */
ssize_t get_input_line(info_t *);
int custom_getline(info_t *, char **, size_t *);
void signal_interrupt_handler(int);

/* Get Info */
void clear_command_line_args(info_t *);
void set_command_line_args(info_t *, char **);
void free_command_line_args(info_t *, int);

/* Environment */
char *get_environment_variable(info_t *, const char *);
int print_environment(info_t *);
int set_environment_variable(info_t *);
int unset_environment_variable(info_t *);
int populate_environment_list(info_t *);

/* Get Environment */
char **get_custom_environment(info_t *);
int unset_custom_environment(info_t *, char *);
int set_custom_environment(info_t *, char *, char *);

/* History */
char *get_history_file(info_t *info);
int write_history_to_file(info_t *info);
int read_history_from_file(info_t *info);
int build_history_list(info_t *info, char *buffer, int linecount);
int renumber_history(info_t *info);

/* Linked List */
list_t *add_node_to_list(list_t **, const char *, int);
list_t *add_node_to_list_end(list_t **, const char *, int);
size_t print_list_strings(const list_t *);
int delete_node_from_list_at_index(list_t **, unsigned int);
void free_list_nodes(list_t **);

/* Linked List 2 */
size_t list_length(const list_t *);
char **list_to_string_array(list_t *);
size_t print_list(const list_t *);
list_t *find_node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* Variables */
int is_command_chaining(info_t *, char *, size_t *);
void check_command_chaining(info_t *, char *, size_t *, size_t, size_t);
int replace_alias_in_string(info_t *);
int replace_variables_in_string(info_t *);
int replace_string_contents(char **, char *);

#endif
